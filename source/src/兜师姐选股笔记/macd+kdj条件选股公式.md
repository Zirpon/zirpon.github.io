/*欢迎使用同花顺公式系统*/
/*按“CTRL+S”启动AI，可输入需求描述生成相应代码*/
// MACD KDJ三位一体选股系统 - 实战优化版
// 适用于同花顺条件选股

// --- 指标计算 ---
SHORT := 12;
LONG := 26;
MID := 9;

DIF := EMA(CLOSE, SHORT) - EMA(CLOSE, LONG);
DEA := EMA(DIF, MID);
MACD := 2*(DIF - DEA);

RSV := (CLOSE - LLV(LOW, 9)) / (HHV(HIGH, 9) - LLV(LOW, 9)) * 100;
K1 := SMA(RSV, 3, 1);
D1 := SMA(K1, 3, 1);
J1 := 3*K1 - 2*D1;

VOL5 := MA(VOL, 5);
VOL10 := MA(VOL, 10);

// --- 优化后的买入信号 ---

// 1. 金叉信号（放宽条件）
MACD金叉 := CROSS(DIF, DEA);
KDJ金叉 := CROSS(K1, D1);

// 2. 低位区域（放宽J值范围）
J值低位 := J1 < 50;  // 原来30，放宽到50
J值超卖 := J1 < 30;  // 保留超卖判断

// 3. 成交量条件（放宽）
量能配合 := VOL > REF(VOL, 1) * 1.1;  // 只要比昨天放量10%即可
量能活跃 := VOL > VOL5 * 0.8;  // 成交量不低于5日均量的80%

// 4. MACD位置（多种情况）
MACD水上 := DIF > 0;
MACD水下 := DIF < 0;
MACD走强 := DIF > DEA;  // MACD红柱状态
MACD底背离 := DIF > REF(DIF, 1) AND CLOSE < REF(CLOSE, 1);  // 价格跌，DIF涨

// 5. 均线支撑（增加安全性）
MA5 := MA(CLOSE, 5);
MA10 := MA(CLOSE, 10);
MA20 := MA(CLOSE, 20);
MA60 := MA(CLOSE, 60);

// 价格在均线附近（不追高）
价格合理 := CLOSE <= MA20 * 1.05;  // 不高于20日线5%

// 6. 底部形态识别
近20日最低 := LLV(LOW, 20);
近20日最高 := HHV(HIGH, 20);
近20日涨幅 := (CLOSE - 近20日最低) / 近20日最低 * 100;
底部区域 := 近20日涨幅 < 15;  // 从20日低点上涨不超过15%

// 7. 新增：KDJ底背离
价格新低 := LOW < REF(LLV(LOW, 5), 1);
J值抬高 := J1 > REF(J1, 1) AND REF(J1, 1) < REF(J1, 2);
简单底背离 := 价格新低 AND J值抬高 AND J1 < 50;

// --- 实战选股方案（保证能选出股票）---

// 方案A：稳健抄底型（适合震荡市）
抄底信号 := (KDJ金叉 OR J值超卖) 
           AND J值低位 
           AND 量能配合
           AND CLOSE > MA5  // 站上5日线
           AND (MACD金叉 OR MACD走强);

// 方案B：强势回调型（适合上升趋势）
回调信号 := MACD水上 
           AND DIF > DEA 
           AND J1 < 60  // J线回调到60以下
           AND J1 > REF(J1, 1)  // J线开始拐头
           AND CLOSE > MA20  // 在20日线上方
           AND VOL > VOL5 * 0.9;

// 方案C：底部反弹型（适合超跌反弹）
反弹信号 := MACD水下 
           AND J1 < 40 
           AND KDJ金叉
           AND CLOSE > REF(CLOSE, 1)  // 今日上涨
           AND 底部区域
           AND 量能配合;

// 方案D：底背离型（左侧交易）
背离信号 := 简单底背离 
           AND J1 < 45
           AND VOL > REF(VOL, 1)
           AND CLOSE < MA20 * 0.98;  // 在20日线下方（超跌）

// 方案E：最简单实用型（保证能选出股票）
简单信号 := (KDJ金叉 OR J1 < 35)  // KDJ金叉或超卖
           AND CLOSE > REF(CLOSE, 1)  // 今日上涨
           AND VOL > REF(VOL, 1)  // 今日放量
           AND (MACD金叉 OR DIF > DEA OR MACD底背离)  // MACD任一积极信号
           AND CLOSE > MA5;  // 站上5日线

// 方案F：温和启动型（最宽松，适合日常选股）
温和信号 := J1 < 65  // J线不高
           AND J1 > REF(J1, 1)  // J线拐头向上
           AND VOL > REF(VOL, 1) * 0.95  // 成交量不萎缩
           AND CLOSE > OPEN  // 收阳线（或十字星）
           AND (DIF > DEA OR MACD金叉 OR REF(DIF, 1) < REF(DEA, 1));  // MACD好转

// 方案G：MACD金叉优先型
MACD优先信号 := MACD金叉 
               AND J1 < 70  // J线只要不在绝对高位
               AND VOL > REF(VOL, 1)
               AND CLOSE > MA5;

// 方案H：KDJ金叉优先型
KDJ优先信号 := KDJ金叉 
               AND J1 < 60
               AND VOL > REF(VOL, 1)
               AND (DIF > DEA OR DIF > -0.2);  // MACD不能太弱

// 方案I：综合精选（平衡数量和质量）
精选信号 := (KDJ金叉 OR J1 < 40 OR 简单底背离)
           AND (MACD金叉 OR DIF > DEA OR MACD底背离)
           AND VOL > REF(VOL, 1) * 0.9
           AND CLOSE > REF(CLOSE, 1)
           AND J1 < 70
           AND (CLOSE > MA5 OR CLOSE > REF(CLOSE, 2));  // 站上5日线或比2天前高

// 方案J：超简版（最容易选出股票）
超简信号 := J1 < 60  // J线在60以下
           AND J1 > REF(J1, 1)  // J线向上
           AND VOL > REF(VOL, 1)  // 放量
           AND CLOSE > REF(CLOSE, 1);  // 上涨

// --- 最终输出（默认使用方案I：精选信号，平衡数量和胜率）---
// 这个方案应该能选出一定数量的股票，如果还选不出，换成方案J

选股条件: 抄底信号 or 回调信号 or 反弹信号 or 背离信号 or 简单信号 or 温和信号 or MACD优先信号 or KDJ优先信号 or 精选信号 or 超简信号;

// 备用选项（如果精选信号选不出，可以换成下面任意一个）：
// 选股条件: 超简信号;      // 最容易选出
// 选股条件: 温和信号;      // 也很宽松
// 选股条件: 简单信号;      // 实用型
// 选股条件: MACD优先信号;  // 重视MACD金叉
// 选股条件: KDJ优先信号;   // 重视KDJ金叉