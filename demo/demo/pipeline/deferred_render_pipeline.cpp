#include <playstate/memory/memory.h>
#include "deferred_render_pipeline.h"
#include <playstate/resources/resource_manager.h>
#include <playstate/camera/camera.h>
#include <playstate/scene/scene.h>
#include <playstate/window/window.h>
//#include "../../renderable.h"
//#include "../../light/point_light.h"
//#include "../../scene/tree/octree.h"

DeferredRenderPipeline::DeferredRenderPipeline(RenderSystem& renderSystem, IWindow& window, ResourceManager& resourceManager)
	: mRenderSystem(renderSystem), mWindow(window), 
	mDiffuseRenderTarget(NULL), mPositionsRenderTarget(NULL), mNormalsRenderTarget(NULL), mDepthRenderTarget(NULL),
	mLightRenderTarget(NULL)
{
	mWindow.AddWindowResizedListener(this);
	
	uint32 width = window.GetWidth();
	uint32 height = window.GetHeight();

	mDiffuseRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA);
	mPositionsRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA16F);
	mNormalsRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGB10_A2);
	mDepthRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::DEPTH24);
	mLightRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA);

	mDeferredShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/demo/effects/deferred/deferred.lua")));
	mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
	mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
	mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
	mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	
	mPointLightTexture = resourceManager.GetResource<Texture2D>("/demo/effects/deferred_point_light/light.png");
	mPointLightShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/demo/effects/deferred/deferred_point_light.lua")));
	mPointLightShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mPointLightShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mPointLightShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mPointLightShader->FindComponent("LightTexture")->SetTexture(mPointLightTexture);
	mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);

	mTexturedShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/demo/effects/deferred/deferred_result.lua")));
	mTexturedShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mTexturedShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mTexturedShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mTexturedShader->FindComponent("LightTexture")->SetTexture(mLightRenderTarget);
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
	delete mDiffuseRenderTarget;
	mDiffuseRenderTarget = NULL;
	
	delete mPositionsRenderTarget;
	mPositionsRenderTarget = NULL;

	delete mNormalsRenderTarget;
	mNormalsRenderTarget = NULL;

	delete mDepthRenderTarget;
	mDepthRenderTarget = NULL;

	delete mLightRenderTarget;
	mLightRenderTarget = NULL;

	mWindow.RemoveWindowResizedListener(this);
}
	
void DeferredRenderPipeline::Render(const Scene& scene, const Camera& camera)
{
	// Draw geometry
	DrawGeometry(scene, camera);
}

void DeferredRenderPipeline::DrawGeometry(const Scene& scene, const Camera& camera)
{
	FindQuery query;
	query.Camera = &camera;
	if(scene.Find(query, &mRenderBlockResultSet)) {
		mDeferredShader->Apply();
		mDeferredShader->Clear(ClearTypes::COLOR | ClearTypes::DEPTH);

		// Set the cameras projection- and view matrix
		mDeferredShader->FindComponent("ProjectionMatrix")->SetMatrix(camera.ProjectionMatrix);
		mDeferredShader->FindComponent("ViewMatrix")->SetMatrix(camera.ViewMatrix);
		
		IGfxProgramComponent* modelMatrix = mDeferredShader->FindComponent("ModelMatrix");
		IGfxProgramComponent* diffuseTexture = mDeferredShader->FindComponent("DiffuseTexture");
		IGfxProgramComponent* diffuseColor = mDeferredShader->FindComponent("DiffuseColor");
		
		// Draw scene objects
		GfxProgram* deferredShader = mDeferredShader.get();
		for(uint32 index = 0; index < mRenderBlockResultSet.Size; ++index) {
			RenderBlock* block = mRenderBlockResultSet.RenderBlocks[index];
			diffuseTexture->SetTexture(block->DiffuseTexture);
			diffuseColor->SetColorRGB(block->DiffuseColor);
			modelMatrix->SetMatrix(block->ModelMatrix);
			deferredShader->Render(block->VertexBuffer, block->IndexBuffer);
		}
		
		// Draw lighting to the lighting render target
		DrawLighting(scene, camera);

		// Draw the final result onto the screen
		DrawFinalResultToScreen(scene, camera);
	}
	mRenderBlockResultSet.Reset();
	CHECK_GL_ERROR();
}

