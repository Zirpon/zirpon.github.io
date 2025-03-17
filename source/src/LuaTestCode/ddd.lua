local usertable = {}

local meta = {}
meta.n = 0
meta.__newindex = function(t, k, v)
    -- print("__newindex", k, v)
    if k == "n" then return nil end
    meta.n = meta.n + 1
    rawset(t, k, v)
end

meta.__index = function(t, k, v)
    print("__index", k, v)
    if k == "n" then return meta.n end
    if not v then
        print(k)
        meta.n = (meta.n <= 1 and 0) or (meta.n - 1)
    else
        return rawset(t, k, v) -- t[k]
    end
end

setmetatable(usertable, meta)
usertable.name = "kawaii"
--print(usertable.n)
--print(#usertable)
--for i, v in pairs(usertable) do print(i, v) end
usertable.name = 'wasabi'
print(usertable.n)
usertable.n = 90
print(usertable.n)
