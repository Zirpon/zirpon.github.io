/*欢迎使用同花顺公式系统*/
/*按“CTRL+S”启动AI，可输入需求描述生成相应代码*/
// MACD KDJ三位一体选股系统 - 指标+方案分离版
// 适用于同花顺条件选股

// ================== 第一部分：基础指标计算 ==================

// --- MACD指标 ---
SHORT := 12;
LONG := 26;
MID := 9;

DIF := EMA(CLOSE, SHORT) - EMA(CLOSE, LONG);
DEA := EMA(DIF, MID);
MACD := 2*(DIF - DEA);

// --- KDJ指标 ---
RSV := (CLOSE - LLV(LOW, 9)) / (HHV(HIGH, 9) - LLV(LOW, 9)) * 100;
K1 := SMA(RSV, 3, 1);
D1 := SMA(K1, 3, 1);
J1 := 3*K1 - 2*D1;

// --- 成交量指标 ---
VOL5 := MA(VOL, 5);
VOL10 := MA(VOL, 10);
VOL20 := MA(VOL, 20);

// --- 均线指标 ---
MA5 := MA(CLOSE, 5);
MA10 := MA(CLOSE, 10);
MA20 := MA(CLOSE, 20);
MA60 := MA(CLOSE, 60);
MA120 := MA(CLOSE, 120);

// --- 价格位置指标 ---
近5日最低 := LLV(LOW, 5);
近10日最低 := LLV(LOW, 10);
近20日最低 := LLV(LOW, 20);
近20日最高 := HHV(HIGH, 20);
近20日涨幅 := (CLOSE - 近20日最低) / 近20日最低 * 100;
近60日最低 := LLV(LOW, 60);
近60日最高 := HHV(HIGH, 60);
近60日跌幅 := (近60日最高 - CLOSE) / 近60日最高 * 100;

// ================== 第二部分：MACD相关信号 ==================

// --- MACD金叉死叉 ---
MACD金叉 := CROSS(DIF, DEA);
MACD死叉 := CROSS(DEA, DIF);

// --- MACD位置状态 ---
MACD水上 := DIF > 0;
MACD水下 := DIF < 0;
MACD走强 := DIF > DEA;  // 红柱状态
MACD走弱 := DIF < DEA;  // 绿柱状态
MACD向上 := DIF > REF(DIF, 1);
MACD向下 := DIF < REF(DIF, 1);

// --- MACD柱状体 ---
MACD红柱 := MACD > 0;
MACD绿柱 := MACD < 0;
MACD红柱放大 := MACD > REF(MACD, 1) AND MACD > 0;
MACD绿柱缩小 := MACD > REF(MACD, 1) AND MACD < 0;

// --- MACD背离 ---
MACD底背离 := DIF > REF(DIF, 1) AND CLOSE < REF(CLOSE, 1);
MACD顶背离 := DIF < REF(DIF, 1) AND CLOSE > REF(CLOSE, 1);

// ================== 第三部分：KDJ相关信号 ==================

// --- KDJ金叉死叉 ---
KDJ金叉 := CROSS(K1, D1);
KDJ死叉 := CROSS(D1, K1);

// --- KDJ位置状态 ---
K值低位 := K1 < 20;
K值中位 := K1 >= 20 AND K1 <= 80;
K值高位 := K1 > 80;

J值超卖 := J1 < 20;
J值低位 := J1 < 30;
J值中低位 := J1 < 40;
J值中位 := J1 >= 40 AND J1 <= 60;
J值中高位 := J1 > 60;
J值高位 := J1 > 70;
J值超买 := J1 > 80;

// --- KDJ趋势 ---
J线向上 := J1 > REF(J1, 1);
J线向下 := J1 < REF(J1, 1);
J线连续向上 := J1 > REF(J1, 1) AND REF(J1, 1) > REF(J1, 2);
J线连续向下 := J1 < REF(J1, 1) AND REF(J1, 1) < REF(J1, 2);

K线向上 := K1 > REF(K1, 1);
D线向上 := D1 > REF(D1, 1);

