#include <playstate/memory/memory.h>
#include "deferred_render_pipeline.h"
#include <playstate/resources/resource_manager.h>
#include <playstate/camera/camera.h>
#include <playstate/scene/scene.h>
#include <playstate/window/window.h>
#include <playstate/component/lightsources/point_light.h>

DeferredRenderPipeline::DeferredRenderPipeline(IRenderSystem& renderSystem, IWindow& window, IResourceManager& resourceManager, IFileSystem& fileSystem)
	: mRenderSystem(renderSystem), mWindow(window), mFileSystem(fileSystem),
	mDiffuseRenderTarget(NULL), mPositionsRenderTarget(NULL), mNormalsRenderTarget(NULL), mDepthRenderTarget(NULL),
	mLightRenderTarget(NULL), mGeometryBuilder(renderSystem)
{
	mWindow.AddWindowResizedListener(this);
	
	const Size& windowSize = window.GetSize();

	mDiffuseRenderTarget = mRenderSystem.CreateRenderTarget2D(windowSize, TextureFormat::RGBA);
	mPositionsRenderTarget = mRenderSystem.CreateRenderTarget2D(windowSize, TextureFormat::RGBA16F);
	mNormalsRenderTarget = mRenderSystem.CreateRenderTarget2D(windowSize, TextureFormat::RGB10_A2);
	mDepthRenderTarget = mRenderSystem.CreateRenderTarget2D(windowSize, TextureFormat::DEPTH24);
	mLightRenderTarget = mRenderSystem.CreateRenderTarget2D(windowSize, TextureFormat::RGBA);

	mDeferredShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(playstate::string("/demo/effects/deferred/deferred.lua")));
	mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
	mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
	mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
	mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	
	mPointLightTexture = resourceManager.GetResource<ITexture2D>("/demo/effects/deferred/light.png");
	mPointLightShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(playstate::string("/demo/effects/deferred/deferred_point_light.lua")));
	mPointLightShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mPointLightShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mPointLightShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);
	
	mTexturedShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(playstate::string("/demo/effects/deferred/deferred_result.lua")));
	mTexturedShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
	mTexturedShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
	mTexturedShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
	mTexturedShader->FindComponent("LightTexture")->SetTexture(mLightRenderTarget);

	mUserInterfaceShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(playstate::string("/demo/effects/gui/gui.lua")));
	//mUserInterfaceShader->EnableScissorTest(true);
	//mUserInterfaceShader->SetScissorRect(Rect(0, 0, windowSize.X, windowSize.Y));
	mWhiteTexture = resourceManager.GetResource<ITexture2D>("/engine/textures/white.png");

	mFileSystem.AddFileChangedListener(playstate::string("/demo/effects/deferred/deferred.lua"), this);
	mFileSystem.AddFileChangedListener(playstate::string("/demo/effects/deferred/deferred_point_light.lua"), this);
	mFileSystem.AddFileChangedListener(playstate::string("/demo/effects/deferred/deferred_result.lua"), this);
	mFileSystem.AddFileChangedListener(playstate::string("/demo/effects/gui/gui.lua"), this);
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
	mFileSystem.RemoveFileChangedListener(this);
}
	
void DeferredRenderPipeline::Render(Scene& scene, Canvas& canvas, const Camera& camera)
{
	// Draw geometry
	DrawGeometry(scene, camera);

	// Draw GUI
	DrawUserInterface(canvas);
}

void DeferredRenderPipeline::DrawUserInterface(Canvas& canvas)
{
	if(canvas.ProcessCanvas(mGeometryBuilder)) {
		mUserInterfaceShader->Apply();
		mUserInterfaceShader->FindComponent("ProjectionMatrix")->SetMatrix(canvas.GetProjectionMatrix());

		IGfxProgramComponent* textureComponent = mUserInterfaceShader->FindComponent("Texture");
		textureComponent->SetTexture(mWhiteTexture);

		WidgetBuildingBlock* blocks = mGeometryBuilder.GetBuildingBlocks();
		uint32 numBlocks = mGeometryBuilder.GetNumBuildingBlocks();
		for(uint32 i = 0; i < numBlocks; ++i) {
			WidgetBuildingBlock& block = blocks[i];
			if(block.Texture == NULL)
				textureComponent->SetTexture(mWhiteTexture);
			else
				textureComponent->SetTexture(block.Texture);
			mUserInterfaceShader->Render(block.IVertexBuffer, NULL, block.StartIndex, block.NumVertices);	
		}
	}
	mGeometryBuilder.Reset();
}

