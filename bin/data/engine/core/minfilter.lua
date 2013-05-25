local minfilter = {}
minfilter.NEAREST =  tonumber("2600", 16) -- #define GL_NEAREST 0x2600
minfilter.LINEAR = tonumber("2601", 16) -- #define GL_LINEAR 0x2601
minfilter.NEAREST_MIPMAP_NEAREST =  tonumber("2700", 16)-- #define GL_NEAREST_MIPMAP_NEAREST 0x2700
minfilter.LINEAR_MIPMAP_NEAREST = tonumber("2701", 16) -- #define GL_LINEAR_MIPMAP_NEAREST 0x2701
minfilter.NEAREST_MIPMAP_LINEAR = tonumber("2702", 16) --#define GL_NEAREST_MIPMAP_LINEAR 0x2702
minfilter.LINEAR_MIPMAP_LINEAR = tonumber("2703", 16) -- #define GL_LINEAR_MIPMAP_LINEAR 0x2703
return minfilter