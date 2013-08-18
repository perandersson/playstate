#include "../../../memory/memory.h"
#include "truetype_font_resource_loader.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
using namespace playstate;

TrueTypeFontResourceLoader::TrueTypeFontResourceLoader(IFileSystem& fileSystem)
	: mFileSystem(fileSystem), mDefaultResource(NULL)
{
}

TrueTypeFontResourceLoader::~TrueTypeFontResourceLoader()
{
	if(mDefaultResource != NULL) {
		delete mDefaultResource;
		mDefaultResource = NULL;
	}
}

ResourceObject* TrueTypeFontResourceLoader::Load(IFile& file)
{
	// https://code.google.com/p/freetype-gl/
	static const char chars[] = "()*+-/0123456789=?@ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
	

	FT_Library library;
	FT_Error error = FT_Init_FreeType(&library);

	if(error) {
		// ERROR
	}

	uint64 size = file.Size();
	byte* memory = new byte[size + 1];
	file.ReadBytes(memory, size);

	FT_Face face;
	error = FT_New_Memory_Face(library, memory, size, 0, &face);
	if(error == FT_Err_Unknown_File_Format) {
		// ERROR
	} else if(error) {
		// ERROR
	}

	error = FT_Set_Char_Size(face, 0, 16 * 64, 300, 300);
	if(error) {
		// ERROR
	}

	/*FT_Glyph glyph;
	error = FT_Load_Glyph(face, FT_Get_Char_Index(face, ch)

	FT_Glyph_To_Bitmap(&glyph, ft_render_mode_normal, 0, 1 );
    FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
 
    // This Reference Will Make Accessing The Bitmap Easier.
    FT_Bitmap& bitmap=bitmap_glyph->bitmap;
	*/
	return NULL;
}

int TrueTypeFontResourceLoader::GetNextP2(int a) const
{
	int val = 2;
	while(val < a) val <<= 1;
	return val;
}

ResourceObject* TrueTypeFontResourceLoader::GetDefaultResource()
{
	return NULL;
}

bool TrueTypeFontResourceLoader::IsThreadable() const
{
	return false;
}
