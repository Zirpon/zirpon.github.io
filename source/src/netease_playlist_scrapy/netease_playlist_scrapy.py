import requests
import json
import time
import logging
from tinydb import TinyDB, Query
from datetime import datetime, timedelta

# ---------------------------- 配置区 ----------------------------
SONGS = ["你瞒我瞒", "连续剧"]            # 需要匹配的歌曲
SEARCH_LIMIT = 100                      # 每个关键词搜索的歌单数量
REQUEST_DELAY = 1                       # 请求间隔（秒）
CACHE_TTL_DAYS = 7                      # 缓存有效期（天）
CACHE_DB_PATH = "playlist_cache.json"   # TinyDB 缓存文件路径

# 官方接口请求头
HEADERS = {
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/122.0.0.0 Safari/537.36",
    "Referer": "https://music.163.com/"
}

# 配置 logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S"
)
#logger = logging.getLogger(__name__)

from utils import mylog
logger = mylog.mylogger(__name__)

# 初始化 TinyDB 缓存数据库
db = TinyDB(CACHE_DB_PATH)
cache_table = db.table("playlist_cache")   # 表名
PlaylistQuery = Query()

# --------------------------------------------------------------

def search_playlists_by_keyword(keyword, limit=SEARCH_LIMIT, offset=0):
    """根据关键词搜索歌单（type=1000）"""
    url = "https://music.163.com/api/search/get/web"
    params = {
        "s": keyword,
        "type": 1000,
        "limit": limit,
        "offset": offset
    }
    try:
        resp = requests.get(url, headers=HEADERS, params=params, timeout=10)
        resp.raise_for_status()
        data = resp.json()
        if data.get("code") == 200:
            playlists = data["result"].get("playlists", [])
            result = []
            for p in playlists:
                result.append({
                    "id": p["id"],
                    "name": p["name"],
                    "playCount": p.get("playCount", 0)
                })
            logger.info(f"关键词 '{keyword}' 搜索到 {len(result)} 个歌单")
            return result
        else:
            logger.warning(f"关键词 '{keyword}' 搜索失败，code={data.get('code')}")
            return []
    except Exception as e:
        logger.error(f"搜索关键词 '{keyword}' 异常: {e}")
        return []

def get_playlist_tracks_official(playlist_id):
    """官方接口：获取歌单详情，返回歌曲名称集合（已去重）"""
    url = f"https://music.163.com/api/playlist/detail?id={playlist_id}"
    try:
        resp = requests.get(url, headers=HEADERS, timeout=10)
        resp.raise_for_status()
        data = resp.json()
        if data.get("code") == 200:
            tracks = data["result"].get("tracks", [])
            song_names = {track["name"] for track in tracks}
            return song_names
        else:
            logger.warning(f"官方接口获取歌单 {playlist_id} 详情失败，code={data.get('code')}")
            return set()
    except Exception as e:
        logger.error(f"官方接口获取歌单 {playlist_id} 详情异常: {e}")
        return set()

def get_cached_playlist_songs(playlist_id):
    """从 TinyDB 缓存中获取歌单歌曲名称列表，如果缓存有效则返回歌曲名称集合，否则返回 None"""
    doc = cache_table.get(PlaylistQuery.playlist_id == playlist_id)
    if doc:
        # 检查是否过期
        expire_time = datetime.fromisoformat(doc["expire_time"])
        if datetime.now() < expire_time:
            # 缓存有效
            song_names = set(doc["song_names"])
            logger.info(f"缓存命中 - 歌单 {playlist_id}，共 {len(song_names)} 首歌曲")
            return song_names
        else:
            # 缓存已过期，删除该条记录
            cache_table.remove(PlaylistQuery.playlist_id == playlist_id)
            logger.info(f"缓存已过期 - 歌单 {playlist_id}")
    return None

def set_cached_playlist_songs(playlist_id, song_names):
    """将歌单歌曲列表存入 TinyDB 缓存，设置过期时间"""
    expire_time = datetime.now() + timedelta(days=CACHE_TTL_DAYS)
    doc = {
        "playlist_id": playlist_id,
        "song_names": list(song_names),   # 存储为列表（JSON 可序列化）
        "expire_time": expire_time.isoformat()
    }
    cache_table.upsert(doc, PlaylistQuery.playlist_id == playlist_id)
    logger.info(f"缓存已更新 - 歌单 {playlist_id}，有效期 {CACHE_TTL_DAYS} 天")

