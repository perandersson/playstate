local srcblend = {}
srcblend.SRC_ALPHA = tonumber("302", 16) -- #define GL_SRC_ALPHA 0x0302
srcblend.SRC_COLOR = tonumber("300", 16) -- #define GL_SRC_COLOR 0x0300
srcblend.ONE_MINUS_SRC_ALPHA = tonumber("303", 16) -- #define GL_ONE_MINUS_SRC_ALPHA 0x0303
srcblend.ONE = 1 -- #define GL_ONE 1
srcblend.ZERO = 0 -- #define GL_ZERO 0
return srcblend