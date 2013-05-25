local depthfunc = {}
depthfunc.NEVER = tonumber("200", 16) -- #define GL_NEVER 0x0200
depthfunc.LESS = tonumber("201", 16) -- #define GL_LESS 0x0201
depthfunc.EQUAL = tonumber("202", 16) -- #define GL_EQUAL 0x0202
depthfunc.LEQUAL = tonumber("203", 16) -- #define GL_LEQUAL 0x0203
depthfunc.GREATER = tonumber("204", 16) -- #define GL_GREATER 0x0204
depthfunc.NOTEQUAL = tonumber("205", 16) -- #define GL_NOTEQUAL 0x0205
depthfunc.GEQUAL = tonumber("206", 16) -- #define GL_GEQUAL 0x0206
depthfunc.ALWAYS = tonumber("207", 16) -- #define GL_ALWAYS 0x0207
return depthfunc