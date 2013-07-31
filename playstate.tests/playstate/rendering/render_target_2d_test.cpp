#include <playstate/rendering/render_target_2d.h>
#include "../test/test.h"
using namespace playstate;

TEST_SUITE(RenderTarget2D)
{
	UNIT_TEST(RenderTarget2D_CreateWithNoBoundTextureSuccessfully)
	{
		const uint32 width = 1024;
		const uint32 height = 768;
		const TextureFormat::Enum format = TextureFormat::RGBA12;

		glmock::IFramework* mock = glmock::Create();
		GLint boundTextureId = 0;
		mock->glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTextureId);
		GLuint textureId = 1;
		mock->glGenTextures(1, &textureId);
		mock->glBindTexture(GL_TEXTURE_2D, textureId);
		mock->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA12, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
		mock->glFlush();
		mock->glGetError()->Returns(GL_NO_ERROR);
		mock->glDeleteTextures(1, &textureId);

		RenderTarget2D* unitToTest = RenderTarget2D::Create(width, height, format);

		ASSERT_NOT_NULL(unitToTest);
		ASSERT_EQUALS(unitToTest->GetWidth(), width);
		ASSERT_EQUALS(unitToTest->GetHeight(), height);
		ASSERT_EQUALS(unitToTest->GetFormat(), TextureFormat::RGBA12);

		delete unitToTest;
		glmock::Destroy(mock);
	}
}
