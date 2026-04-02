import tushare as ts
import pandas as pd
import numpy as np
from datetime import datetime, timedelta

# 你的Tushare Token（请替换为你自己的token）
TUSHARE_TOKEN = "94b73042c6f48672fb8bb18cd35f793009c2d43c6666485d00f92c6f"
ts.set_token(TUSHARE_TOKEN)
pro = ts.pro_api()
from tushare.stock import trading
org = trading

def test_scan():
    """测试扫描少量股票"""
    
    # 测试股票列表（不同板块的代表）
    test_stocks = [
        {'ts_code': '000001.SZ', 'name': '平安银行', 'symbol': '000001'},
        {'ts_code': '000858.SZ', 'name': '五粮液', 'symbol': '000858'},
        {'ts_code': '600519.SH', 'name': '贵州茅台', 'symbol': '600519'},
        {'ts_code': '300750.SZ', 'name': '宁德时代', 'symbol': '300750'},
        {'ts_code': '002415.SZ', 'name': '海康威视', 'symbol': '002415'},
    ]
    
    target_date = '2025-03-14'
    trade_date = target_date.replace('-', '')
    start_date = (pd.to_datetime(target_date) - timedelta(days=60)).strftime('%Y%m%d')
    
    results = []
    
    print(f"测试扫描 {target_date}")
    print("-" * 40)
    
    for stock in test_stocks:
        try:
            from tushare.stock.rtq import realtime_quote

            # 获取日线数据
            df = realtime_quote(ts_code=stock['ts_code'], 
                          start_date=start_date,
                          end_date=trade_date, src="sina")
            
            if df is None or len(df) < 35:
                print(f"✗ {stock['symbol']} {stock['name']} - 数据不足")
                continue
            
            df = df.sort_values('trade_date')
            
            # 计算VAR5
            df['avg_price'] = (df['open'] + df['close'] + df['high'] + df['low']) / 4
            df['VAR1'] = df['avg_price'].shift(1)
            
            low_var1_diff = abs(df['low'] - df['VAR1'])
            max_var1 = np.maximum(df['low'] - df['VAR1'], 0)
            
            df['VAR2'] = low_var1_diff.rolling(13).mean() / max_var1.rolling(10).mean()
            df['VAR3'] = df['VAR2'].ewm(span=10).mean()
            df['VAR4'] = df['low'].rolling(33).min()
            df['VAR5'] = np.where(df['low'] <= df['VAR4'], df['VAR3'], 0)
            df['VAR5'] = df['VAR5'].ewm(span=3).mean()
            
            # 获取信号
            current_var5 = df['VAR5'].iloc[-1]
            prev_var5 = df['VAR5'].iloc[-2]
            signal = current_var5 > prev_var5 and current_var5 > 0
            
            # 获取资金流
            money_flow = pro.moneyflow(ts_code=stock['ts_code'], trade_date=trade_date)
            main_net = 0
            if money_flow is not None and len(money_flow) > 0:
                main_net = money_flow.iloc[0].get('net_main_amount', 0)
            
            # 获取当日数据
            today = df[df['trade_date'] == trade_date].iloc[0]
            prev_close = df[df['trade_date'] < trade_date].iloc[-1]['close']
            increase = (today['close'] - prev_close) / prev_close * 100
            
            status = "✓" if signal else " "
            print(f"{status} {stock['symbol']} {stock['name']:10} "
                  f"VAR5: {current_var5:.3f} 信号: {'是' if signal else '否'} "
                  f"涨幅: {increase:.1f}% 主力净额: {main_net/10000:.0f}万")
            
            if signal:
                results.append({
                    '代码': stock['symbol'],
                    '名称': stock['name'],
                    '信号': '是',
                    'VAR5': round(current_var5, 3),
                    '涨幅': round(increase, 2),
                    '主力净额(万)': round(main_net/10000, 2)
                })
                
        except Exception as e:
            print(f"✗ {stock['symbol']} {stock['name']} - 错误: {e}")
    
    print(f"\n符合条件的股票: {len(results)} 只")
    if results:
        print(pd.DataFrame(results))

if __name__ == "__main__":
    test_scan()