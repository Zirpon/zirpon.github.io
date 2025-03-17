s = [[then he said: "it's all right"!]]
a, b, c, quotedPart = string.find(s, "([\"'])(.-)%1")
print(quotedPart) --> it's all right
print(a,b,c) --> "

function f(...)
    a = select(3,...)  -->从第三个位置开始，变量 a 对应右边变量列表的第一个参数
    print (a)
    print (select(3,...)) -->打印所有列表参数
end


f(0,1,2,3,4,5)

local kb = math.floor(collectgarbage("count"))
local t = {}
for i = 1, 100000 do
    t[-i] = i
end
kb = math.floor(collectgarbage("count")) - kb
print(string.format("%sKB", kb))