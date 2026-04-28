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


def get_playlist_metadata_official(playlist_id):
    """
    官方接口：获取歌单详情，返回歌曲名称集合 + 歌单元数据
    返回: (song_names_set, metadata_dict)
    """
    url = f"https://music.163.com/api/playlist/detail?id={playlist_id}"
    try:
        resp = requests.get(url, headers=HEADERS, timeout=10)
        resp.raise_for_status()
        data = resp.json()
        if data.get("code") == 200:
            playlist_data = data.get("result", {})
            tracks = data["result"].get("tracks", [])
            song_names = {track["name"] for track in tracks}

            # 提取歌单元数据
            metadata = {
                "playlist_name": playlist_data.get("name"),
                "play_count": playlist_data.get("playCount", 0),
                "comment_count": playlist_data.get("commentCount", 0),
                "subscribed_count": playlist_data.get("subscribedCount", 0),
                "tags": playlist_data.get("tags", [])
            }
            return song_names, metadata
        else:
            logger.warning(f"官方接口获取歌单 {playlist_id} 详情失败，code={data.get('code')}")
            return set(), {}
    except Exception as e:
        logger.error(f"官方接口获取歌单 {playlist_id} 详情异常: {e}")
        return set(), {}


def get_cached_playlist_metadata(playlist_id):
    """从 TinyDB 缓存中获取歌单元数据，如果缓存有效则返回(song_names, metadata)。"""
    doc = cache_table.get(PlaylistQuery.playlist_id == playlist_id)
    if doc:
        expire_time = datetime.fromisoformat(doc["expire_time"])
        if datetime.now() < expire_time:
            song_names = set(doc["song_names"])
            metadata = doc.get("metadata", {})
            logger.info(f"缓存命中 - 歌单 {playlist_id}，共 {len(song_names)} 首歌曲")
            return song_names, metadata
        else:
            cache_table.remove(PlaylistQuery.playlist_id == playlist_id)
            logger.info(f"缓存已过期 - 歌单 {playlist_id}")
    return None, None


def set_cached_playlist_metadata(playlist_id, song_names, metadata):
    """将歌单元数据存入 TinyDB 缓存。"""
    expire_time = datetime.now() + timedelta(days=CACHE_TTL_DAYS)
    doc = {
        "playlist_id": playlist_id,
        "song_names": list(song_names),
        "metadata": metadata,
        "expire_time": expire_time.isoformat()
    }
    cache_table.upsert(doc, PlaylistQuery.playlist_id == playlist_id)
    logger.info(f"缓存已更新 - 歌单 {playlist_id}，有效期 {CACHE_TTL_DAYS} 天")


def get_playlist_metadata_fallback(playlist_id):
    """
    备用接口（oiapi.net）：先查缓存，未命中则请求接口并存入缓存。
    返回: (song_names_set, metadata_dict)
    """
    # 1. 尝试从缓存获取
    cached_songs, cached_metadata = get_cached_playlist_metadata(playlist_id)
    if cached_songs is not None:
        return cached_songs, cached_metadata

    # 2. 缓存未命中，请求备用接口
    url = f"https://www.oiapi.net/api/NeteasePlaylistDetail?id={playlist_id}"
    try:
        resp = requests.get(url, timeout=15)
        resp.raise_for_status()
        data = resp.json()
        if data.get("code") == 1:
            songs = data.get("data", [])
            song_names = {song["name"] for song in songs if "name" in song}

            # 备用接口的数据结构与官方不同，没有直接返回评论区数、收藏数，设置为默认值
            # 备用接口只返回歌曲列表，无法获取评论/收藏/标签
            metadata = {
                "playlist_name": f"Playlist_{playlist_id}",  # 备用接口不返回歌单名
                "play_count": 0,
                "comment_count": 0,
                "subscribed_count": 0,
                "tags": []
            }
            # 从备用接口返回的第一首歌里尝试获取歌单名（如果有）
            if songs and "playlist_name" in songs[0]:
                metadata["playlist_name"] = songs[0].get("playlist_name", f"Playlist_{playlist_id}")

            logger.info(f"备用接口获取歌单 {playlist_id} 成功，共 {len(song_names)} 首歌曲")
            # 备用接口数据存入缓存
            set_cached_playlist_metadata(playlist_id, song_names, metadata)
            return song_names, metadata
        else:
            logger.warning(f"备用接口获取歌单 {playlist_id} 失败，code={data.get('code')}, message={data.get('message')}")
            return set(), {}
    except Exception as e:
        logger.error(f"备用接口获取歌单 {playlist_id} 异常: {e}")
        return set(), {}


