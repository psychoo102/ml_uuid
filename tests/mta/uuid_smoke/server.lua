local function check(value, version)
    assert(type(value) == "string", "UUID must be a string")
    assert(#value == 36, "UUID must have 36 characters")
    assert(value:sub(9, 9) == "-" and value:sub(14, 14) == "-"
        and value:sub(19, 19) == "-" and value:sub(24, 24) == "-", "UUID separators")
    local hex = value:gsub("-", "")
    assert(#hex == 32 and not hex:find("[^0-9a-f]"), "UUID lowercase hex")
    assert(value:sub(15, 15) == version, "UUID version")
    assert(value:sub(20, 20):match("[89ab]"), "UUID RFC variant")
end

addEventHandler("onResourceStart", resourceRoot, function()
    assert(type(uuidv4) == "function" and type(uuidv7) == "function", "ml_uuid is not loaded")
    local seen, previous, regressions = {}, nil, 0
    local before = getRealTime().timestamp
    local first, last
    for i = 1, 10000 do
        local v4, err4 = uuidv4()
        local v7, err7 = uuidv7()
        assert(v4, err4)
        assert(v7, err7)
        check(v4, "4")
        check(v7, "7")
        assert(not seen[v4] and not seen[v7], "Duplicate UUID in sample")
        seen[v4], seen[v7] = true, true
        if previous and v7 <= previous then regressions = regressions + 1 end
        first = first or v7
        last, previous = v7, v7
    end
    local after = getRealTime().timestamp
    for _, value in ipairs({first, last}) do
        local timestamp = tonumber(value:sub(1, 8) .. value:sub(10, 13), 16) / 1000
        assert(timestamp >= before - 1 and timestamp <= after + 1,
            "UUID v7 timestamp outside test interval (also check clock adjustments)")
    end
    outputDebugString("[ml_uuid] PASS: 20,000 UUIDs; format, versions, variants, uniqueness and timestamps")
    if regressions > 0 then
        outputDebugString("[ml_uuid] v7 ordering regressions: " .. regressions
            .. "; check clock changes and Boost ordering conditions", 2)
    else
        outputDebugString("[ml_uuid] PASS: v7 ordering in this sample")
    end
end)
