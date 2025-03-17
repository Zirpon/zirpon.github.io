local tbcmt = { 
    __close = function() 
        print("close to-be-closed var")
    end
}

local function create_tbcv()
    local tbcv = {}
    setmetatable(tbcv, tbcmt)
    return tbcv
end
    
do
    local tbcv <close> = create_tbcv()
    print("not yet")
end