def get_playlist_tracks_fallback(playlist_id):
    """
    备用接口（oiapi.net）：获取歌单详情，返回歌曲名称集合
    先查缓存，未命中则请求接口并存入缓存
    """
    # 1. 尝试从缓存获取
    cached_songs = get_cached_playlist_songs(playlist_id)
    if cached_songs is not None:
        return cached_songs

    # 2. 缓存未命中，请求备用接口
    url = f"https://www.oiapi.net/api/NeteasePlaylistDetail?id={playlist_id}"
    try:
        resp = requests.get(url, timeout=15)
        resp.raise_for_status()
        data = resp.json()
        # oiapi.net 成功时 code=1，data 为歌曲列表
        if data.get("code") == 1:
            songs = data.get("data", [])
            song_names = {song["name"] for song in songs if "name" in song}
            logger.info(f"备用接口获取歌单 {playlist_id} 成功，共 {len(song_names)} 首歌曲")
            # 存入缓存
            set_cached_playlist_songs(playlist_id, song_names)
            return song_names
        else:
            logger.warning(f"备用接口获取歌单 {playlist_id} 失败，code={data.get('code')}, message={data.get('message')}")
            return set()
    except Exception as e:
        logger.error(f"备用接口获取歌单 {playlist_id} 异常: {e}")
        return set()

def get_playlist_tracks_with_fallback(playlist_id):
    """
    获取歌单歌曲列表，带备用降级及本地缓存机制：
    1. 优先使用官方接口（无缓存）
    2. 如果官方接口返回的歌曲列表中缺少目标歌曲，则尝试备用接口（带 TinyDB 缓存）
    3. 返回最终合并后的歌曲列表
    """
    official_tracks = get_playlist_tracks_official(playlist_id)
    
    # 检查官方接口是否已经包含所有目标歌曲
    if False and all(song in official_tracks for song in SONGS):
        logger.info(f"歌单 {playlist_id} 官方接口已匹配成功，无需备用接口")
        return official_tracks
    
    # 官方接口匹配失败，尝试备用接口（带缓存）
    logger.info(f"歌单 {playlist_id} 官方接口匹配失败，尝试备用接口（缓存优先）...")
    fallback_tracks = get_playlist_tracks_fallback(playlist_id)
    
    # 合并两个来源的歌曲集合（去重）
    merged_tracks = official_tracks.union(fallback_tracks)
    
    if len(merged_tracks) > len(official_tracks):
        logger.info(f"备用接口补充了 {len(merged_tracks) - len(official_tracks)} 首歌曲")
    
    return merged_tracks

def main():
    logger.info("=" * 60)
    logger.info("网易云音乐歌单爬虫（官方接口 + 备用接口双重验证 + TinyDB缓存）")
    logger.info("=" * 60)

    # 1. 分别搜索每首歌，收集所有候选歌单（去除重复id）
    candidate_map = {}
    for song in SONGS:
        logger.info(f"开始搜索包含《{song}》的歌单...")
        playlists = search_playlists_by_keyword(song, limit=SEARCH_LIMIT)
        for pl in playlists:
            pid = pl["id"]
            if pid not in candidate_map:
                candidate_map[pid] = {
                    "id": pid,
                    "name": pl["name"],
                    "playCount": pl["playCount"]
                }
        time.sleep(REQUEST_DELAY)

    candidate_list = list(candidate_map.values())
    logger.info(f"候选歌单总数（去重后）: {len(candidate_list)}")

    if not candidate_list:
        logger.warning("未找到任何候选歌单，程序结束")
        return

    # 2. 逐个验证候选歌单是否同时包含两首歌（带备用降级 & 缓存）
    matched_playlists = []
    total = len(candidate_list)
    for idx, pl in enumerate(candidate_list, 1):
        logger.info(f"正在验证 [{idx}/{total}] 歌单: {pl['name']} (ID: {pl['id']})")
        song_set = get_playlist_tracks_with_fallback(pl["id"])
        
        if all(song in song_set for song in SONGS):
            matched_playlists.append({
                "playlist_id": pl["id"],
                "playlist_name": pl["name"],
                "play_count": pl["playCount"],
                "matched_songs": SONGS
            })
            logger.info(f"  ✅ 匹配！同时包含 {', '.join(SONGS)}")
        else:
            missing = [s for s in SONGS if s not in song_set]
            logger.info(f"  ❌ 不匹配，缺失: {', '.join(missing)}")
        #time.sleep(REQUEST_DELAY)

    # 3. 输出最终结果
    logger.info("=" * 60)
    if not matched_playlists:
        logger.info("没有找到同时包含指定歌曲的歌单。")
        return

    logger.info(f"✅ 共发现 {len(matched_playlists)} 个同时包含《{SONGS[0]}》和《{SONGS[1]}》的歌单：")
    for idx, pl in enumerate(matched_playlists, 1):
        logger.info(f"\n{idx}. 歌单名称：{pl['playlist_name']}")
        logger.info(f"   歌单 ID：{pl['playlist_id']}")
        logger.info(f"   播放量：{pl['play_count']}")
        logger.info(f"   链接：https://music.163.com/playlist?id={pl['playlist_id']}")

    # 4. 保存到文件
    with open("matched_playlists.json", "w", encoding="utf-8") as f:
        json.dump(matched_playlists, f, ensure_ascii=False, indent=2)
    logger.info(f"\n结果已保存至 matched_playlists.json")

if __name__ == "__main__":
    main()