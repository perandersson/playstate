#include "../../memory/memory.h"
#include "state_policy.h"
using namespace playstate;

void StatePolicy::UseProgram(GLuint programID)
{
	glUseProgram(programID);
}

void StatePolicy::EnableDepthTest(bool enable)
{
	if(enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void StatePolicy::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	glDepthFunc((GLenum)depthFunc);
}

void StatePolicy::EnableBlend(bool enable)
{
	if(enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void StatePolicy::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
}

void StatePolicy::SetCullFaces(CullFaces::Enum cullFaces)
{
	if(cullFaces == CullFaces::DISABLED)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glFrontFace((GLenum)cullFaces);
		glCullFace(GL_BACK);
	}
}

void StatePolicy::SetClearColor(const Color& color)
{
	glClearColor(color.Red, color.Green, color.Blue, color.Alpha);
}

void StatePolicy::SetClearDepth(float32 depth)
{
	glClearDepth(depth);
}

void StatePolicy::EnableScissorTest(bool enable)
{
	if(enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void StatePolicy::SetScissorRect(const Rect& rect)
{
	glScissor(rect.X, rect.Y, rect.Width, rect.Height);
}

void StatePolicy::SetActiveTexture(uint32 id)
{
	glActiveTexture(GL_TEXTURE0 + id);
}

void StatePolicy::BindTexture(GLenum type, GLuint texture)
{
	glBindTexture(type, texture);
}

void StatePolicy::Viewport(const Rect& viewport)
{
	glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
}

////

namespace {
	GLuint _programID = 0;

	bool _depthTest = true;
	DepthFunc::Enum _depthFunc = DepthFunc::LEQUAL;

	bool _blend = false;
	SrcFactor::Enum _srcFunc = SrcFactor::ONE;
	DestFactor::Enum _destFunc = DestFactor::ZERO;

	CullFaces::Enum _cullFaces = CullFaces::CCW;

	Color _clearColor = Color::Nothing;
	float32 _clearDepth = 1.0f;
	
	bool _scissorTest = false;
	Rect _scissorRect;

	uint32 _activeTexture = 0;
	uint32 _bindTextures[MaxActiveTextures] = {0};

	Rect _viewport;
}

void StatePolicyGuard::UseProgram(GLuint programID)
{
	if(_programID == programID)
		return;

	_programID = programID;
	StatePolicy::UseProgram(programID);
}

void StatePolicyGuard::EnableDepthTest(bool enable)
{
	if(enable == _depthTest)
		return;

	_depthTest = enable;
	StatePolicy::EnableDepthTest(enable);
}

void StatePolicyGuard::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	if(depthFunc == _depthFunc)
		return;

	_depthFunc = depthFunc;
	StatePolicy::SetDepthFunc(depthFunc);
}

void StatePolicyGuard::EnableBlend(bool enable)
{
	if(enable == _blend)
		return;

	_blend = enable;
	StatePolicy::EnableBlend(enable);
}

void StatePolicyGuard::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	if(_srcFunc == sfactor && _destFunc == dfactor)
		return;

	_srcFunc = sfactor;
	_destFunc = dfactor;
	StatePolicy::SetBlendFunc(sfactor, dfactor);
}

void StatePolicyGuard::SetCullFaces(CullFaces::Enum cullFaces)
{
	if(_cullFaces == cullFaces)
		return;

	StatePolicy::SetCullFaces(cullFaces);
	_cullFaces = cullFaces;
}

void StatePolicyGuard::SetClearColor(const Color& color)
{
	if(_clearColor == color)
		return;

	StatePolicy::SetClearColor(color);
	_clearColor = color;
}

void StatePolicyGuard::SetClearDepth(float32 depth)
{
	if(abs(_clearDepth - depth) < FLT_EPSILON)
		return;

	_clearDepth = depth;
	StatePolicy::SetClearDepth(depth);
}
		
void StatePolicyGuard::EnableScissorTest(bool enable)
{
	if(_scissorTest == enable)
		return;

	_scissorTest = enable;
	StatePolicy::EnableScissorTest(enable);
}

void StatePolicyGuard::SetScissorRect(const Rect& rect)
{
	if(_scissorRect == rect)
		return;

	StatePolicy::SetScissorRect(rect);
	_scissorRect = rect;
}

void StatePolicyGuard::SetActiveTexture(uint32 id)
{
	if(_activeTexture == id)
		return;

	StatePolicy::SetActiveTexture(id);
	_activeTexture = id;
}

void StatePolicyGuard::BindTexture(GLenum type, GLuint texture)
{
	if(_bindTextures[_activeTexture] == texture)
		return;

	StatePolicy::BindTexture(type, texture);
	_bindTextures[_activeTexture] = texture;
}

void StatePolicyGuard::InvalidateTexture(GLuint texture)
{
	for(uint32 i = 0; i < MaxActiveTextures; ++i) {
		if(_bindTextures[i] == texture)
			_bindTextures[i] = 0;
	}
}
		
void StatePolicyGuard::Viewport(const Rect& viewport)
{
	if(_viewport == viewport)
		return;

	StatePolicy::Viewport(viewport);
	_viewport = viewport;
}

void StatePolicyGuard::MarkAsDirty()
{
	UseProgram(0);
	
	EnableDepthTest(true);
	SetDepthFunc(DepthFunc::Default);

	EnableBlend(false);
	SetBlendFunc(SrcFactor::Default, DestFactor::Default);

	SetCullFaces(CullFaces::Default);

	SetClearColor(Color::Nothing);
	SetClearDepth(1.0f);

	SetActiveTexture(0);
	for(uint32 i = 0; i < MaxActiveTextures; ++i) {
		_bindTextures[i] = 0;
	}
	
	_viewport = Rect();
}
