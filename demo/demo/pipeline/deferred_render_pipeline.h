#pragma once

#include <playstate/rendering/render_system.h>
#include <playstate/rendering/render_pipeline.h>
#include <playstate/window/window_resized_listener.h>
#include <playstate/rendering/gfx_program.h>
#include <playstate/search/render_block_result_set.h>
#include <playstate/search/light_source_result_set.h>
using namespace playstate;

//
// Renders the supplied scene using a deferred render pipeline
class DeferredRenderPipeline : public IRenderPipeline, public IWindowResizedListener
{
public:
	DeferredRenderPipeline(RenderSystem& renderSystem, IWindow& window, ResourceManager& resourceManager);
	virtual ~DeferredRenderPipeline();

// IRenderPipeline
public:
	virtual void Render(const Scene& scene, const Camera& camera);

// IWindowResizeListener
public:
	virtual void OnWindowResized(uint32 width, uint32 height);
		
private:
	void DrawGeometry(const Scene& scene, const Camera& camera);
	void DrawLighting(const Scene& scene, const Camera& camera);
	void DrawFinalResultToScreen(const Scene& scene, const Camera& camera);
	void DrawUserInterface(const Scene& scene);

	Matrix4x4 CalculateBillboardModelMatrix(const Vector3& position, const Camera& camera);
		
private:
	RenderSystem& mRenderSystem;
	IWindow& mWindow;

	std::auto_ptr<GfxProgram> mDeferredShader;
	std::auto_ptr<GfxProgram> mTexturedShader;
	std::auto_ptr<GfxProgram> mPointLightShader;
	std::auto_ptr<GfxProgram> mUserInterfaceShader;
	Resource<Texture2D> mPointLightTexture;
	Resource<Texture2D> mWhiteTexture;

	RenderTarget2D* mDiffuseRenderTarget;
	RenderTarget2D* mPositionsRenderTarget;
	RenderTarget2D* mNormalsRenderTarget;
	RenderTarget2D* mDepthRenderTarget;

	RenderTarget2D* mLightRenderTarget;
	RenderBlockResultSet mRenderBlockResultSet;
	LightSourceResultSet mLightSourceResultSet;
};


//
// Script integration
//

extern int DeferredRenderPipeline_Factory(lua_State* L);
static luaL_Reg DeferredRenderPipeline_Methods[] = {
	{ LUA_CONSTRUCTOR, DeferredRenderPipeline_Factory },
	{ NULL, NULL }
};