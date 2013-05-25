local cullfaces = {}
cullfaces.DISABLE = 0
cullfaces.CW = tonumber("900", 16) -- #define GL_CW 0x0900
cullfaces.CCW = tonumber("901", 16) -- #define GL_CCW 0x0901
return cullfaces