def get_playlist_with_fallback(playlist_id):
    """
    获取歌单完整信息（歌曲集合 + 元数据）：
    1. 优先使用官方接口
    2. 官方接口缺失目标歌曲则备用接口补充歌曲列表（元数据仍参考官方）
    3. 返回歌曲名称集合和整合后的元数据
    """
    official_songs, official_metadata = get_playlist_metadata_official(playlist_id)

    # 如果官方已有完整元数据（包含playlist_name），直接使用
    fallback_songs = set()
    if all(song in official_songs for song in SONGS):
        logger.info(f"歌单 {playlist_id} 官方接口已匹配成功，无需备用接口")
        merged_songs = official_songs
    else:
        logger.info(f"歌单 {playlist_id} 官方接口歌曲不足，尝试备用接口补充...")
        fallback_songs, _ = get_playlist_metadata_fallback(playlist_id)
        merged_songs = official_songs.union(fallback_songs)
        if len(merged_songs) > len(official_songs):
            logger.info(f"备用接口补充了 {len(merged_songs) - len(official_songs)} 首歌曲")        

    # 元数据以官方为准（备用接口一般缺少）
    metadata = {
        "playlist_name": official_metadata.get("playlist_name") or f"Playlist_{playlist_id}",
        "play_count": official_metadata.get("play_count", 0),
        "comment_count": official_metadata.get("comment_count", 0),
        "subscribed_count": official_metadata.get("subscribed_count", 0),
        "tags": official_metadata.get("tags", [])
    }
    return merged_songs, metadata


def main():
    logger.info("=" * 60)
    logger.info("网易云音乐歌单爬虫（显示评数/收藏数/标签）")
    logger.info("=" * 60)

    # 1. 分别搜索每首歌，收集候选歌单
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

    # 2. 逐个验证，并记录元数据
    matched_playlists = []  # 每个元素将包含 song_count 及元数据
    total = len(candidate_list)
    for idx, pl in enumerate(candidate_list, 1):
        logger.info(f"正在验证 [{idx}/{total}] 歌单: {pl['name']} (ID: {pl['id']})")
        song_set, metadata = get_playlist_with_fallback(pl["id"])
        song_count = len(song_set)

        if all(song in song_set for song in SONGS):
            matched_playlists.append({
                "playlist_id": pl["id"],
                "playlist_name": pl['name'],
                "play_count": metadata["play_count"],
                "comment_count": metadata["comment_count"],
                "subscribed_count": metadata["subscribed_count"],
                "tags": metadata["tags"],
                "song_count": song_count,
                "matched_songs": SONGS,
                "url": f"https://music.163.com/playlist?id={pl['playlist_id']}"
            })
            logger.info(f"  ✅ 匹配！歌曲总数：{song_count}，同时包含 {', '.join(SONGS)}")
        else:
            missing = [s for s in SONGS if s not in song_set]
            logger.info(f"  ❌ 不匹配，歌曲总数：{song_count}，缺失: {', '.join(missing)}")
        time.sleep(REQUEST_DELAY)

    # 3. 按歌曲数量从小到大排序
    matched_playlists.sort(key=lambda x: x["song_count"])

    # 4. 输出结果并保存文件
    logger.info("=" * 60)
    if not matched_playlists:
        logger.info("没有找到同时包含指定歌曲的歌单。")
        return

    logger.info(f"✅ 共发现 {len(matched_playlists)} 个同时包含《{SONGS[0]}》和《{SONGS[1]}》的歌单（按歌曲数升序）：")
    for idx, pl in enumerate(matched_playlists, 1):
        logger.info(f"\n{idx}. 歌单名称：{pl['playlist_name']}")
        logger.info(f"   歌单 ID：{pl['playlist_id']}")
        if pl['tags']:
            logger.info(f"   标  签：{', '.join(pl['tags'])}")
        else:
            logger.info("   标  签：无")
        logger.info(f"   收藏数：{pl['subscribed_count']}")
        logger.info(f"   评论数：{pl['comment_count']}")
        logger.info(f"   播放量：{pl['play_count']}")
        logger.info(f"   歌曲总数：{pl['song_count']}")
        logger.info(f"   链  接：https://music.163.com/playlist?id={pl['playlist_id']}")

    # 动态生成文件名
    safe_songs = [song.replace("/", "_").replace("\\", "_") for song in SONGS]
    filename = f"{safe_songs[0]}_{safe_songs[1]}_matched.json"
    with open("./logs/"+filename, "w", encoding="utf-8") as f:
        json.dump(matched_playlists, f, ensure_ascii=False, indent=2)
    logger.info(f"\n结果已保存至 {filename}")

if __name__ == "__main__":
    main()