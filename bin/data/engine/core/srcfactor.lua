local srcfactor = {}
srcfactor.ZERO = 0
srcfactor.ONE = 1
srcfactor.SRC_COLOR = tonumber("300", 16)
srcfactor.ONE_MINUS_SRC_COLOR = tonumber("301", 16)
srcfactor.SRC_ALPHA = tonumber("302", 16)
srcfactor.ONE_MINUS_SRC_ALPHA = tonumber("303", 16)
srcfactor.DST_ALPHA = tonumber("304", 16)
srcfactor.ONE_MINUS_DST_ALPHA = tonumber("305", 16)
srcfactor.DST_COLOR = tonumber("306", 16)
srcfactor.ONE_MINUS_DST_COLOR = tonumber("307", 16)
srcfactor.CONSTANT_COLOR = tonumber("8001", 16)
srcfactor.ONE_MINUS_CONSTANT_COLOR = tonumber("8002", 16)
srcfactor.CONSTANT_ALPHA = tonumber("8003", 16)
srcfactor.ONE_MINUS_CONSTANT_ALPHA = tonumber("8004", 16)
return srcfactor