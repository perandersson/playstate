#include <playstate/memory/memory.h>
#include "ogl3_gfx_program.h"
#include "components/float_gfx_program_component.h"
#include "components/int_gfx_program_component.h"
#include "components/matrix_gfx_program_component.h"
#include "components/sampler2d_gfx_program_component.h"
#include "ogl3_render_system.h"
#include "ogl3_vertex_buffer.h"
#include "ogl3_index_buffer.h"
#include "ogl3_state_policy.h"
#include "ogl3_render_target_2d.h"
using namespace playstate;

namespace {
	OGL3GfxProgram* _current_program = 0;
}

OGL3GfxProgram::OGL3GfxProgram(IRenderSystem& renderSystem)
	: mProgramId(0), mApplied(false),
	mDepthTest(true), mDepthFunc(DepthFunc::Default),
	mBlend(false), mSrcFactor(SrcFactor::Default), mDestFactor(DestFactor::Default), 
	mClearColor(Color::Nothing), mClearDepth(1.0f), mCullFaces(CullFaces::Default),
	mScissorTest(false),
	mRenderSystem(renderSystem), mDepthRenderTarget(NULL), mApplyRenderTarget(false)
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
}

OGL3GfxProgram::OGL3GfxProgram(GLuint programId, IRenderSystem& renderSystem, const ScriptCollection& collection) 
	:  mProgramId(programId), mApplied(false),
	mDepthTest(true), mDepthFunc(DepthFunc::Default),
	mBlend(false), mSrcFactor(SrcFactor::Default), mDestFactor(DestFactor::Default), 
	mClearColor(Color::Nothing), mClearDepth(1.0f), mCullFaces(CullFaces::Default),
	mScissorTest(false),
	mRenderSystem(renderSystem), mDepthRenderTarget(NULL), mApplyRenderTarget(false)
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
	Prepare(collection);
}

OGL3GfxProgram::~OGL3GfxProgram()
{
	ComponentMap::iterator it = mComponents.begin();
	ComponentMap::const_iterator end = mComponents.end();
	for(; it != end; ++it) {
		IGfxProgramComponent* ptr = it->second;
		delete ptr;
	}
	mComponents.clear();

	if(mProgramId != 0) {
		glDeleteProgram(mProgramId);
		mProgramId = 0;
	}

	if(_current_program == this) {
		OGL3StatePolicyGuard::UseProgram(0);
		_current_program = NULL;
	}
}

void OGL3GfxProgram::MarkAsDirty()
{
	ComponentMap::iterator it = mComponents.begin();
	ComponentMap::const_iterator end = mComponents.end();
	for(;it != end; ++it) {
		it->second->MarkAsDirty();
	}

	mApplied = false;
}

void OGL3GfxProgram::Prepare(const ScriptCollection& collection)
{
	GLint numUniforms = 0;
	glGetProgramiv(mProgramId, GL_ACTIVE_UNIFORMS, &numUniforms);
	GLchar nameData[256] = {0};
	uint32 nextTextureId = 0;
	for(GLint uniformIndex = 0; uniformIndex < numUniforms; ++uniformIndex) {
		GLint arraySize = 0;
		GLenum type = 0;
		GLsizei actualLength = 0;
		
		//
		// http://www.opengl.org/sdk/docs/man/xhtml/glGetActiveUniform.xml
		// 

		glGetActiveUniform(mProgramId, uniformIndex, sizeof(nameData), &actualLength, &arraySize, &type, nameData);
		nameData[actualLength] = 0;
		playstate::string name = nameData;

		GLint componentId = glGetUniformLocation(mProgramId, name.c_str());
		IGfxProgramComponent* component = NULL;
		if(type == GL_FLOAT || type == GL_FLOAT_VEC2 || type == GL_FLOAT_VEC3 || type == GL_FLOAT_VEC4) {
			component = new FloatGfxProgramComponent(*this, componentId, type);
		} else if(type == GL_INT || type == GL_INT_VEC2 || type == GL_INT_VEC3 || type == GL_INT_VEC4) {
			component = new IntGfxProgramComponent(*this, componentId, type);
		} else if(type == GL_UNSIGNED_INT || type == GL_UNSIGNED_INT_VEC2 || type == GL_UNSIGNED_INT_VEC3 || type == GL_UNSIGNED_INT_VEC4) {
			component = new IntGfxProgramComponent(*this, componentId, type);
		} else if(type == GL_FLOAT_MAT4) {
			component = new MatrixGfxProgramComponent(*this, componentId, type);
		} else if(type == GL_SAMPLER_2D) {
			const playstate::string minFilterKey = playstate::string(nameData) + ".MinFilter";
			MinFilter::Enum minFilter = (MinFilter::Enum)collection.FindInt(minFilterKey.c_str(), MinFilter::Default);
			
			const playstate::string magFilterKey = playstate::string(nameData) + ".MagFilter";
			MagFilter::Enum magFilter = (MagFilter::Enum)collection.FindInt(magFilterKey.c_str(), MagFilter::Default);
			
			const playstate::string wrapsKey = playstate::string(nameData) + ".Wrap.S";
			TextureWrap::Enum wraps = (TextureWrap::Enum)collection.FindInt(wrapsKey.c_str(), TextureWrap::Default);
			
			const playstate::string wraptKey = playstate::string(nameData) + ".Wrap.T";
			TextureWrap::Enum wrapt = (TextureWrap::Enum)collection.FindInt(wraptKey.c_str(), TextureWrap::Default);

			component = new Sampler2DGfxProgramComponent(*this, componentId, nextTextureId++, minFilter, magFilter, wraps, wrapt);
		}

		if(component == NULL) {
			THROW_EXCEPTION(RenderingException, "Unmapped component type for uniform variable: '%s'", nameData);
		}

		mComponents.insert(std::make_pair(name, component));
	}
}

