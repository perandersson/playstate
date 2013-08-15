local destfactor = {}
destfactor.ZERO = 0
destfactor.ONE = 1
destfactor.SRC_COLOR = tonumber("300", 16)
destfactor.ONE_MINUS_SRC_COLOR = tonumber("301", 16)
destfactor.SRC_ALPHA = tonumber("302", 16)
destfactor.ONE_MINUS_SRC_ALPHA = tonumber("303", 16)
destfactor.DST_ALPHA = tonumber("304", 16)
destfactor.ONE_MINUS_DST_ALPHA = tonumber("305", 16)
destfactor.DST_COLOR = tonumber("306", 16)
destfactor.ONE_MINUS_DST_COLOR = tonumber("307", 16)
destfactor.CONSTANT_COLOR = tonumber("8001", 16)
destfactor.ONE_MINUS_CONSTANT_COLOR = tonumber("8002", 16)
destfactor.CONSTANT_ALPHA = tonumber("8003", 16)
destfactor.ONE_MINUS_CONSTANT_ALPHA = tonumber("8004", 16)
return destfactor