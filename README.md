PlayState
========

PlayState is a game engine that I am developing because I want to keep myself updated on the newer types of features available within the game development community.

Please read the [wiki](https://github.com/perandersson/playstate/wiki) for more information.

### Credits

http://glew.sourceforge.net/ - GLEW (OpenGL Extension library)

http://www.lua.org/ - The Programming Language Lua (LUA)

http://freeimage.sourceforge.net/ - FreeImage

http://www.freetype.org/ - The FreeType Project

http://kcat.strangesoft.net/openal.html - OpenAL Soft

Modifications in the OpenAL Soft build:
* Altered so that the source project builds the library with a _d suffix for the *debug* filename. Example (OpenAL32_d.lib). This is so that I can have both builds in the same bin directory.