---
--- Created by Administrator.
--- DateTime: 2018/3/1 0001 14:20
---
local bit = require "bit"
local socket = require "socket"

--------------------------------------------------------------------------------------
local def_actionType = {
    NULL   = 0x00,
    Left   = 0x01, --tonumber("0x01",16),
    Center = 0x02, --tonumber("0x02",16),
    Right  = 0x04, --tonumber("0x04",16),
    Peng   = 0x08, --tonumber("0x08",16),
    Gang   = 0x10, --tonumber("0x10",16),
    Bu     = 0x20, --tonumber("0x20",16),
    Chihu  = 0x40, --tonumber("0x40",16),
    Listen = 0x80, --tonumber("0x80",16),
}

local def_cardColor = {
    wan  = 0,
    tiao = 1,
    tong = 2,
    feng = 3,
    zi   = 4,
}

local def_kindItemUseUpJingCardIndex = {
    NULL = 0,
    Left = 1,
    Center = 2,
    Right  = 3,
    Both   = 4,
}

local def_chr = {
    NULL            = 0x00000000,
    PingHu          = 0x00000001,
    QiXiaoDui       = 0x00000002,
    QiDaDui         = 0x00000004,
    DaHu            = 0x00000010,
    HunYiSe         = 0x00000020,
    QingYiSe        = 0x00000040,
    FourBao         = 0x00000080,
    DiHu            = 0x00000100,
    TianHu          = 0x00000200,
    DeGuo           = 0x00000400,
    ZiYiSe          = 0x00000800,
    QiangGang       = 0x00001000,
    GangShangHua    = 0x00002000,
    JingDiao        = 0x00004000,
    GangShangPao    = 0x00008000,
    QuanQiuRen      = 0x00010000,
    HaoHuaQiXiaoDui = 0x00020000,
}

local def_ShiChengMJMode = {
    NULL      = 0,
    FangHu    = 1,
    HongZhong = 2,
    XuanBao   = 3,
}

local def_maxIndex = 34
local def_maxCount = 14
local def_maskcolor = 0xF0
local def_maskvalue = 0x0F

--------------------------------------------------------------------------------------
local def_upJingCard = 0x28
local def_bReduceUpJingCardCountSwitch = false
local def_outUpJingCard = 0
local def_currentCard = 0x11
local def_chiHuRight = 0
local def_tCardData = {0x12,0x13,0x24,0x25,0x26,0x28,0x28}
local def_tWeaveItems = {
    {
        weaveKind = def_actionType.Peng,
        centerCard = 0x07,
        publicCard = true,
        provideUser = 01,
    },
    {
        weaveKind = def_actionType.Left,
        centerCard = 0x02,
        publicCard = true,
        provideUser = 01,
    },
}

--------------------------------------------------------------------------------------
local function clone(object)
    local lookup_table = {}
    local function _copy(object)
        if type(object) ~= "table" then
            return object
        elseif lookup_table[object] then
            return lookup_table[object]
        end
        local new_table = {}
        lookup_table[object] = new_table
        for key, value in pairs(object) do
            new_table[_copy(key)] = _copy(value)
        end
        return setmetatable(new_table, getmetatable(object))
    end
    return _copy(object)
end
--------------------------------------------------------------------------------------
local function IsValidCard(cardData)
    local value = bit.band(cardData, def_maskvalue)
    local color = bit.rshift(bit.band(cardData, def_maskcolor), 4)

    if (color <= def_cardColor.tong and value >= 1 and value <= 9)
    or (color == def_cardColor.feng and value >= 1 and value <= 4)
    or (color == def_cardColor.zi and value >= 1 and value <= 3) then
        return true
    end

    return false
end

local function switchToCardIndex(cardData)
    if not IsValidCard(cardData) then return nil end

    local value = bit.band(cardData, def_maskvalue)
    local color = bit.rshift(bit.band(cardData, def_maskcolor), 4)

    if color <= def_cardColor.feng then
        return color * 9 + value
    elseif color == def_cardColor.zi then
        return (color - 1) * 9 + 4 + value
    end

    return nil
end

local function switchToCardData(cardIndex)
    if cardIndex >= 28 and cardIndex <= 31 then
        return bit.bor(48, cardIndex % 9) -- 3 << 4 = 48
    elseif cardIndex >= 32 and cardIndex <= 34 then
        return bit.bor(64,cardIndex - 31)  -- 4 << 4 = 64
    elseif cardIndex >= 1 and cardIndex <= 27 then
        return bit.bor(bit.lshift(math.floor((cardIndex-1) / 9), 4), (cardIndex - 1) % 9 + 1)
    end

    return nil
end

local function t_switchToCardData(tCardIndex)
    if type(tCardIndex) ~= "table" then
        return nil
    end

    local tCardData = {}
    local position = 0
    for i, v in pairs(tCardIndex) do
        if v > 0 then
            for j = 1, v do
                if #tCardData >= def_maxCount then
                    return nil
                end

                tCardData[position] = switchToCardData(i)
                position = position + 1
            end
        end
    end

    return tCardData
end

local function t_swithToCardIndex(tCardData)
    local tCardIndex = {}
    for i, v in ipairs(tCardData) do
        local cardIndex = switchToCardIndex(v)
        if not tCardIndex[cardIndex] then
            tCardIndex[cardIndex] = 1
        else
            tCardIndex[cardIndex] = tCardIndex[cardIndex] + 1
        end
    end

    return tCardIndex
end

local function IsUpJing(cardData)
    return (cardData == def_upJingCard)
end

local function HaveUpJingInHand(tCardIndex)
    local tmpUpJingCardIndex = switchToCardIndex(def_upJingCard)

    if not tmpUpJingCardIndex then
        return false
    end

    local reduceTime = 0
    if true == def_bReduceUpJingCardCountSwitch and 0 ~= def_outUpJingCard then
        reduceTime = reduceTime + 1
    end

    if tCardIndex[tmpUpJingCardIndex] then
        return (tCardIndex[tmpUpJingCardIndex] - reduceTime) > 0
    end

    return false
end

