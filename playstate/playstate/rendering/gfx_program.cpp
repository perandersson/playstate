#include "../memory/memory.h"

#include "gfx_program.h"
#include "texture2d.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "render_system.h"
#include "render_target_2d.h"
#include "../resources/exception/load_resource_exception.h"
#include "components/float_gfx_program_component.h"
#include "components/int_gfx_program_component.h"
#include "components/matrix_gfx_program_component.h"
#include "components/sampler2d_gfx_program_component.h"

#include <algorithm>
#include <hash_map>
#include <cassert>

using namespace playstate;

namespace {
	GfxProgram* _current_program = 0;
}

GfxProgram::GfxProgram(IRenderSystem& renderSystem)
	: mProgramId(0), mApplied(false),
	mDepthTest(true), mDepthFunc(DepthFunc::Default),
	mBlend(false), mSrcFactor(SrcFactor::Default), mDestFactor(DestFactor::Default), 
	mClearColor(Color::Nothing), mClearDepth(1.0f), mCullFaces(CullFaces::Default),
	mScissorTest(false),
	mRenderSystem(renderSystem), mDepthRenderTarget(NULL), mApplyRenderTarget(false)
{
	memset(mRenderTargets, 0, sizeof(mRenderTargets));
}

GfxProgram::GfxProgram(GLuint programId, IRenderSystem& renderSystem, const ScriptCollection& collection) 
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

GfxProgram::~GfxProgram()
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
		StatePolicyGuard::UseProgram(0);
		_current_program = NULL;
	}
}

void GfxProgram::Apply()
{
	if(_current_program != NULL) {
		_current_program->mApplied = false;
	}
	mApplied = true;
	_current_program = this;

	StatePolicyGuard::UseProgram(mProgramId);
	StatePolicyGuard::EnableBlend(mBlend);
	StatePolicyGuard::SetCullFaces(mCullFaces);
	StatePolicyGuard::EnableDepthTest(mDepthTest);
	if(mBlend)
		StatePolicyGuard::SetBlendFunc(mSrcFactor, mDestFactor);
	StatePolicyGuard::SetDepthFunc(mDepthFunc);
	StatePolicyGuard::EnableScissorTest(mScissorTest);
	StatePolicyGuard::SetScissorRect(mScissorRect);
	
	for(int i = 0; i < MaxDrawBuffers; ++i) {
		mRenderSystem.SetRenderTarget(mRenderTargets[i], i);
	}
	mRenderSystem.SetDepthRenderTarget(mDepthRenderTarget);
	mRenderSystem.ApplyRenderTargets();
	mApplyRenderTarget = false;

	ApplyComponents();
}

void GfxProgram::MarkAsDirty()
{
	ComponentMap::iterator it = mComponents.begin();
	ComponentMap::const_iterator end = mComponents.end();
	for(;it != end; ++it) {
		it->second->MarkAsDirty();
	}

	mApplied = false;
}

void GfxProgram::Clear(uint32 clearBits)
{
	GLenum clear = 0;
	if(BIT_ISSET(clearBits, ClearTypes::COLOR)) {
		clear = GL_COLOR_BUFFER_BIT;
	}

	if(BIT_ISSET(clearBits, ClearTypes::DEPTH)) {
		clear |= GL_DEPTH_BUFFER_BIT;
	}

	StatePolicyGuard::SetClearDepth(mClearDepth);
	StatePolicyGuard::SetClearColor(mClearColor);

	if(clear != 0) {
		glClear(clear);
	}
}

void GfxProgram::Prepare(const ScriptCollection& collection)
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

void GfxProgram::ApplyComponents()
{
	ComponentMap::iterator it = mComponents.begin();
	ComponentMap::const_iterator end = mComponents.end();
	for(;it != end; ++it) {
		it->second->Apply();
	}
}

IGfxProgramComponent* GfxProgram::FindComponent(const char* name)
{
	return FindComponent(playstate::string(name));
}

