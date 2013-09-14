#include "../../memory/memory.h"
#include "default_gfx_program_component.h"

#include <cassert>

using namespace playstate;

DefaultGfxProgramComponent::DefaultGfxProgramComponent()
{
}

DefaultGfxProgramComponent::~DefaultGfxProgramComponent()
{
}

void DefaultGfxProgramComponent::SetFloat(float32 a)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetFloat(float32 a, float32 b)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetFloat(float32 a, float32 b, float32 c)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetFloat(float32 a, float32 b, float32 c, float32 d)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetInt(int32 a)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetInt(int32 a, int32 b)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetInt(int32 a, int32 b, int32 c)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetInt(int32 a, int32 b, int32 c, int32 d)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetTexture(ITexture2D* texture)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetTexture(Resource<ITexture2D>& texture)
{
	SetTexture(texture.Get());
}

void DefaultGfxProgramComponent::SetMatrix(const Matrix4x4& matrix)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetColorRGBA(const Color& color)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetColorRGB(const Color& color)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetVector3(const Vector3& vec)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::SetVector2(const Vector2& vec)
{
	assert(false && "Invalid component type");
}

void DefaultGfxProgramComponent::MarkAsDirty()
{
}