local function GetUpJingCardDataFromHand(tCardIndex)
    local upJingCardList = {}
    local reduceTime = 1
    local outUpJingCardIndex = nil
    if 0 ~= def_outUpJingCard and true == def_bReduceUpJingCardCountSwitch then
        outUpJingCardIndex = switchToCardIndex(def_outUpJingCard)
    end

    local tmpCardIndex = clone(tCardIndex)
    for i, v in pairs(tmpCardIndex) do
        if outUpJingCardIndex and outUpJingCardIndex == i and v > 0 then
            tmpCardIndex[i] = v - 1
        end

        local tmpCardIndex_cardData = switchToCardData(i)
        if tmpCardIndex[i] > 0 and IsUpJing(tmpCardIndex_cardData) then
            for cardData_count = 1, tmpCardIndex[i] do
                table.insert(upJingCardList, tmpCardIndex_cardData)
            end
        end
    end
    
    return upJingCardList
end

local function IsQiXiaoDui(tCardIndex, weaveItems)
    if #weaveItems > 0 then
        return 0
    end

    local singularCardIndex = {}
    local singularCardICount = 0
    local bIsXiaoQiDui = true
    for i, v in pairs(tCardIndex) do
        if v > 0 and v ~= 2 and v ~= 4 then
            singularCardIndex[i] = v
            if not IsUpJing(switchToCardData(i)) then
                singularCardICount = singularCardICount + 1
            end

            bIsXiaoQiDui = false
        end
    end

    if true == bIsXiaoQiDui then
        return 2
    end

    local tmpUpJingCardList = GetUpJingCardDataFromHand(tCardIndex)
    if #tmpUpJingCardList >= singularCardICount then
        return 1
    else
        return 0
    end
end

local function IsQiDaDui(tCardIndex, weaveItems)
    local ret = def_chr.NULL
    for weaveIndex, weaveValue in ipairs(weaveItems) do
        if weaveValue.weaveKind > 0 and (def_actionType.Gang ~= weaveValue.weaveKind and def_actionType.Peng ~= weaveValue.weaveKind  and def_actionType.Bu ~= weaveValue.weaveKind ) then
            return ret
        end
    end

    local pair = 0
    local bIsDaQiDuiHu = true
    local bIsUpJingCenterCard = IsUpJing(centerCard)
    local cbCenterCardIndex = switchToCardIndex(centerCard)
    local singularCardIndex = {}
    local pairCardIndex = {}
    local noTripleCardIndexList = {}

    for i, v in pairs(tCardIndex) do
        local cardData = switchToCardData(i)
        local bIsUpJingFlag = IsUpJing(cardData)
        if 2 == v then
            if not bIsUpJingFlag then
                pairCardIndex = pairCardIndex + 1
                noTripleCardIndexList[i] = v
            end
            pair = pair + 1
        elseif 4 == v or 1 == v then
            if not bIsUpJingFlag then
                singularCardIndex = singularCardIndex + 1
                noTripleCardIndexList[i] = v
            end
            bIsDaQiDuiHu = false
        end
    end

    local pairCardCount = #pairCardIndex
    local singularCardCount = #singularCardIndex
    local tmpUpJingCardList = GetUpJingCardDataFromHand(tCardIndex)
    local upJingCardCount = #tmpUpJingCardList
    if 1 == pair and true == bIsDaQiDuiHu then
        ret = bit.bor(def_chr.QiDaDui, def_chr.DeGuo)
        if 0 == pairCardCountthen then
            ret = bit.bor(ret, def_chr.JingDiao)
        elseif 1 == pairCardCount then
            if upJingCardCount >= 2 then
                ret = bit.bor(ret, def_chr.JingDiao)
            end
        end
        return ret
    end

    if pairCardCount > 0 and 0 == singularCardCount then
        local upJingCardNeed = (pairCardCount-1)*1
        if upJingCardNeed == upJingCardCount then
            if 0 ~= def_outUpJingCard then
                if not bIsUpJingCenterCard then
                    ret = bit.bor(def_chr.QiDaDui, def_chr.JingDiao)
                end
            else
                ret = def_chr.QiDaDui
            end
            return ret
        elseif upJingCardNeed < upJingCardCount then
            ret = bit.bor(def_chr.QiDaDui, def_chr.JingDiao)
            return ret
        end
    elseif 0 == pairCardCount and singularCardCount > 0 then
        local upJingCardNeed = (singularCardCount-1)*2 + 1
        if upJingCardNeed < upJingCardCount then
            ret = bot.bor(def_chr.QiDaDui, def_chr.JingDiao)
            return ret
        elseif upJingCardNeed == upJingCardCount then
            if 0 ~= def_outUpJingCard then
                if false == bIsUpJingCenterCard then
                    ret = def_chr.QiDaDui
                    if tCardIndex[cbCenterCardIndex] and (1 == tCardIndex[cbCenterCardIndex] or 4 == tCardIndex[cbCenterCardIndex]) then
                        ret = bit.bor(ret, def_chr.JingDiao)
                    end
                    return ret
                end
            else
                ret = def_chr.QiDaDui
                if false == bIsUpJingCenterCard then
                    if tCardIndex[cbCenterCardIndex] and (1 == tCardIndex[cbCenterCardIndex] or 4 == tCardIndex[cbCenterCardIndex]) then
                        ret = bit.bor(ret, def_chr.JingDiao)
                    end
                end
                return ret
            end
        end
    elseif pairCardCount > 0 and singularCardCount > 0 then
        local upJingCardNeed = singularCardCount*2+(pairCardCount-1)*1
        if upJingCardNeed == upJingCardCount then
            if 0 ~= def_outUpJingCard then
                if not bIsUpJingCenterCard then
                    ret = def_chr.QiDaDui
                    if tCardIndex[cbCenterCardIndex] and (1 == tCardIndex[cbCenterCardIndex] or 4 == tCardIndex[cbCenterCardIndex]) then
                        ret = bit.bor(ret, def_chr.JingDiao)
                    end
                    return ret
                end
            else
                ret = def_chr.QiDaDui
                if false == bIsUpJingCenterCard and (tCardIndex[cbCenterCardIndex] and (1 == tCardIndex[cbCenterCardIndex] or 4 == tCardIndex[cbCenterCardIndex])) then
                    ret = bit.bor(ret, def_chr.JingDiao)
                end
                return ret
            end
        elseif upJingCardNeed < upJingCardCount then
            ret = bit.bor(def_chr.QiDaDui, def_chr.JingDiao)
            return ret
        end
    elseif 0 == pairCardCount and 0 == singularCardCount then
        if bIsUpJingCenterCard and upJingCardCount >= 1 then
            ret = bit.bor(def_chr.QiDaDui, def_chr.JingDiao)
            return ret
        elseif false == bIsUpJingCenterCard and upJingCardCount >= 2 then
            ret = bit.bor(def_chr.QiDaDui, def_chr.JingDiao)
            return ret
        end
    end

    return def_chr.NULL