void DeferredRenderPipeline::DrawLighting(const Scene& scene, const Camera& camera)
{

	// Get lights that affect drawable geometry?
	// 
	/*


	FindQuery query;
	query.Camera = &camera;
	if(scene.Find(query, mLightSourceResultSet)) {
		mPointLightShader->Apply();
		mPointLightShader->Clear(ClearTypes::COLOR);

		mPointLightShader->GetComponent("ProjectionMatrix")->SetMatrix(camera.ProjectionMatrix);
		mPointLightShader->GetComponent("ViewMatrix")->SetMatrix(camera.ViewMatrix);

		IGfxProgramComponent* modelMatrix = mPointLightShader->GetComponent("ModelMatrix");
		IGfxProgramComponent* lightColor = mPointLightShader->GetComponent("LightColor");
		IGfxProgramComponent* lightPosition = mPointLightShader->GetComponent("LightPosition");
		IGfxProgramComponent* constantAttenuation = mPointLightShader->GetComponent("ConstantAttenuation");
		IGfxProgramComponent* linearAttenuation = mPointLightShader->GetComponent("LinearAttenuation");
		IGfxProgramComponent* quadraticAttenuation = mPointLightShader->GetComponent("QuadraticAttenuation");
		IGfxProgramComponent* lightRadius = mPointLightShader->GetComponent("LightRadius");

		for(uint32 index = 0; index < mLightSourceResultSet.Size; ++index) {
			RenderBlock* block = mRenderBlockResultSet.RenderBlocks[index];
			diffuseTexture->SetTexture(block->DiffuseTexture);
			diffuseColor->SetColorRGB(block->DiffuseColor);
			modelMatrix->SetMatrix(block->ModelMatrix);
			deferredShader->Render(block->VertexBuffer, block->IndexBuffer);
		}

		// Draw the lights
		// TODO Check shader version so that we can decide what technique should be used
		// to render the light
		std::vector<Light*>::size_type size = lights.size();
		for(std::vector<Light*>::size_type i = 0; i < size; ++i) {
			const PointLight* pl = dynamic_cast<const PointLight*>(lights[i]);
			if(pl != NULL)   {
				// TODO Render point lights as six spot-lights with texture "LightTexture" that's specified above.
				modelMatrix->SetMatrix(CalculateBillboardModelMatrix(pl->AbsolutePosition, camera));
				
				lightColor->SetColorRGB(pl->LightColor);
				lightPosition->SetVector3(pl->AbsolutePosition);
				constantAttenuation->SetFloat(pl->ConstantAttenuation);
				linearAttenuation->SetFloat(pl->LinearAttenuation);
				quadraticAttenuation->SetFloat(pl->QuadricAttenuation);
				lightRadius->SetFloat(pl->Radius);

				mPointLightShader->Render(mQuadBufferObject);
				continue;
			}

			const SpotLight* spl = dynamic_cast<const SpotLight*>(lights[i]);
			if(spl != NULL) {
				continue;
			}
		}
	}
	*/
	
	CHECK_GL_ERROR();
}

Matrix4x4 DeferredRenderPipeline::CalculateBillboardModelMatrix(const Vector3& position, const Camera& camera)
{
	const Vector3 direction = (camera.Position - position).GetNormalized();
	const Vector3 right = camera.Up.CrossProduct(direction);
	const Vector3 up = direction.CrossProduct(right);

	Matrix4x4 mat(right.X, right.Y, right.Z, 0.0f,
		up.X, up.Y, up.Z, 0.0f,
		direction.X, direction.Y, direction.Z, 0.0f,
		position.X, position.Y, position.Z, 1.0f);
	return mat;
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene& scene, const Camera& camera)
{
	mTexturedShader->Apply();
	mTexturedShader->Clear(ClearTypes::COLOR);
	mTexturedShader->FindComponent("AmbientColor")->SetColorRGB(scene.AmbientLight);
	mTexturedShader->Render(mRenderSystem.UniformVertexBuffer);
}

void DeferredRenderPipeline::OnWindowResized(IWindow& window, uint32 width, uint32 height)
{
	delete mDiffuseRenderTarget;
	delete mPositionsRenderTarget;
	delete mNormalsRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mDiffuseRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA);
	mPositionsRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA16F);
	mNormalsRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGB10_A2);
	mDepthRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::DEPTH24);
	mLightRenderTarget = RenderTarget2D::Create(width, height, TextureFormat::RGBA);

	mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
	mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
	mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
	mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	
	mPointLightShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mPointLightShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mPointLightShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);

	mTexturedShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mTexturedShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mTexturedShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mTexturedShader->FindComponent("LightTexture")->SetTexture(mLightRenderTarget);
}

int DeferredRenderPipeline_Factory(lua_State* L)
{
	int top1 = lua_gettop(L);
	DeferredRenderPipeline* pipeline = new DeferredRenderPipeline(RenderSystem::Get(), IWindow::Get(), ResourceManager::Get());
	int top2 = lua_gettop(L);
	luaM_pushobject(L, "DeferredRenderPipeline", pipeline);
	int top3 = lua_gettop(L);
	return 1;
}