// --- KDJ背离 ---
价格新低 := LOW < REF(LLV(LOW, 10), 1);
J值抬高 := J1 > REF(J1, 1) AND REF(J1, 1) < REF(J1, 2);
KDJ底背离 := 价格新低 AND J值抬高;

价格新高 := HIGH > REF(HHV(HIGH, 10), 1);
J值降低 := J1 < REF(J1, 1) AND REF(J1, 1) > REF(J1, 2);
KDJ顶背离 := 价格新高 AND J值降低;

// ================== 第四部分：成交量相关信号 ==================

// --- 放量缩量 ---
今日放量 := VOL > REF(VOL, 1) * 1.2;
温和放量 := VOL > REF(VOL, 1) * 1.1;
微微放量 := VOL > REF(VOL, 1) * 1.05;
平量 := VOL >= REF(VOL, 1) * 0.95 AND VOL <= REF(VOL, 1) * 1.05;
微微缩量 := VOL < REF(VOL, 1) * 0.95;
明显缩量 := VOL < REF(VOL, 1) * 0.8;

// --- 均量关系 ---
高于5日均量 := VOL > VOL5;
高于10日均量 := VOL > VOL10;
高于20日均量 := VOL > VOL20;

5日均量向上 := VOL5 > REF(VOL5, 1);
10日均量向上 := VOL10 > REF(VOL10, 1);

// --- 放量组合 ---
放量上涨 := VOL > REF(VOL, 1) AND CLOSE > REF(CLOSE, 1);
放量下跌 := VOL > REF(VOL, 1) AND CLOSE < REF(CLOSE, 1);
缩量上涨 := VOL < REF(VOL, 1) AND CLOSE > REF(CLOSE, 1);
缩量下跌 := VOL < REF(VOL, 1) AND CLOSE < REF(CLOSE, 1);

// ================== 第五部分：价格形态信号 ==================

// --- 价格与均线关系 ---
站上5日线 := CLOSE > MA5;
站上10日线 := CLOSE > MA10;
站上20日线 := CLOSE > MA20;
站上60日线 := CLOSE > MA60;

跌破5日线 := CLOSE < MA5;
跌破10日线 := CLOSE < MA10;
跌破20日线 := CLOSE < MA20;
跌破60日线 := CLOSE < MA60;

5日线向上 := MA5 > REF(MA5, 1);
10日线向上 := MA10 > REF(MA10, 1);
20日线向上 := MA20 > REF(MA20, 1);
60日线向上 := MA60 > REF(MA60, 1);

// --- 价格位置 ---
近20日低位 := CLOSE <= 近20日最低 * 1.05;
近20日底部区域 := CLOSE <= 近20日最低 + (近20日最高 - 近20日最低) * 0.2;
近60日低位 := CLOSE <= 近60日最低 * 1.1;
近60日底部区域 := CLOSE <= 近60日最低 + (近60日最高 - 近60日最低) * 0.3;

// --- 涨跌状态 ---
今日上涨 := CLOSE > REF(CLOSE, 1);
今日下跌 := CLOSE < REF(CLOSE, 1);
今日收阳 := CLOSE > OPEN;
今日收阴 := CLOSE < OPEN;
今日十字星 := ABS(CLOSE - OPEN) <= (HIGH - LOW) * 0.1;

连续上涨 := BARSLASTCOUNT(CLOSE > REF(CLOSE, 1)) >= 2;
连续下跌 := BARSLASTCOUNT(CLOSE < REF(CLOSE, 1)) >= 2;

// ================== 第六部分：选股方案 ==================

// --- 原严格方案（方案1-5）---

// 方案1：仅共振买入信号
选股条件1 := MACD金叉 AND KDJ金叉 AND J1 < 30 AND VOL > VOL5 * 1.2;

// 方案2：共振买入 + 底背离预警
选股条件2 := 选股条件1 OR (价格新低 AND J值抬高 AND J1 < 40);

// 方案3：强势区回调后的买入机会
选股条件3 := MACD水上 AND DIF > DEA AND J1 < 60 AND CROSS(J1, D1) AND VOL > VOL5;