end

local function IsZiYiSe(tCardIndex, weaveItems, chiHuRight)
    local tmpChihuRight = bit.band(chiHuRight, bit.bor(def_chr.QiDaDui, def_chr.QiXiaoDui))
    if def_chr.NULL == tmpChihuRight then
        return 0
    end

    local bIsDeguo = true
    for i, v in pairs(tCardIndex) do
        if v > 0 and i < 28 then
            local cardData = switchToCardData(i)
            local bIsUpJing = IsUpJing(cardData)
            if bIsUpJing then
                bIsDeguo = false
            else
                return 0
            end
        end
    end

    for i, v in ipairs(weaveItems) do
        local centerCard = v.centerCard
        local centerCardColor = bit.rshift(bit.band(centerCard, def_maskcolor), 4)
        if centerCardColor <= 2 then
            return 0
        end
    end

    if false == bIsDeguo then
        return 1
    end

    return 2
end

local function IsQingYiSe(tCardIndex, weaveItems)
    local cardColor = nil
    for i, v in pairs(tCardIndex) do
        if v > 0 then
            local cardData = switchToCardData(i)
            local bIsUpJing = IsUpJing(cardData)

            local curCardColor = bit.rshift(bit.band(cardData, def_maskcolor), 4)
            if curCardColor <= 2 then
                if nil == cardColor then
                    if false == bIsUpJing then
                        cardColor = curCardColor
                    end
                else
                    if cardColor ~= curCardColor then
                        if false == bIsUpJing then
                            return 0
                        else
                            --continue
                        end
                    else
                        --continue
                    end
                end
            else
                return 0
            end
        end
    end

    for i, v in ipairs(weaveItems) do
        local centerCard = v.centerCard
        local centerCardColor = bit.rshift(bit.band(centerCard, def_maskcolor), 4)
        if centerCardColor ~=  cardColor then
            return 0
        end
    end

    local bHaveUpjingCard = HaveUpJingInHand(tCardIndex)
    if true == bHaveUpjingCard then
        local baoCardColor = bit.rshift(bit.band(def_upJingCard, def_maskcolor), 4)
        if baoCardColor ~= cardColor then
            return 1
        end
    end

    return 2
end

local function IsHunYiSe(tCardIndex, weaveItems)
    local cardColor = nil
    for i, v in pairs(tCardIndex) do
        if v > 0 then
            local cardData = switchToCardData(i)
            local bIsUpJing = IsUpJing(cardData)
            local curCardColor = bit.rshift(bit.band(cardData, def_maskcolor), 4)
            if curCardColor <= 2 then
                if nil == cardColor then
                    if false == bIsUpJing then
                        cardColor = curCardColor
                    end
                else
                    if cardColor ~= curCardColor then
                        if false == bIsUpJing then
                            return 0
                        else
                            --continue
                        end
                    else
                        --continue
                    end
                end
            else
                return 0
            end
        end
    end
    for i, v in ipairs(weaveItems) do
        local centerCard = v.centerCard
        local centerCardColor = bit.rshift(bit.band(centerCard, def_maskcolor), 4)
        if centerCardColor ~=  cardColor and centerCardColor <= 2 then
            return 0
        end
    end
    local tmpUpJingCardList = GetUpJingCardDataFromHand(tCardIndex)
    if #tmpUpJingCardList then
        local baoCardColor = bit.rshift(bit.band(def_upJingCard, def_maskcolor), 4)
        if baoCardColor <= 2 and baoCardColor~= cardColor then
            return 1
        end
    end

    return 2
end

local function IsFourBao(tCardIndex, weaveItems)
    if 0 == def_upJingCard then
        return false
    end

    local tmpUpJingCardIndex =switchToCardIndex(def_upJingCard)
    if not tmpUpJingCardIndex then
        return false
    end

    local tmpUpJingCardList = GetUpJingCardDataFromHand(tCardIndex)
    if 4 == #tmpUpJingCardList then
        return true
    end

    return false
end

