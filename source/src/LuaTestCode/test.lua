local x = {12, 123, 44}
-- {'y', 'x', 'z'}
-- {[3]='x', [2]='y', [6]='z'}
for k in pairs(_G) do print(k) end

local t = _G -- start with the table of globals

t.mm = t.mm or {} -- create table if absent
t = t.mm

if not _G.mm then
    print 'not exist'
else
    print 'exist'
end

for k in pairs(_G) do print(k) end