void DeferredRenderPipeline::DrawGeometry(Scene& scene, const Camera& camera)
{
	FindQuery query;
	query.Camera = &camera;
	query.Filter = RenderStateFilter::GEOMETRY | RenderStateFilter::TEXTURES;

	if(scene.Find(query, &mRenderBlockResultSet)) {
		mDeferredShader->Apply();
		mDeferredShader->Clear(ClearTypes::COLOR | ClearTypes::DEPTH);

		// Set the cameras projection- and view matrix
		mDeferredShader->FindComponent("ProjectionMatrix")->SetMatrix(camera.GetProjectionMatrix());
		mDeferredShader->FindComponent("ViewMatrix")->SetMatrix(camera.GetViewMatrix());
		
		IGfxProgramComponent* modelMatrix = mDeferredShader->FindComponent("ModelMatrix");
		IGfxProgramComponent* diffuseTexture = mDeferredShader->FindComponent("DiffuseTexture");
		IGfxProgramComponent* diffuseColor = mDeferredShader->FindComponent("DiffuseColor");
		
		IGfxProgram* deferredShader = mDeferredShader.get();

		// Draw scene objects
		ResultSetIterator<RenderBlock> it(mRenderBlockResultSet);
		ResultSetIterator<RenderBlock>::Type block;
		while(block = it.Next()) {
			diffuseTexture->SetTexture(block->DiffuseTexture);
			diffuseColor->SetColorRGB(block->DiffuseColor);
			modelMatrix->SetMatrix(block->ModelMatrix);
			deferredShader->Render(block->VertexBuffer, block->IndexBuffer);
		}

		// Draw lighting to the lighting render target
		DrawLighting(scene, camera);

		// Draw the final result onto the screen
		DrawFinalResultToScreen(scene, camera);
	} else {
		mTexturedShader->Apply();
		mTexturedShader->Clear(ClearTypes::COLOR | ClearTypes::DEPTH);
	}
}

void DeferredRenderPipeline::DrawLighting(Scene& scene, const Camera& camera)
{
	FindQuery query;
	query.Camera = &camera;
	if(scene.Find(query, &mLightSourceResultSet)) {
		mPointLightShader->Apply();
		mPointLightShader->Clear(ClearTypes::COLOR);

		mPointLightShader->FindComponent("ProjectionMatrix")->SetMatrix(camera.GetProjectionMatrix());
		mPointLightShader->FindComponent("ViewMatrix")->SetMatrix(camera.GetViewMatrix());
		mPointLightShader->FindComponent("LightTexture")->SetTexture(mPointLightTexture);
		
		IGfxProgramComponent* modelMatrix = mPointLightShader->FindComponent("ModelMatrix");
		IGfxProgramComponent* lightColor = mPointLightShader->FindComponent("LightColor");
		IGfxProgramComponent* lightPosition = mPointLightShader->FindComponent("LightPosition");
		IGfxProgramComponent* constantAttenuation = mPointLightShader->FindComponent("ConstantAttenuation");
		IGfxProgramComponent* linearAttenuation = mPointLightShader->FindComponent("LinearAttenuation");
		IGfxProgramComponent* quadraticAttenuation = mPointLightShader->FindComponent("QuadraticAttenuation");
		IGfxProgramComponent* lightRadius = mPointLightShader->FindComponent("LightRadius");

		ResultSetIterator<LightSource> it(mLightSourceResultSet);
		ResultSetIterator<LightSource>::Type lightSource;
		while(lightSource = it.Next()) {
			PointLight* pl = dynamic_cast<PointLight*>(lightSource);
			if(pl != NULL)   {
				// TODO Render point lights as six spot-lights with texture "LightTexture" that's specified above.
				modelMatrix->SetMatrix(CalculateBillboardModelMatrix(pl->GetNode()->GetAbsolutePosition(), camera));
				
				lightColor->SetColorRGB(pl->GetColor());
				lightPosition->SetVector3(pl->GetNode()->GetAbsolutePosition());
				constantAttenuation->SetFloat(pl->GetConstantAttenuation());
				linearAttenuation->SetFloat(pl->GetLinearAttenuation());
				quadraticAttenuation->SetFloat(pl->GetQuadricAttenuation());
				lightRadius->SetFloat(pl->GetRadius());

				mPointLightShader->Render(mRenderSystem.GetUniformVertexBuffer());
			}
		}
	}
}