local function AnalyseCard(tInputCardIndex, tWeaveItems)
    local tCardIndex = clone(tInputCardIndex)
    local cardCount = 0
    for i, v in pairs(tCardIndex) do cardCount = cardCount + v end

    local tOutAnalyseItem = {}
    if cardCount < 2 or cardCount > def_maxCount or (((cardCount - 2)%3) ~= 0) then
        return tOutAnalyseItem
    end

    --local kindItemCount = 0
    local tKindItem = {}
    local tmpUpJingCardIndex = switchToCardIndex(def_upJingCard)
    local lessKindItem = math.floor((cardCount - 2) / 3)
    local originalUpJingCardList = GetUpJingCardDataFromHand(tCardIndex)
    local tmpUpJingCardCount = #originalUpJingCardList
    
    if 0 == lessKindItem then
        local bHaveUpJingInHand = false
        if tmpUpJingCardCount > 0 then
            bHaveUpJingInHand = true
        end
        local analyseItemSingle = {}
        analyseItemSingle.cardEyes = {}
        local cardEyesIndex = 0
        
        for icard, vcard in pairs(tCardIndex) do
            if 2 == vcard then
                local analyseItem = {}
                analyseItem.weaveItems = {}
                analyseItem.weaveItems = clone(tWeaveItems)
                for weaveIndex, weaveValue in ipairs(analyseItem.weaveItems) do
                    weaveValue.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                end

                analyseItem.cardEyes = {}
                analyseItem.cardEyes[1] = switchToCardData(icard)
                analyseItem.cardEyes[2] = switchToCardData(icard)
                analyseItem.bIsUpJingCardChange = false
                table.insert(tOutAnalyseItem, analyseItem)
                return tOutAnalyseItem
            elseif true == bHaveUpJingInHand then
                analyseItemSingle.cardEyes[cardEyesIndex] = switchToCardData(icard)
                cardEyesIndex = cardEyesIndex + 1
                
                if cardEyesIndex > 1 then
                    analyseItemSingle.weaveItems = {}
                    analyseItemSingle.weaveItems = clone(tWeaveItems)
                    for weaveIndex, weaveValue in ipairs(analyseItemSingle.weaveItems) do
                        weaveValue.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                    end
                    analyseItemSingle.bIsUpJingCardChange = true
                    table.insert(tOutAnalyseItem, analyseItemSingle)
                    return tOutAnalyseItem
                end
            end
        end

        return tOutAnalyseItem
    end

    if cardCount > 3 then
        for icard, vcard in pairs(tCardIndex) do
            local centerCardData = switchToCardData(icard)
            local bIsUpJingCardFlag = IsUpJing(centerCardData)

            if tCardIndex[icard] >= 3 then
                local tmpKindItem = {}
                tmpKindItem.cardIndex = {}
                tmpKindItem.cardIndex[1] = icard
                tmpKindItem.cardIndex[2] = icard
                tmpKindItem.cardIndex[3] = icard
                tmpKindItem.weaveKind = def_actionType.Peng
                tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                tmpKindItem.centerCard = switchToCardData(icard)
                table.insert(tKindItem, tmpKindItem)
            end

            if icard < 28 and vcard > 0 and (((icard-1)%9) < 7) then
                local iCostCount = 0
                local iSurplusCount = 0
                for vcardCount = 1, vcard do
                    if tCardIndex[icard+1] and tCardIndex[icard+1] >= vcardCount and tCardIndex[icard+2] and tCardIndex[icard+2] >= vcardCount then
                        local tmpKindItem = {}
                        tmpKindItem.cardIndex = {}
                        tmpKindItem.cardIndex[1] = icard
                        tmpKindItem.cardIndex[2] = icard+1
                        tmpKindItem.cardIndex[3] = icard+2
                        tmpKindItem.weaveKind = def_actionType.Left
                        tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                        tmpKindItem.centerCard = switchToCardData(icard)
                        table.insert(tKindItem, tmpKindItem)
                        iCostCount = iCostCount + 1
                    else
                        break
                    end
                end

                iSurplusCount = vcard - iCostCount
                if tmpUpJingCardCount > 0 and false == bIsUpJingCardFlag then
                    if iSurplusCount > 0 then
                        if tCardIndex[icard+2] and (tCardIndex[icard+2] > iCostCount or vcard >= 2) then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = icard
                            tmpKindItem.cardIndex[2] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[3] = icard+2
                            tmpKindItem.weaveKind = def_actionType.Left
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Center
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                        if tCardIndex[icard+1] and (tCardIndex[icard+1] > iCostCount or vcard >= 2) then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = icard
                            tmpKindItem.cardIndex[2] = icard+1
                            tmpKindItem.cardIndex[3] = tmpUpJingCardIndex
                            tmpKindItem.weaveKind = def_actionType.Left
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Right
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                    elseif 0 == iSurplusCount then
                        if tCardIndex[icard+1] and tCardIndex[icard+1] >= 2 and tCardIndex[icard+2] and tCardIndex[icard+2] > 0 then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = icard
                            tmpKindItem.cardIndex[2] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[3] = icard+2
                            tmpKindItem.weaveKind = def_actionType.Left
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Center
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                        if tCardIndex[icard+2] and tCardIndex[icard+2] >= 2 and tCardIndex[icard+1] and tCardIndex[icard+1] > 0 then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = icard
                            tmpKindItem.cardIndex[2] = icard+1
                            tmpKindItem.cardIndex[3] = tmpUpJingCardIndex
                            tmpKindItem.weaveKind = def_actionType.Left
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Right
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                    end
                    if tmpUpJingCardCount > 1 and ((tCardIndex[icard+1] and tCardIndex[icard+1] >= 2) or ((tCardIndex[icard+2] and tCardIndex[icard+2] >= 2)) or (not tCardIndex[icard+1] and not tCardIndex[icard+2])) then
                        local tmpKindItem = {}
                        tmpKindItem.cardIndex = {}
                        tmpKindItem.cardIndex[1] = icard
                        tmpKindItem.cardIndex[2] = tmpUpJingCardIndex
                        tmpKindItem.cardIndex[3] = tmpUpJingCardIndex
                        tmpKindItem.weaveKind = def_actionType.Peng
                        tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Both
                        tmpKindItem.centerCard = switchToCardData(icard)
                        table.insert(tKindItem, tmpKindItem)
                    end
                end
            elseif icard < 28 and vcard > 0 and (((icard-1)%9) >= 7) then
                if tmpUpJingCardCount > 0 and false == bIsUpJingCardFlag then
                    if 7 == ((icard-1)%9) then
                        if tCardIndex[icard+1] and tCardIndex[icard+1] > 0 then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[2] = icard
                            tmpKindItem.cardIndex[3] = icard+1
                            tmpKindItem.weaveKind = def_actionType.Center
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Left
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                        if tmpUpJingCardCount > 1 then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[2] = icard
                            tmpKindItem.cardIndex[3] = tmpUpJingCardIndex
                            tmpKindItem.weaveKind = def_actionType.Peng
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Both
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                    elseif 8 == ((icard-1)%9) then
                        if (tCardIndex[icard] and (1 == tCardIndex[icard] or 4 == tCardIndex[icard])) and tmpUpJingCardCount > 1 then
                            local tmpKindItem = {}
                            tmpKindItem.cardIndex = {}
                            tmpKindItem.cardIndex[1] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[2] = tmpUpJingCardIndex
                            tmpKindItem.cardIndex[3] = icard
                            tmpKindItem.weaveKind = def_actionType.Peng
                            tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Both
                            tmpKindItem.centerCard = switchToCardData(icard)
                            table.insert(tKindItem, tmpKindItem)
                        end
                    end                    
                end
            elseif icard >= 28 and icard <= 33 and vcard > 0 then
                if tmpUpJingCardCount > 1 and false == bIsUpJingCardFlag and (1 == vcard or 4 == vcard) then
                    local tmpKindItem = {}
                    tmpKindItem.cardIndex = {}
                    tmpKindItem.cardIndex[1] = tmpUpJingCardIndex
                    tmpKindItem.cardIndex[2] = tmpUpJingCardIndex
                    tmpKindItem.cardIndex[3] = icard
                    tmpKindItem.weaveKind = def_actionType.Peng
                    tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Both
                    tmpKindItem.centerCard = switchToCardData(icard)
                    table.insert(tKindItem, tmpKindItem)
                end
            end

            if 2 == vcard and tmpUpJingCardCount > 0 and false == bIsUpJingCardFlag then
                local tmpKindItem = {}
                tmpKindItem.cardIndex = {}
                tmpKindItem.cardIndex[1] = icard
                tmpKindItem.cardIndex[2] = icard
                tmpKindItem.cardIndex[3] = tmpUpJingCardIndex
                tmpKindItem.weaveKind = def_actionType.Peng
                if icard ~= tmpUpJingCardIndex then
                    tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.Right
                else
                    tmpKindItem.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                end
                tmpKindItem.centerCard = switchToCardData(icard)
                table.insert(tKindItem, tmpKindItem)
            end
        end

    end

    local kindItemCount = #tKindItem
    if kindItemCount >= lessKindItem then
        local index = {1,2,3,4}
        local tmpKindItemCombie = {}
        while true do
            local tmp_tCardIndex = clone(tCardIndex)
            local tmpUpJingCardList = clone(originalUpJingCardList)
            local tmpUpJingCardListCount = #tmpUpJingCardList

            local bIsUpJingCardChange = false
            
            local iUpJingCardChangeNum = 0
            local onUseDeguoCardIndex = {}
            for i = 1, def_maxIndex do onUseDeguoCardIndex[i] = 0 end
            
            for i = 1, lessKindItem do
                tmpKindItemCombie[i] = tKindItem[index[i]]
                local cbKindItemCenterCard = tmpKindItemCombie[i].centerCard
                local cbKindItemCenterCardIndex = switchToCardIndex(cbKindItemCenterCard)

                local tmpKindItemWeaveKind = tmpKindItemCombie[i].weaveKind
                if tmpKindItemWeaveKind == def_actionType.Left then
                    onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] + 1
                    if def_kindItemUseUpJingCardIndex.NULL == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Center == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Right == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                    end
                elseif tmpKindItemWeaveKind == def_actionType.Center then
                    onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                    if def_kindItemUseUpJingCardIndex.NULL == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] + 1
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Left == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Right == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                    end
                elseif tmpKindItemWeaveKind == def_actionType.Right then
                    onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[3]] + 1
                    if def_kindItemUseUpJingCardIndex.NULL == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] + 1
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Left == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[2]] + 1
                    elseif def_kindItemUseUpJingCardIndex.Center == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] = onUseDeguoCardIndex[tmpKindItemCombie[i].cardIndex[1]] + 1
                    end
                elseif tmpKindItemWeaveKind == def_actionType.Peng then
                    onUseDeguoCardIndex[cbKindItemCenterCardIndex] = onUseDeguoCardIndex[cbKindItemCenterCardIndex] + 1
                    if def_kindItemUseUpJingCardIndex.NULL == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[cbKindItemCenterCardIndex] = onUseDeguoCardIndex[cbKindItemCenterCardIndex] + 2
                    elseif def_kindItemUseUpJingCardIndex.Right == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[cbKindItemCenterCardIndex] = onUseDeguoCardIndex[cbKindItemCenterCardIndex] + 1
                    elseif def_kindItemUseUpJingCardIndex.Left == tmpKindItemCombie[i].useUpJingCardIndex then
                        onUseDeguoCardIndex[cbKindItemCenterCardIndex] = onUseDeguoCardIndex[cbKindItemCenterCardIndex] + 1
                    end
                end

                if tmpKindItemCombie[i].useUpJingCardIndex > 0 and tmpKindItemCombie[i].useUpJingCardIndex < def_kindItemUseUpJingCardIndex.Both then
                    iUpJingCardChangeNum = iUpJingCardChangeNum + 1
                elseif tmpKindItemCombie[i].useUpJingCardIndex == def_kindItemUseUpJingCardIndex.Both then
                    iUpJingCardChangeNum = iUpJingCardChangeNum + 2
                end
            end

            local function useJingCardIndexSort(a,b)
                return a.useUpJingCardIndex < b.useUpJingCardIndex
            end
            table.sort(tmpKindItemCombie, useJingCardIndexSort)

            local enoughCard = true
            for i, v in pairs(onUseDeguoCardIndex) do
                if v > 0 then
                    if not tCardIndex[i] or (tCardIndex[i] and v > tCardIndex[i]) then
                        enoughCard = false
                        break
                    end
                end
            end

            if true == enoughCard then
                if tmpUpJingCardListCount < iUpJingCardChangeNum then
                    enoughCard = false
                else
                    for i = 1, (lessKindItem*3) do
                        local cardSetNum = math.floor((i-1)/3) + 1
                        local cardCardSetIndex = i - (cardSetNum-1)*3
                        local cardIndex = tmpKindItemCombie[cardSetNum].cardIndex[cardCardSetIndex]
                        local cardData = switchToCardData(cardIndex)
                        local cbUserUpJingCardIndex = tmpKindItemCombie[cardSetNum].useUpJingCardIndex

                        if cbUserUpJingCardIndex > def_kindItemUseUpJingCardIndex.NULL then
                            bIsUpJingCardChange = true
                        end

                        if cbUserUpJingCardIndex > def_kindItemUseUpJingCardIndex.NULL and IsUpJing(cardData)
                        and (cardCardSetIndex == cbUserUpJingCardIndex or cbUserUpJingCardIndex == def_kindItemUseUpJingCardIndex.Both) then
                            if #tmpUpJingCardList > 0 then
                                --tmpUpJingCardListCount = tmpUpJingCardListCount - 1
                                local cbUpJingCard = tmpUpJingCardList[1]
                                table.remove(tmpUpJingCardList, 1)
                                cardData = cbUpJingCard
                                tmpKindItemCombie[cardSetNum].cardIndex[cardCardSetIndex] = switchToCardIndex(cbUpJingCard)
                                cardIndex = tmpKindItemCombie[cardSetNum].cardIndex[cardCardSetIndex]
                            else
                                enoughCard = false
                                break
                            end
                        end
                        if not tmp_tCardIndex[cardIndex] or tmp_tCardIndex[cardIndex] <= 0 then
                            enoughCard = false
                            break
                        else
                            tmp_tCardIndex[cardIndex] = tmp_tCardIndex[cardIndex] - 1
                            if IsUpJing(cardData) and (not tmp_tCardIndex[cardIndex] or tmp_tCardIndex[cardIndex] <= 0) then
                                local leftUpJingCardCount = #tmpUpJingCardList
                                local tmpUnderRemoveUpJingCardList = {}
                                for k = 1, leftUpJingCardCount do
                                    if tmpUpJingCardList[k] == cardData then
                                        table.insert(tmpUnderRemoveUpJingCardList, k)
                                    end
                                end
                                for k = 1, #tmpUnderRemoveUpJingCardList do
                                    table.remove(tmpUpJingCardList, tmpUnderRemoveUpJingCardList[k])
                                end
                            end
                        end
                    end

                    local tmpUnderRemoveUpJingCardList = {}
                    for i, v in ipairs(tmpUpJingCardList) do
                        local upJingCardIndex = switchToCardIndex(v)
                        if not tmp_tCardIndex[upJingCardIndex] or tmp_tCardIndex[upJingCardIndex] <= 0 then
                            table.insert(tmpUnderRemoveUpJingCardList, i)
                        end
                    end
                    for i, v in ipairs(tmpUnderRemoveUpJingCardList) do
                        table.remove(tmpUpJingCardList, v)
                    end
                end
            end

            if true == enoughCard then
                local cardEyes = {}
                cardEyes[1] = 0
                cardEyes[2] = 0
                local iCardEyeIndex = 0
                for i, v in pairs(tmp_tCardIndex) do
                    local cardData = switchToCardData(i)
                    if 2 == tmp_tCardIndex[i] then
                        cardEyes[1] = cardData
                        cardEyes[2] = cardData
                        break
                    elseif #tmpUpJingCardList > 0 and tmp_tCardIndex[i] and 1 == tmp_tCardIndex[i]  then
                        cardEyes[iCardEyeIndex+1] = cardData
                        if iCardEyeIndex >= 1 then
                            if not IsUpJing(cardEyes[1]) and not IsUpJing(cardEyes[2]) then
                                cardEyes[1] = 0
                                cardEyes[2] = 0
                            else
                                if #tmpUpJingCardList >= 1 then
                                    bIsUpJingCardChange = true

                                else
                                    cardEyes[1] = 0
                                    cardEyes[2] = 0
                                end
                            end

                            break
                        end
                        iCardEyeIndex = iCardEyeIndex + 1
                    end
                end

                if 0 ~= cardEyes[1] and 0 ~= cardEyes[2] then
                    local analyseItem = {}
                    analyseItem.weaveItems = {}
                    analyseItem.weaveItems = clone(tWeaveItems)
                    for weaveIndex, weaveValue in ipairs(analyseItem.weaveItems) do
                        weaveValue.useUpJingCardIndex = def_kindItemUseUpJingCardIndex.NULL
                    end

                    for i, v in ipairs(tmpKindItemCombie) do
                        table.insert(analyseItem.weaveItems, v)
                    end

                    analyseItem.cardEyes = {}
                    analyseItem.cardEyes[1] = cardEyes[1]
                    analyseItem.cardEyes[2] = cardEyes[2]
                    analyseItem.bIsUpJingCardChange = bIsUpJingCardChange
                    table.insert(tOutAnalyseItem, analyseItem)
                end
            end

            if index[lessKindItem] == (kindItemCount) then
                local i = lessKindItem
                while i > 1 do
                    if (index[i-1]+1) ~= index[i] then
                        local cbNewIndex = index[i-1]
                        for j = (i-1), lessKindItem do
                            index[j] = cbNewIndex + j - i + 2
                        end
                        break
                    end
                    i = i - 1
                end

                if 1 == i then
                    break
                end
            else
                index[lessKindItem] = index[lessKindItem] + 1
            end
        end
    end

    return tOutAnalyseItem
