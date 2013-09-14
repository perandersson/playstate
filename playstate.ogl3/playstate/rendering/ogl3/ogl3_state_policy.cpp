#include <playstate/memory/memory.h>
#include "ogl3_state_policy.h"
#include "ogl3_vertex_buffer.h"
#include "ogl3_index_buffer.h"
using namespace playstate;

void OGL3StatePolicy::BindIndexBuffer(OGL3IndexBuffer* indexBuffer)
{
	if(indexBuffer != NULL) {
		indexBuffer->Bind();
	} else {
		//_IIndexBuffer->Unbind();
	}
}

void OGL3StatePolicy::BindVertexBuffer(OGL3VertexBuffer* vertexBuffer)
{
	if(vertexBuffer != NULL) {
		vertexBuffer->Bind();
	} else {
		// Unbind?
	}
}

void OGL3StatePolicy::UseProgram(GLuint programID)
{
	glUseProgram(programID);
}

void OGL3StatePolicy::EnableDepthTest(bool enable)
{
	if(enable)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void OGL3StatePolicy::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	glDepthFunc(OGL3DepthFunc[depthFunc]);
}

void OGL3StatePolicy::EnableBlend(bool enable)
{
	if(enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}

void OGL3StatePolicy::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	glBlendFunc(OGL3SrcFactor[sfactor], OGL3DestFactor[dfactor]);
}

void OGL3StatePolicy::SetCullFaces(CullFaces::Enum cullFaces)
{
	if(cullFaces == CullFaces::DISABLED)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(OGL3CullFaces[cullFaces]);
		glCullFace(GL_BACK);
	}
}

void OGL3StatePolicy::SetClearColor(const Color& color)
{
	glClearColor(color.Red, color.Green, color.Blue, color.Alpha);
}

void OGL3StatePolicy::SetClearDepth(float32 depth)
{
	glClearDepth(depth);
}

void OGL3StatePolicy::EnableScissorTest(bool enable)
{
	if(enable)
		glEnable(GL_SCISSOR_TEST);
	else
		glDisable(GL_SCISSOR_TEST);
}

void OGL3StatePolicy::SetScissorRect(const Rect& rect)
{
	glScissor(rect.X, rect.Y, rect.Width, rect.Height);
}

void OGL3StatePolicy::SetActiveTexture(uint32 id)
{
	glActiveTexture(GL_TEXTURE0 + id);
}

void OGL3StatePolicy::BindTexture(GLenum type, GLuint texture)
{
	glBindTexture(type, texture);
}

void OGL3StatePolicy::Viewport(const Rect& viewport)
{
	glViewport(viewport.X, viewport.Y, viewport.Width, viewport.Height);
}


namespace playstate {
	const OGL3VertexBuffer* _vertexBuffer = 0;
	const OGL3IndexBuffer* _indexBuffer = 0;

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

void OGL3StatePolicyGuard::BindIndexBuffer(OGL3IndexBuffer* indexBuffer)
{
	if(_indexBuffer != indexBuffer) {
		OGL3StatePolicy::BindIndexBuffer(indexBuffer);
		_indexBuffer = indexBuffer;
	}
}

void OGL3StatePolicyGuard::BindVertexBuffer(OGL3VertexBuffer* vertexBuffer)
{
	if(_vertexBuffer != vertexBuffer) {
		_vertexBuffer = vertexBuffer;
		OGL3StatePolicy::BindVertexBuffer(vertexBuffer);
	}
}

void OGL3StatePolicyGuard::UseProgram(GLuint programID)
{
	if(_programID == programID)
		return;

	_programID = programID;
	OGL3StatePolicy::UseProgram(programID);
}

void OGL3StatePolicyGuard::EnableDepthTest(bool enable)
{
	if(enable == _depthTest)
		return;

	_depthTest = enable;
	OGL3StatePolicy::EnableDepthTest(enable);
}

void OGL3StatePolicyGuard::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	if(depthFunc == _depthFunc)
		return;

	_depthFunc = depthFunc;
	OGL3StatePolicy::SetDepthFunc(depthFunc);
}

void OGL3StatePolicyGuard::EnableBlend(bool enable)
{
	if(enable == _blend)
		return;

	_blend = enable;
	OGL3StatePolicy::EnableBlend(enable);
}

void OGL3StatePolicyGuard::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	if(_srcFunc == sfactor && _destFunc == dfactor)
		return;

	_srcFunc = sfactor;
	_destFunc = dfactor;
	OGL3StatePolicy::SetBlendFunc(sfactor, dfactor);
}

void OGL3StatePolicyGuard::SetCullFaces(CullFaces::Enum cullFaces)
{
	if(_cullFaces == cullFaces)
		return;

	OGL3StatePolicy::SetCullFaces(cullFaces);
	_cullFaces = cullFaces;
}

void OGL3StatePolicyGuard::SetClearColor(const Color& color)
{
	if(_clearColor == color)
		return;

	OGL3StatePolicy::SetClearColor(color);
	_clearColor = color;
}

void OGL3StatePolicyGuard::SetClearDepth(float32 depth)
{
	if(abs(_clearDepth - depth) < FLT_EPSILON)
		return;

	_clearDepth = depth;
	OGL3StatePolicy::SetClearDepth(depth);
}
		
void OGL3StatePolicyGuard::EnableScissorTest(bool enable)
{
	if(_scissorTest == enable)
		return;

	_scissorTest = enable;
	OGL3StatePolicy::EnableScissorTest(enable);
}

void OGL3StatePolicyGuard::SetScissorRect(const Rect& rect)
{
	if(_scissorRect == rect)
		return;

	OGL3StatePolicy::SetScissorRect(rect);
	_scissorRect = rect;
}

void OGL3StatePolicyGuard::SetActiveTexture(uint32 id)
{
	if(_activeTexture == id)
		return;

	OGL3StatePolicy::SetActiveTexture(id);
	_activeTexture = id;
}

void OGL3StatePolicyGuard::BindTexture(GLenum type, GLuint texture)
{
	if(_bindTextures[_activeTexture] == texture)
		return;

	OGL3StatePolicy::BindTexture(type, texture);
	_bindTextures[_activeTexture] = texture;
}

void OGL3StatePolicyGuard::InvalidateTexture(GLuint texture)
{
	for(uint32 i = 0; i < MaxActiveTextures; ++i) {
		if(_bindTextures[i] == texture)
			_bindTextures[i] = 0;
	}
}
		
void OGL3StatePolicyGuard::Viewport(const Rect& viewport)
{
	if(_viewport == viewport)
		return;

	OGL3StatePolicy::Viewport(viewport);
	_viewport = viewport;
}

void OGL3StatePolicyGuard::MarkAsDirty()
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	
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