Matrix4x4 DeferredRenderPipeline::CalculateBillboardModelMatrix(const Vector3& position, const Camera& camera)
{
	const Vector3 direction = (camera.GetPosition() - position).GetNormalized();
	const Vector3 right = camera.GetUp().CrossProduct(direction);
	const Vector3 up = direction.CrossProduct(right);

	Matrix4x4 mat(right.X, right.Y, right.Z, 0.0f,
		up.X, up.Y, up.Z, 0.0f,
		direction.X, direction.Y, direction.Z, 0.0f,
		position.X, position.Y, position.Z, 1.0f);
	return mat;
}

void DeferredRenderPipeline::DrawFinalResultToScreen(Scene& scene, const Camera& camera)
{
	mTexturedShader->Apply();
	mTexturedShader->Clear(ClearTypes::COLOR | ClearTypes::DEPTH);
	mTexturedShader->FindComponent("AmbientColor")->SetColorRGB(scene.GetAmbientLight());
	mTexturedShader->FindComponent("ProjectionMatrix")->SetMatrix(Camera::GetOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f));
	mTexturedShader->Render(mRenderSystem.GetUniformVertexBuffer());
}

void DeferredRenderPipeline::OnWindowResized(const Size& newSize)
{
	delete mDiffuseRenderTarget;
	delete mPositionsRenderTarget;
	delete mNormalsRenderTarget;
	delete mDepthRenderTarget;
	delete mLightRenderTarget;

	mDiffuseRenderTarget = mRenderSystem.CreateRenderTarget2D(newSize, TextureFormat::RGBA);
	mPositionsRenderTarget = mRenderSystem.CreateRenderTarget2D(newSize, TextureFormat::RGBA16F);
	mNormalsRenderTarget = mRenderSystem.CreateRenderTarget2D(newSize, TextureFormat::RGB10_A2);
	mDepthRenderTarget = mRenderSystem.CreateRenderTarget2D(newSize, TextureFormat::DEPTH24);
	mLightRenderTarget = mRenderSystem.CreateRenderTarget2D(newSize, TextureFormat::RGBA);

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

void DeferredRenderPipeline::FileChanged(const IFile& file, FileChangeAction::Enum action)
{
	const playstate::string deferred = "/demo/effects/deferred/deferred.lua";
	const playstate::string deferred_point_light = "/demo/effects/deferred/deferred_point_light.lua";
	const playstate::string deferred_result = "/demo/effects/deferred/deferred_result.lua";
	const playstate::string gui = "/demo/effects/gui/gui.lua";

	// Update shaders when changing file

	if(file.GetPath() == deferred) {
		mDeferredShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(deferred));
		mDeferredShader->SetRenderTarget(mDiffuseRenderTarget, 0);
		mDeferredShader->SetRenderTarget(mPositionsRenderTarget, 1);
		mDeferredShader->SetRenderTarget(mNormalsRenderTarget, 2);
		mDeferredShader->SetDepthRenderTarget(mDepthRenderTarget);
	} else if(file.GetPath() == deferred_point_light) {
		mPointLightShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(deferred_point_light));
		mPointLightShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
		mPointLightShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
		mPointLightShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
		mPointLightShader->SetRenderTarget(mLightRenderTarget, 0);
	} else if(file.GetPath() == deferred_result) {
		mTexturedShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(deferred_result));
		mTexturedShader->FindComponent("DiffuseTexture")->SetTexture(mDiffuseRenderTarget);
		mTexturedShader->FindComponent("PositionsTexture")->SetTexture(mPositionsRenderTarget);
		mTexturedShader->FindComponent("NormalsTexture")->SetTexture(mNormalsRenderTarget);
		mTexturedShader->FindComponent("LightTexture")->SetTexture(mLightRenderTarget);
	} else if(file.GetPath() == gui) {
		mUserInterfaceShader = std::auto_ptr<IGfxProgram>(mRenderSystem.LoadGfxProgram(gui));
	}
}

class ScriptableDeferredRenderPipeline : public DeferredRenderPipeline, public Scriptable
{
public:
	ScriptableDeferredRenderPipeline(IRenderSystem& renderSystem, IWindow& window, IResourceManager& resourceManager, IFileSystem& fileSystem) 
		: DeferredRenderPipeline(renderSystem, window, resourceManager, fileSystem) {
	}
};

int DeferredRenderPipeline_Factory(lua_State* L)
{
	int top1 = lua_gettop(L);
	ScriptableDeferredRenderPipeline* pipeline = new ScriptableDeferredRenderPipeline(IRenderSystem::Get(), IWindow::Get(), IResourceManager::Get(), IFileSystem::Get());
	int top2 = lua_gettop(L);
	luaM_pushobject(L, "DeferredRenderPipeline", pipeline);
	int top3 = lua_gettop(L);
	return 1;
}
