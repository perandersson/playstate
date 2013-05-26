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

	mDiffuseRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA);
	mPositionsRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA16F);
	mNormalsRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGB10_A2);
	mDepthRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, DEPTH24);
	mLightRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA);

	mDeferredShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/effects/deferred/deferred.lua")));
	mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
	mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
	mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
	mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	
	mPointLightTexture = resourceManager.GetResource<Texture2D>("/effects/deferred_point_light/light.png");
	mPointLightShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/effects/deferred/deferred_point_light.lua")));
	mPointLightShader->GetComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mPointLightShader->GetComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mPointLightShader->GetComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mPointLightShader->GetComponent("LightTexture")->SetTexture(mPointLightTexture);
	mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);

	mTexturedShader = std::auto_ptr<GfxProgram>(mRenderSystem.LoadGfxProgram(std::string("/effects/deferred/deferred_result.lua")));
	mTexturedShader->GetComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mTexturedShader->GetComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mTexturedShader->GetComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mTexturedShader->GetComponent("LightTexture")->SetTexture(mLightRenderTarget);
}

DeferredRenderPipeline::~DeferredRenderPipeline()
{
	if(mDiffuseRenderTarget != NULL) {
		delete mDiffuseRenderTarget;
		mDiffuseRenderTarget = NULL;
	}
	
	if(mPositionsRenderTarget != NULL) {
		delete mPositionsRenderTarget;
		mPositionsRenderTarget = NULL;
	}

	if(mNormalsRenderTarget != NULL) {
		delete mNormalsRenderTarget;
		mNormalsRenderTarget = NULL;
	}

	if(mDepthRenderTarget != NULL) {
		delete mDepthRenderTarget;
		mDepthRenderTarget = NULL;
	}

	if(mLightRenderTarget != NULL) {
		delete mLightRenderTarget;
		mLightRenderTarget = NULL;
	}

	mWindow.RemoveWindowResizedListener(this);
}
	
void DeferredRenderPipeline::Render(const Scene* scene, const Camera* camera)
{
	// Draw geometry
	DrawGeometry(scene, camera);
}

void DeferredRenderPipeline::DrawGeometry(const Scene* scene, const Camera* camera)
{
	FindQuery query;
	query.Camera = camera;

	FindResultSet<RenderBlock> resultSet;
	if(scene->Find(query, &resultSet)) {
		mDeferredShader->Apply();
		mDeferredShader->Clear(ClearTypes::COLOR | ClearTypes::DEPTH);

		// Set the cameras projection- and view matrix
		mDeferredShader->GetComponent("ProjectionMatrix")->SetMatrix(camera->ProjectionMatrix);
		mDeferredShader->GetComponent("ViewMatrix")->SetMatrix(camera->ViewMatrix);
		
		IGfxProgramComponent* modelMatrix = mDeferredShader->GetComponent("ModelMatrix");
		IGfxProgramComponent* diffuseTexture = mDeferredShader->GetComponent("DiffuseTexture");
		IGfxProgramComponent* diffuseColor = mDeferredShader->GetComponent("DiffuseColor");
		
		// Draw scene objects
		RenderBlock* block = resultSet.Next();
		GfxProgram* deferredShader = mDeferredShader.get();
		while(block != NULL) {
			diffuseTexture->SetTexture(block->DiffuseTexture);
			diffuseColor->SetColorRGB(block->DiffuseColor);
			modelMatrix->SetMatrix(block->ModelMatrix);
			deferredShader->Render(block->VertexBuffer, block->IndexBuffer);
			block = resultSet.Next();
		}
		
		// Draw lighting to the lighting render target
		//DrawLighting(scene, camera);

		// Draw the final result onto the screen
		DrawFinalResultToScreen(scene, camera);
	}

	CHECK_GL_ERROR();
}

void DeferredRenderPipeline::DrawLighting(const Scene* scene, const Camera* camera)
{
	//std::vector<Light*> lights;
	//if(scene->FindLights(camera, &lights)) {
		mPointLightShader->Apply();
		mPointLightShader->Clear(ClearTypes::COLOR);
/*
		mPointLightShader->GetComponent("ProjectionMatrix")->SetMatrix(camera->ProjectionMatrix);
		mPointLightShader->GetComponent("ViewMatrix")->SetMatrix(camera->ViewMatrix);

		IGfxProgramComponent* modelMatrix = mPointLightShader->GetComponent("ModelMatrix");
		IGfxProgramComponent* lightColor = mPointLightShader->GetComponent("LightColor");
		IGfxProgramComponent* lightPosition = mPointLightShader->GetComponent("LightPosition");
		IGfxProgramComponent* constantAttenuation = mPointLightShader->GetComponent("ConstantAttenuation");
		IGfxProgramComponent* linearAttenuation = mPointLightShader->GetComponent("LinearAttenuation");
		IGfxProgramComponent* quadraticAttenuation = mPointLightShader->GetComponent("QuadraticAttenuation");
		IGfxProgramComponent* lightRadius = mPointLightShader->GetComponent("LightRadius");

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
	}*/
}

Matrix4x4 DeferredRenderPipeline::CalculateBillboardModelMatrix(const Vector3& position, const Camera* camera)
{
	const Vector3 direction = (camera->Position - position).GetNormalized();
	const Vector3 right = camera->Up.CrossProduct(direction);
	const Vector3 up = direction.CrossProduct(right);

	Matrix4x4 mat(right.X, right.Y, right.Z, 0.0f,
		up.X, up.Y, up.Z, 0.0f,
		direction.X, direction.Y, direction.Z, 0.0f,
		position.X, position.Y, position.Z, 1.0f);
	return mat;
}

void DeferredRenderPipeline::DrawFinalResultToScreen(const Scene* scene, const Camera* camera)
{
	mTexturedShader->Apply();
	mTexturedShader->Clear(ClearTypes::COLOR);
	//mTexturedShader->GetComponent("AmbientColor")->SetColorRGB(scene->GetAmbientLight());
	mTexturedShader->GetComponent("AmbientColor")->SetColorRGB(Color(0.f, 0.f, 0.f));
	mTexturedShader->Render(mRenderSystem.UniformVertexBuffer);
}

void DeferredRenderPipeline::OnWindowResized(IWindow& window, uint32 width, uint32 height)
{
	if(mDiffuseRenderTarget != NULL)
		delete mDiffuseRenderTarget;
	
	if(mPositionsRenderTarget != NULL)
		delete mPositionsRenderTarget;

	if(mNormalsRenderTarget != NULL)
		delete mNormalsRenderTarget;

	if(mDepthRenderTarget != NULL)
		delete mDepthRenderTarget;

	if(mLightRenderTarget != NULL)
		delete mLightRenderTarget;

	mDiffuseRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA);
	mPositionsRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA16F);
	mNormalsRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGB10_A2);
	mDepthRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, DEPTH24);
	mLightRenderTarget = mRenderSystem.CreateRenderTarget2D(width, height, RGBA);

	mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
	mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
	mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
	mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	
	mPointLightShader->GetComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mPointLightShader->GetComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mPointLightShader->GetComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);

	mTexturedShader->GetComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mTexturedShader->GetComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mTexturedShader->GetComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mTexturedShader->GetComponent("LightTexture")->SetTexture(mLightRenderTarget);
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