end

local function AnalyseChiHuCard(tCardIndex, tWeaveItems, currentCard, chiHuRight, iShiChengMJMode)
    local outChiHuResult= {}
    outChiHuResult.chiHuRight = chiHuRight

    local cardIndexTemp = clone(tCardIndex)
    local currentCardIndex = switchToCardIndex(currentCard)
    cardIndexTemp[currentCardIndex] = (cardIndexTemp[currentCardIndex] or 0) + 1
    local analyseItemArray = AnalyseCard(cardIndexTemp, tWeaveItems)
    if analyseItemArray and #analyseItemArray > 0 then
        for arrayIndex, arrayValue in ipairs(analyseItemArray) do
            local bLianCard = false
            local bPengCard = false
            local currentCardAsPengInWeave = false

            for weaveItemsIndex, weaveItemValue in ipairs(arrayValue.weaveItems) do
                local weaveKind = weaveItemValue.weaveKind
                local centerCard = weaveItemValue.CenterCard
                local cbUseUpJingCardIndex = weaveItemValue.useUpJingCardIndex

                local tmpCalKind = bit.band(weaveKind, (bit.bor(def_actionType.Gang, def_actionType.Peng)))
                if 0 == tmpCalKind then
                    bPengCard = true
                end
                tmpCalKind = bit.band(weaveKind, (bit.bor(def_actionType.Left, def_actionType.Center, def_actionType.Right)))
                if  0 == tmpCalKind then
                    bLianCard = true
                end
                if currentCard == centerCard and def_actionType.Peng == weaveKind and def_kindItemUseUpJingCardIndex.Both == cbUseUpJingCardIndex then
                    currentCardAsPengInWeave = true
                end
                if true == bLianCard then
                    chiHuRight = bit.bor(chiHuRight, def_chr.PingHu)
                end

                if def_ShiChengMJMode.HongZhong == iShiChengMJMode and true == bPengCard then
                    chiHuRight = bit.bor(chiHuRight, def_chr.PingHu)
                end

                if false == arrayValue.bIsUpJingCardChange then
                    chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                end

                local cardEye_1 = arrayValue.cardEyes[1]
                local cardEye_2 = arrayValue.cardEyes[2]
                local bIsUpJing_cardEye_1 = IsUpJing(cardEye_1)
                local bIsUpJing_cardEye_2 = IsUpJing(cardEye_2)
                if cardEye_1 == cardEye_2 then
                    if cardEye_1 == currentCard and bIsUpJing_cardEye_2 then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    elseif true == bIsUpJing_cardEye_1 then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    end
                elseif cardEye_1 ~= cardEye_2 then
                    if cardEye_1 == currentCard and bIsUpJing_cardEye_2 then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    elseif cardEye_2 == currentCard and bIsUpJing_cardEye_1 then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    elseif bIsUpJing_cardEye_1 and bIsUpJing_cardEye_2 then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    elseif (bIsUpJing_cardEye_1 or bIsUpJing_cardEye_2) and (cardEye_2 ~= currentCard) and (cardEye_1 ~= currentCard) and true == currentCardAsPengInWeave then
                        chiHuRight = bit.bor(chiHuRight, def_chr.JingDiao)
                    end
                end
            end
        end
    end

    local ret = IsQiDaDui(cardIndexTemp, tWeaveItems)
    if ret and ret > 0 then
        if 2 == ret then
            if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) then
                chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
            end
            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
        elseif 1 == ret then
            if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) then
                chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
            end
            if def_chr.NULL ~= bit.band(chiHuRight, def_chr.DeGuo) then
                chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.DeGuo))
            end
        end

        chiHuRight = bit.bor(chiHuRight, def_chr.QiXiaoDui)
        for i, v in pairs(cardIndexTemp) do
            if 4 == v then
                chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.QiXiaoDui))
                chiHuRight = bit.bor(chiHuRight, def_chr.HaoHuaQiXiaoDui)
                break
            end
        end
    end

    if def_ShiChengMJMode.HongZhong == iShiChengMJMode then
        ret = IsQiDaDui(cardIndexTemp, tWeaveItems, currentCard)
        if ret and ret > 0 then
            if def_chr.NULL ~= bit.band(ret, def_chr.DeGuo) then
                if def_chr.NULL ~= bit.band(chiHuRight, bit.bor(def_chr.HaoHuaQiXiaoDui, def_chr.QiDaDui)) and def_chr.NULL ~= bit.band(chiHuRight, def_chr.DeGuo) then
                    --do nothing
                else
                    chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def.def_chr.PingHu, def_chr.QiXiaoDui, def_chr.HaoHuaQiXiaoDui)))
                    chiHuRight = bit.bor(chiHuRight, def_chr.QiDaDui)
                    chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                end
            else
                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) or def_chr.NULL == bit.band(chiHuRight, bit.bor(def_chr.QiXiaoDui, def_chr.HaoHuaQiXiaoDui)) then
                    chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
                    chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.DeGuo))
                    chiHuRight = bit.bor(chiHuRight, def_chr.QiDaDui)
                end
            end
        end
    end

    if def_ShiChengMJMode.HongZhong ~= iShiChengMJMode and def_chr.NULL ~= bit.band(chiHuRight, bit.bor(def_chr.PingHu, def_chr.QiXiaoDui, def_chr.QiDaDui, def_chr.HaoHuaQiXiaoDui)) then
        local originalChihuRight = chiHuRight
        if def_chr.NULL ~= bit.band(chiHuRight, bit.bor(def_chr.QiXiaoDui, def_chr.QiDaDui, def_chr.HaoHuaQiXiaoDui)) then
            local iZiYiSetRet = IsZiYiSe(cardIndexTemp, tWeaveItems, chiHuRight)
            if 2 == iZiYiSetRet then
                if def_chr.NULL == bit.band(chiHuRight, def_chr.DeGuo) then
                    --do nothing
                    if def_chr.NULL ~= bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.ZiYiSe)
                    end
                else
                    if def_chr.NULL ~= bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                        --do nothing
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.ZiYiSe)
                        chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                    end
                end

            elseif 1 == iZiYiSetRet then
                if def_chr.NULL == bit.band(chiHuRight, def_chr.DeGuo) then
                    if def_chr.NULL ~= bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                        --do nothing
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.ZiYiSe)
                    end
                else
                    --do nothing
                end
            end
        end

        if def_chr.NULL == bit.band(chiHuRight, def_chr.ZiYiSe) then
            local iQingYiSeRet = IsQingYiSe(cardIndexTemp, tWeaveItems)
            if 2 == iQingYiSeRet then
                if def_chr.NULL  == bit.band(chiHuRight, def_chr.DeGuo) then
                    if def_chr.NULL ~= bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                        --do nothing
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.PingHu, def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.QingYiSe)
                    end
                else
                    if def_chr.NULL ~= bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.PingHu, def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.QingYiSe)
                        chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                    end
                end
            elseif 1 == iQingYiSeRet then
                if def_chr.NULL == bit.band(chiHuRight, def_chr.DeGuo) then
                    if def_chr.NULL == bit.band(chiHuRight, def_chr.HaoHuaQiXiaoDui) then
                        --do nothing
                    else
                        chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.PingHu, def_chr.QiXiaoDui, def_chr.QiDaDui)))
                        chiHuRight = bit.bor(chiHuRight, def_chr.QingYiSe)
                    end
                else
                    --do nothing
                end
            end
        end

        local iHunYiSeRet = IsHunYiSe(cardIndexTemp, tWeaveItems)
        if 2 == iHunYiSeRet then
            if def_chr.NULL == bit.band(chiHuRight, def_chr.DeGuo) then
                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) then
                    chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
                    chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                else
                    --do nothing
                end

                local SingleCardCount = 0
                local DoubleCardCount = 0
                local TripleCardCount = 0
                local FourCardCount = 0
                local UpJingCardCount = 0
                local upJingCardIndex = switchToCardIndex(def_upJingCard)

                for i, v in pairs(cardIndexTemp) do
                    if upJingCardIndex and upJingCardIndex == i then
                        if v > 0 then
                            UpJingcardCount = UpJingCardCount + 1
                        end
                    else
                        if 1 == v then
                            SingleCardCount = SingleCardCount + 1
                        elseif 2 == v then
                            DoubleCardCount = DoubleCardCount + 1
                        elseif 3 == v then
                            TripleCardCount = TripleCardCount + 1
                        elseif 4 == v then
                            FourCardCount = FourCardCount + 1
                        end
                    end
                end

                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.ZiYiSe) then
                    if def_chr.NULL ~= bit.band(originalChihuRight, def_chr.QiDaDui) then
                        if (2 == UpJingCardCount and 0 == SingleCardCount and 0 == DoubleCardCount and 0 == FourCardCount)
                        or (3 == UpJingCardCount and 0 == SingleCardCount and 1 == DoubleCardCount and 0 == FourCardCount) then
                            chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.ZiYiSe))
                            chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                        end
                    elseif def_chr.NULL ~= bit.band(originalChihuRight, def_chr.QiXiaoDui) then
                        if (2 == UpJingCardCount and 0 == SingleCardCount and 0 == TripleCardCount)
                        or (4 == UpJingCardCount and 0 == SingleCardCount and 0 == TripleCardCount) then
                            chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.ZiYiSe))
                            chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                        end
                    end
                end

                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.QingYiSe) then
                    if def_chr.NULL ~= bit.band(originalChihuRight, def_chr.QiDaDui) then
                        if (2 == UpJingCardCount and 0 == SingleCardCount and 0 == DoubleCardCount and 0 == FourCardCount)
                        or (3 == UpJingCardCount and 0 == SingleCardCount and 1 == DoubleCardCount and 0 == FourCardCount) then
                            chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.QingYiSe))
                            chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                        end
                    elseif def_chr.NULL ~= bit.band(originalChihuRight, def_chr.QiXiaoDui) then
                        if (2 == UpJingCardCount and 0 == SingleCardCount and 0 == TripleCardCount)
                        or (4 == UpJingCardCount and 0 == SingleCardCount and 0 == TripleCardCount) then
                            chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.QingYiSe))
                            chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                        end
                    elseif def_chr.NULL ~= bit.band(originalChihuRight, def_chr.PingHu) and def_chr.NULL ~= bit.band(originalChihuRight, def_chr.JingDiao) then
                        if (2 == UpJingCardCount and 0 == DoubleCardCount and upJingCardIndex >= 28)
                        or (3 == UpJingCardCount and 1 == DoubleCardCount) then
                            chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.QingYiSe))
                            chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                            chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                        end
                    end
                end
            else
                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) then
                    chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
                    chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                    chiHuRight = bit.bor(chiHuRight, def_chr.DeGuo)
                else
                    --do nothing
                end
            end
        elseif 1 == iHunYiSeRet then
            if def_chr.NULL == bit.band(chiHuRight, def_chr.DeGuo) then
                if def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) then
                    chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.PingHu))
                    chiHuRight = bit.bor(chiHuRight, def_chr.HunYiSe)
                else
                    --do nothing
                end
            else
                --do nothing
            end
        end
    end

    if def_ShiChengMJMode.XuanBao == iShiChengMJMode then
        local bIsFourBao = IsFourBao(cardIndexTemp, tWeaveItems)
        if true == bIsFourBao and (def_chr.NULL == bit.band(chiHuRight, bit.bor(def_chr.QiXiaoDui, def_chr.HaoHuaQiXiaoDui, def_chr.QiDaDui, def_chr.QiDaDui, def_chr.ZiYiSe, def_chr.QingYiSe, def_chr.HunYiSe, def_chr.PingHu))
        or (def_chr.NULL ~= bit.band(chiHuRight, def_chr.PingHu) and def_chr.NULL == bit.band(chiHuRight, bit.bor(def_chr.QiXiaoDui, def_chr.HaoHuaQiXiaoDui, def_chr.QiDaDui, def_chr.ZiYiSe, def_chr.QingYiSe, def_chr.HunYiSe)))) then
            chiHuRight = bit.band(chiHuRight, bit.bnot(bit.bor(def_chr.DeGuo, def_chr.PingHu)))
            chiHuRight = bit.bor(chiHuRight, def_chr.FourBao)
        end
    end
    
    local tmpRight = bit.bor(def_chr.QiangGang, def_chr.DiHu, def_chr.TianHu)
    if chiHuRight == def_chr.QiangGang or chiHuRight == def_chr.DiHu or chiHuRight == def_chr.TianHu or chiHuRight == bit.bor(def_chr.QiangGang, def_chr.DiHu)
    or chiHuRight == bit.bor(def_chr.QiangGang, def_chr.TianHu) or chiHuRight == tmpRight then
        return def_actionType.NULL, outChiHuResult
    end

    if def_chr.NULL ~= chiHuRight then
        chiHuRight = bit.band(chiHuRight, bit.bnot(def_chr.JingDiao))
        if def_ShiChengMJMode.FangHu == iShiChengMJMode then
            if def_chr.NULL == bit.band(chiHuRight, def_chr.PingHu) and def_chr.NULL ~= bit.band(chiHuRight, bit.bor(def_chr.QiXiaoDui, def_chr.HaoHuaQiXiaoDui, def_chr.QiDaDui, def_chr.ZiYiSe, def_chr.QingYiSe, def_chr.HunYiSe)) then
               chiHuRight = def_chr.DaHu
            end
        end

        outChiHuResult.chiHuRight = chiHuRight

        return def_actionType.Chihu, outChiHuResult
    end

    return def_actionType.NULL, outChiHuResult
end

local def_tCardIndex = t_swithToCardIndex(def_tCardData)
local t0 = socket.gettime()
local t1 = socket.gettime()
local tingCardList = {}
for i = 1, def_maxIndex do
    local tmpCurrentCard = switchToCardData(i)
    local tmpIIII = switchToCardIndex(tmpCurrentCard)
    local actionType, result = AnalyseChiHuCard(def_tCardIndex, def_tWeaveItems, tmpCurrentCard, def_chiHuRight, def_ShiChengMJMode.XuanBao)
    t2 = socket.gettime()
    print(os.date("%Y-%m-%d %H:%M:%S", os.time()), "costTime="..(t2-t1).."\t".."Card="..string.format("%02X", tmpCurrentCard).."|"..string.format("%02d", tmpIIII), "chiHuRight="..string.format("%02X", result.chiHuRight) )
    t1 = t2

    -- do something
    if actionType > def_chr.NULL then
        table.insert(tingCardList, tmpCurrentCard)
    end
end

t1 = socket.gettime()
print(os.date("%Y-%m-%d %H:%M:%S", os.time()), "costTime="..(t1-t0).."\t")

local tmpTingCardList = clone(tingCardList)
