#include "../../memory/memory.h"
#include "truetype_font_resource_loader.h"
#include "../../math/color.h"
#include "../font.h"
using namespace playstate;

TrueTypeFontResourceLoader::TrueTypeFontResourceLoader(IFileSystem& fileSystem)
	: mFileSystem(fileSystem), mLibrary(NULL)
{
	if(FT_Init_FreeType(&mLibrary)) {
		THROW_EXCEPTION(LoadResourceException, "Could not initialize freetype library");
	}
}

TrueTypeFontResourceLoader::~TrueTypeFontResourceLoader()
{
	FT_Done_FreeType(mLibrary);
	mLibrary = NULL;
}

ResourceObject* TrueTypeFontResourceLoader::Load(IFile& file)
{
	static const playstate::string values = SAFE_STRING("()*+-/0123456789=.?@ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz");	

	uint32 size = file.Size();
	auto_array<byte> memory(size + 1);
	file.ReadBytes(memory.Get(), size);

	FT_Face face;
	FT_Error error = FT_New_Memory_Face(mLibrary, memory.Get(), size, 0, &face);
	if(error == FT_Err_Unknown_File_Format) {
		THROW_EXCEPTION(LoadResourceException, "Unknown face format for freetype file: '%s'", file.GetPath().c_str());
	} else if(error) {
		THROW_EXCEPTION(LoadResourceException, "Could not initialize freetype face. for file: '%s'", file.GetPath().c_str());
	}

	const uint32 px = 12;
	error = FT_Set_Char_Size(face, px << 6, px << 6, 96, 96);
	if(error) {
		THROW_EXCEPTION(LoadResourceException, "Invalid freetype char size for file: '%s'", file.GetPath().c_str());
	}

	const uint32 textureWidth = 1024;
	const uint32 textureHeight = 1024;
	auto_array<playstate::byte> bytes(textureWidth * textureHeight * 2);

	uint32 offsetX = 0;
	uint32 offsetY = 0;
	uint32 highestHeightOnRow = 0;
	Font::FontCharInfoMap infoMap;
	for(uint32 i = 0; i < values.length(); ++i) {
		const playstate::character ch = values[i];
		if(FT_Load_Glyph(face, FT_Get_Char_Index(face, ch), FT_LOAD_DEFAULT)) {
			// ERROR
		}
		
		FT_Glyph glyph;
		if(FT_Get_Glyph(face->glyph, &glyph)) {
			// ERROR
		}

		FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
		FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;

		const FT_Bitmap& bitmap = bitmap_glyph->bitmap;
		const uint32 bitmapHeight = (uint32)bitmap.rows;
		const uint32 bitmapWidth = (uint32)bitmap.width;

		// Texture offset 
		if(offsetX + bitmapWidth > textureWidth) {
			offsetX = 0;
			offsetY += highestHeightOnRow;
		}

		CopyToBuffer(offsetX, offsetY, bitmapWidth, bitmapHeight, bytes.Get(), bitmap.buffer);

		// Create character description
		FontCharInfo* info = new FontCharInfo();
		info->Size.Set(bitmapWidth, bitmapHeight);
		info->Offset.Set(0, px - (face->glyph->metrics.horiBearingY >> 6));
		info->BottomRightTexCoord.Set((offsetX + bitmapWidth) / (float32)textureWidth, offsetY / (float32)textureHeight);
		info->TopLeftTexCoord.Set(offsetX / (float32)textureWidth, (offsetY + bitmapHeight) / (float32)textureHeight);
		infoMap.insert(std::make_pair(ch, info));

		offsetX += bitmap.width;
		highestHeightOnRow = highestHeightOnRow < bitmapHeight ? bitmapHeight : highestHeightOnRow;

		FT_Done_Glyph(glyph);
	}
	FT_Done_Face(face);
	
	GLuint textureId = 0;	
	glGenTextures(1, &textureId);
	StatePolicy::BindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, textureWidth, textureHeight, 0, GL_RED, GL_UNSIGNED_BYTE, bytes.Get());

	GLenum err = glGetError();
	if(err == GL_INVALID_ENUM) {
		THROW_EXCEPTION(LoadResourceException, "Could not load TrueType font '%s'. \
											   The OpenGL generated resource didn't have \
											   valid 'target', 'format' or 'type' parameters", file.GetPath().c_str());
	}

	return new Font(textureId, textureWidth, textureHeight, infoMap, px / 2.0f, face->height >> 6);
}

void TrueTypeFontResourceLoader::CopyToBuffer(uint32 x, uint32 y, uint32 width, uint32 height, playstate::byte* target, const playstate::byte* src)
{
	for(int32 t = height - 1; t >= 0; --t) {
		for(uint32 s = 0; s < width; ++s) {
			const uint32 targetOffset = (s + x) + (t + y) * 1024;
			target[targetOffset] = *src++;
		}
	}
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