void OGL3GfxProgram::ApplyComponents()
{
	ComponentMap::iterator it = mComponents.begin();
	ComponentMap::const_iterator end = mComponents.end();
	for(;it != end; ++it) {
		it->second->Apply();
	}
}

void OGL3GfxProgram::Apply()
{
	if(_current_program != NULL) {
		_current_program->mApplied = false;
	}
	mApplied = true;
	_current_program = this;

	OGL3StatePolicyGuard::UseProgram(mProgramId);
	OGL3StatePolicyGuard::EnableBlend(mBlend);
	OGL3StatePolicyGuard::SetCullFaces(mCullFaces);
	OGL3StatePolicyGuard::EnableDepthTest(mDepthTest);
	if(mBlend)
		OGL3StatePolicyGuard::SetBlendFunc(mSrcFactor, mDestFactor);
	OGL3StatePolicyGuard::SetDepthFunc(mDepthFunc);
	OGL3StatePolicyGuard::EnableScissorTest(mScissorTest);
	OGL3StatePolicyGuard::SetScissorRect(mScissorRect);
	
	for(int i = 0; i < MaxDrawBuffers; ++i) {
		mRenderSystem.SetRenderTarget(mRenderTargets[i], i);
	}
	mRenderSystem.SetDepthRenderTarget(mDepthRenderTarget);
	mRenderSystem.ApplyRenderTargets();
	mApplyRenderTarget = false;

	ApplyComponents();
}

void OGL3GfxProgram::Clear(uint32 clearBits)
{
	GLenum clear = 0;
	if(BIT_ISSET(clearBits, ClearTypes::COLOR)) {
		clear = GL_COLOR_BUFFER_BIT;
	}

	if(BIT_ISSET(clearBits, ClearTypes::DEPTH)) {
		clear |= GL_DEPTH_BUFFER_BIT;
	}

	OGL3StatePolicyGuard::SetClearDepth(mClearDepth);
	OGL3StatePolicyGuard::SetClearColor(mClearColor);

	if(clear != 0) {
		glClear(clear);
	}
}

IGfxProgramComponent* OGL3GfxProgram::FindComponent(const char* name)
{
	return FindComponent(playstate::string(name));
}

IGfxProgramComponent* OGL3GfxProgram::FindComponent(const playstate::string& name)
{
	IGfxProgramComponent* component = NULL;
	ComponentMap::iterator it = mComponents.find(name);
	if(it == mComponents.end()) {
		return &mComponentNotFound;
	}
	return it->second;
}

void OGL3GfxProgram::Render(IVertexBuffer* buffer)
{
	Render(buffer, NULL, 0);
}

void OGL3GfxProgram::Render(IVertexBuffer* buffer, IIndexBuffer* IIndexBuffer)
{
	Render(buffer, IIndexBuffer, 0);
}