// 方案4：底背离后的金叉确认
选股条件4 := KDJ底背离 AND J1 < 40 AND MACD金叉 AND KDJ金叉;

// 方案5：综合最强买入信号
选股条件5 := (选股条件1 OR (KDJ底背离 AND CROSS(J1, D1))) 
             AND DIF > -0.5 
             AND CLOSE > MA20;

// --- 新增宽松方案（方案6-15）---

// 方案6：温和启动型
选股条件6 := J线向上 
             AND J1 < 70 
             AND (MACD走强 OR MACD金叉 OR DIF > -0.3)
             AND VOL > REF(VOL, 1) * 0.8
             AND CLOSE > 近5日最低 * 0.95;

// 方案7：超跌反弹型
选股条件7 := J1 < 45 
             AND (今日上涨 OR 今日收阳)
             AND VOL > REF(VOL, 1) * 0.9
             AND (DIF > REF(DIF, 1) OR DIF > DEA);

// 方案8：KDJ金叉优先型
选股条件8 := KDJ金叉 
             AND J1 < 75
             AND VOL > REF(VOL, 1) * 0.85
             AND (DIF > DEA OR DIF > -0.2);

// 方案9：MACD金叉优先型
选股条件9 := MACD金叉 
             AND J1 < 70
             AND VOL > REF(VOL, 1) * 0.9
             AND (CLOSE > MA5 OR 今日收阳);

// 方案10：低位布局型
选股条件10 := CLOSE < MA20 * 1.05 
              AND J1 < 60 
              AND J线向上
              AND VOL > REF(VOL, 1) * 0.8
              AND (K1 > D1 OR KDJ金叉);

// 方案11：底背离宽松版
选股条件11 := 价格新低 
              AND J值抬高 
              AND J1 < 55
              AND VOL > REF(VOL, 1)
              AND 今日上涨;

// 方案12：综合宽松型
选股条件12 := (J线向上 OR KDJ金叉)
              AND J1 < 80
              AND VOL > REF(VOL, 1) * 0.7
              AND CLOSE > 近10日最低 * 0.9
              AND (DIF > DEA OR J1 < 60 OR MACD金叉);

// 方案13：简单实用型
选股条件13 := J1 < 75 
              AND (J线向上 OR KDJ金叉)
              AND VOL > REF(VOL, 1) * 0.75
              AND (今日上涨 OR 今日收阳)
              AND (DIF > DEA OR J1 < 55);

// 方案14：超简版
选股条件14 := J1 < 70 
              AND J线向上
              AND VOL > REF(VOL, 1) * 0.7
              AND 今日上涨;

// 方案15：保证有票版
选股条件15 := CLOSE > REF(CLOSE, 1) * 0.95
              AND VOL > 1000
              AND J1 < 85
              AND (DIF > DEA OR J1 < 65 OR KDJ金叉);

// ================== 第七部分：输出选股条件 ==================

// 默认使用方案5（原综合最强买入信号）
// 您可以自由切换以下任意方案

选股条件: 选股条件5;

/* 
使用说明：

方案1-5（严格版，适合强势市场）：
1: 共振买入 - MACD金叉+KDJ金叉+低位+放量
2: 共振+底背离 - 双重确认
3: 强势回调 - 水上强势区回调金叉
4: 背离金叉 - 底背离后双金叉
5: 综合最强 - 以上条件组合+均线过滤

方案6-10（温和版，适合震荡市场）：
6: 温和启动 - J线向上+MACD不差+放量
7: 超跌反弹 - J值超跌+止跌+放量
8: KDJ优先 - KDJ金叉主导
9: MACD优先 - MACD金叉主导
10: 低位布局 - 价格低位+KDJ走好

方案11-15（宽松版，适合弱势市场）：
11: 底背离宽松 - 简化版底背离
12: 综合宽松 - 多条件组合宽松版
13: 简单实用 - 核心条件精简
14: 超简版 - 极简条件
15: 保证有票 - 最宽松，几乎每天有票

切换方法：
将最后一行的"选股条件5"改为对应的方案编号即可
例如：选股条件: 选股条件15;  // 使用保证有票版
*/