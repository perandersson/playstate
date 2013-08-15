local destblend = {}
destblend.ONE_MINUS_DST_ALPHA = tonumber("305", 16) -- #define GL_ONE_MINUS_DST_ALPHA 0x0305
destblend.DST_ALPHA = tonumber("304", 16) -- #define GL_DST_ALPHA 0x0304
destblend.DST_COLOR = tonumber("306", 16) -- #define GL_DST_COLOR 0x0306
destblend.ONE_MINUS_DST_COLOR = tonumber("307", 16) -- #define GL_ONE_MINUS_DST_COLOR 0x0307
destblend.ONE_MINUS_SRC_ALPHA = tonumber("303", 16) -- #define GL_ONE_MINUS_SRC_ALPHA 0x0303
destblend.ONE = 1 -- #define GL_ONE 1
return destblend