void OGL3GfxProgram::Render(IVertexBuffer* buffer, IIndexBuffer* indexBuffer, uint32 startElement)
{
	assert(_current_program == this && "You are trying to render a vertex and/or index buffer on a non-bound gfx program");
	assert_not_null(buffer);

	ApplyBuffers(buffer, indexBuffer);
		
	if(indexBuffer != NULL)
		static_cast<OGL3IndexBuffer*>(indexBuffer)->Render(buffer, startElement);
	else
		static_cast<OGL3VertexBuffer*>(buffer)->Render(startElement);

	//GLenum err = glGetError();
	//assert(err == GL_NO_ERROR);
}

void OGL3GfxProgram::Render(IVertexBuffer* buffer, IIndexBuffer* indexBuffer, uint32 startElement, uint32 numElements)
{
	assert(_current_program == this && "You are trying to render a vertex and/or index buffer on a non-bound gfx program");
	assert_not_null(buffer);
	assert(numElements > 0 && "You cannot draw 0 elements");

	ApplyBuffers(buffer, indexBuffer);
		
	if(indexBuffer != NULL)
		static_cast<OGL3IndexBuffer*>(indexBuffer)->Render(buffer, startElement, numElements);
	else
		static_cast<OGL3VertexBuffer*>(buffer)->Render(startElement, numElements);

	GLenum err = glGetError();
	assert(err == GL_NO_ERROR);
}

void OGL3GfxProgram::ApplyBuffers(IVertexBuffer* buffer, IIndexBuffer* indexBuffer)
{
	if(mApplyRenderTarget) {
		mApplyRenderTarget = false;
		mRenderSystem.ApplyRenderTargets();
	}

	OGL3StatePolicyGuard::BindVertexBuffer(static_cast<OGL3VertexBuffer*>(buffer));
	OGL3StatePolicyGuard::BindIndexBuffer(static_cast<OGL3IndexBuffer*>(indexBuffer));
	
	GLenum err = glGetError();
	assert(err == GL_NO_ERROR);
}

void OGL3GfxProgram::EnableDepthTest(bool enable)
{
	mDepthTest = enable;
	if(mApplied)
		OGL3StatePolicyGuard::EnableDepthTest(enable);
}

void OGL3GfxProgram::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	mDepthFunc = depthFunc;
	if(mApplied)
		OGL3StatePolicyGuard::SetDepthFunc(depthFunc);
}

void OGL3GfxProgram::EnableBlend(bool enable)
{
	mBlend = enable;
	if(mApplied)
		OGL3StatePolicyGuard::EnableBlend(enable);
}

void OGL3GfxProgram::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	mSrcFactor = sfactor;
	mDestFactor = dfactor;

	if(mApplied)
		OGL3StatePolicyGuard::SetBlendFunc(sfactor, dfactor);
}

void OGL3GfxProgram::SetClearColor(const Color& color)
{
	mClearColor = color;
	if(mApplied)
		OGL3StatePolicyGuard::SetClearColor(color);
}

void OGL3GfxProgram::SetCullFaces(CullFaces::Enum cullFaces)
{
	mCullFaces = cullFaces;
	if(mApplied)
		OGL3StatePolicyGuard::SetCullFaces(cullFaces);
}

void OGL3GfxProgram::EnableScissorTest(bool enable)
{
	mScissorTest = enable;
	if(mApplied)
		OGL3StatePolicyGuard::EnableScissorTest(mScissorTest);
}

void OGL3GfxProgram::SetScissorRect(const Rect& rect)
{
	mScissorRect = rect;
	if(mApplied)
		OGL3StatePolicyGuard::SetScissorRect(mScissorRect);
}

void OGL3GfxProgram::SetDepthRenderTarget(IRenderTarget2D* renderTarget)
{
	mDepthRenderTarget = static_cast<OGL3RenderTarget2D*>(renderTarget);

	if(mApplied) {
		mApplyRenderTarget = true;
		mRenderSystem.SetDepthRenderTarget(renderTarget);
	}
}

void OGL3GfxProgram::SetRenderTarget(IRenderTarget2D* renderTarget, uint32 index)
{
	assert(index < MaxDrawBuffers && "You are not allowed to use that many render targets at once");
	mRenderTargets[index] = static_cast<OGL3RenderTarget2D*>(renderTarget);

	if(mApplied) {
		mApplyRenderTarget = true;
		mRenderSystem.SetRenderTarget(renderTarget, index);
	}
}
