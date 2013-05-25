local wrap = {}
wrap.CLAMP_TO_EDGE = tonumber("812F", 16) -- #define GL_CLAMP_TO_EDGE 0x812F
wrap.CLAMP_TO_BORDER = tonumber("812D", 16) -- #define GL_CLAMP_TO_BORDER 0x812D
wrap.MIRRORED_REPEAT = tonumber("8370", 16) --#define GL_MIRRORED_REPEAT 0x8370
wrap.REPEAT = tonumber("2901", 16) -- #define GL_REPEAT 0x2901
return wrap