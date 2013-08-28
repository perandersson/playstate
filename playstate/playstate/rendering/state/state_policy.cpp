#include "../../memory/memory.h"
#include "state_policy.h"
using namespace playstate;

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

	const VertexBuffer* _vertexBuffer = 0;
	const IndexBuffer* _indexBuffer = 0;

	Rect _viewport;
}

void StatePolicy::UseProgram(GLuint programID)
{
	if(_programID == programID)
		return;

	_programID = programID;
	glUseProgram(programID);
}

void StatePolicy::EnableDepthTest(bool enable)
{
	if(enable == _depthTest)
		return;

	_depthTest = enable;
	if(enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void StatePolicy::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	if(depthFunc == _depthFunc)
		return;

	_depthFunc = depthFunc;
	glDepthFunc((GLenum)depthFunc);
}

void StatePolicy::EnableBlend(bool enable)
{
	if(enable == _blend)
		return;

	_blend = enable;
	if(enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void StatePolicy::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	if(_srcFunc == sfactor && _destFunc == dfactor)
		return;

	_srcFunc = sfactor;
	_destFunc = dfactor;
	glBlendFunc((GLenum)sfactor, (GLenum)dfactor);
}

void StatePolicy::SetCullFaces(CullFaces::Enum cullFaces)
{
	if(_cullFaces == cullFaces)
		return;

	if(cullFaces == CullFaces::DISABLED)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		if(_cullFaces == CullFaces::DISABLED)
			glEnable(GL_CULL_FACE);

		glFrontFace((GLenum)cullFaces);
		glCullFace(GL_BACK);
	}

	_cullFaces = cullFaces;
}

void StatePolicy::SetClearColor(const Color& color)
{
	if(_clearColor == color)
		return;

	glClearColor(color.Red, color.Green, color.Blue, color.Alpha);
	_clearColor = color;
}

void StatePolicy::SetClearDepth(float32 depth)
{
	if(abs(_clearDepth - depth) < FLT_EPSILON)
		return;

	_clearDepth = depth;
	glClearDepth(depth);
}

void StatePolicy::EnableScissorTest(bool enable)
{
	if(_scissorTest == enable)
		return;

	_scissorTest = enable;
	if(enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void StatePolicy::SetScissorRect(const Rect& rect)
{
	if(_scissorRect == rect)
		return;

	glScissor(rect.X, rect.Y, rect.Width, rect.Height);
	_scissorRect = rect;
}

void StatePolicy::SetActiveTexture(uint32 id)
{
	if(_activeTexture == id)
		return;

	glActiveTexture(GL_TEXTURE0 + id);
	_activeTexture = id;
}

void StatePolicy::BindTexture(GLenum type, GLuint texture)
{
	if(_bindTextures[_activeTexture] == texture)
		return;

	glBindTexture(type, texture);
	_bindTextures[_activeTexture] = texture;
}

void StatePolicy::InvalidateTexture(GLuint texture)
{
	for(uint32 i = 0; i < MaxActiveTextures; ++i) {
		if(_bindTextures[i] == texture)
			_bindTextures[i] = 0;
	}
}

void StatePolicy::Viewport(const Rect& viewport)
{
	if(_viewport == viewport)
		return;

	glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
	_viewport = viewport;
}

void StatePolicy::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
	if(_vertexBuffer != vertexBuffer) {
		_vertexBuffer = vertexBuffer;
		vertexBuffer->Bind();
	}
}

void StatePolicy::BindIndexBuffer(IndexBuffer* indexBuffer)
{
	if(_indexBuffer != indexBuffer) {
		if(indexBuffer == NULL) {
			//_indexBuffer->Unbind();
		} else {
			indexBuffer->Bind();
		}
		_indexBuffer = indexBuffer;
	}
}

void StatePolicy::MarkAsDirty()
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

	_vertexBuffer = 0;
	_indexBuffer = 0;
}