IGfxProgramComponent* GfxProgram::FindComponent(const playstate::string& name)
{
	IGfxProgramComponent* component = NULL;
	ComponentMap::iterator it = mComponents.find(name);
	if(it == mComponents.end()) {
		return &mComponentNotFound;
	}
	return it->second;
}

void GfxProgram::Render(VertexBuffer* buffer)
{
	Render(buffer, NULL, 0);
}

void GfxProgram::Render(VertexBuffer* buffer, IndexBuffer* indexBuffer)
{
	Render(buffer, indexBuffer, 0);
}

void GfxProgram::Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startElement)
{
	assert(_current_program == this && "You are trying to render a vertex and/or index buffer on a non-bound gfx program");
	assert_not_null(buffer);

	ApplyBuffers(buffer, indexBuffer);
		
	if(indexBuffer != NULL)
		indexBuffer->Render(startElement);
	else
		buffer->Render(startElement);
}

void GfxProgram::Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startElement, uint32 numElements)
{
	assert(_current_program == this && "You are trying to render a vertex and/or index buffer on a non-bound gfx program");
	assert_not_null(buffer);
	assert(numElements > 0 && "You cannot draw 0 elements");

	ApplyBuffers(buffer, indexBuffer);
		
	if(indexBuffer != NULL)
		indexBuffer->Render(startElement, numElements);
	else
		buffer->Render(startElement, numElements);
}

void GfxProgram::ApplyBuffers(VertexBuffer* buffer, IndexBuffer* indexBuffer)
{
	if(mApplyRenderTarget) {
		mApplyRenderTarget = false;
		mRenderSystem.ApplyRenderTargets();
	}

	StatePolicyGuard::BindVertexBuffer(buffer);
	StatePolicyGuard::BindIndexBuffer(indexBuffer);
}

void GfxProgram::EnableDepthTest(bool enable)
{
	mDepthTest = enable;
	if(mApplied)
		StatePolicyGuard::EnableDepthTest(enable);
}

void GfxProgram::SetDepthFunc(DepthFunc::Enum depthFunc)
{
	mDepthFunc = depthFunc;
	if(mApplied)
		StatePolicyGuard::SetDepthFunc(depthFunc);
}

void GfxProgram::EnableBlend(bool enable)
{
	mBlend = enable;
	if(mApplied)
		StatePolicyGuard::EnableBlend(enable);
}

void GfxProgram::SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor)
{
	mSrcFactor = sfactor;
	mDestFactor = dfactor;

	if(mApplied)
		StatePolicyGuard::SetBlendFunc(sfactor, dfactor);
}

void GfxProgram::SetClearColor(const Color& color)
{
	mClearColor = color;
	if(mApplied)
		StatePolicyGuard::SetClearColor(color);
}

void GfxProgram::SetCullFaces(CullFaces::Enum cullFaces)
{
	mCullFaces = cullFaces;
	if(mApplied)
		StatePolicyGuard::SetCullFaces(cullFaces);
}

void GfxProgram::EnableScissorTest(bool enable)
{
	mScissorTest = enable;
	if(mApplied)
		StatePolicyGuard::EnableScissorTest(mScissorTest);
}

void GfxProgram::SetScissorRect(const Rect& rect)
{
	mScissorRect = rect;
	if(mApplied)
		StatePolicyGuard::SetScissorRect(mScissorRect);
}

void GfxProgram::SetDepthRenderTarget(RenderTarget2D* renderTarget)
{
	mDepthRenderTarget = renderTarget;

	if(mApplied) {
		mApplyRenderTarget = true;
		mRenderSystem.SetDepthRenderTarget(renderTarget);
	}
}

void GfxProgram::SetRenderTarget(RenderTarget2D* renderTarget, uint32 index)
{
	assert(index < MaxDrawBuffers && "You are not allowed to use that many render targets at once");
	mRenderTargets[index] = renderTarget;

	if(mApplied) {
		mApplyRenderTarget = true;
		mRenderSystem.SetRenderTarget(renderTarget, index);
	}
}

bool GfxProgram::IsApplied() const
{
	return mApplied;
}
