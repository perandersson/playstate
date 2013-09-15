#include "../memory/memory.h"
#include "canvas.h"
#include "../game/game_runner.h"
#include "../camera/camera.h"
using namespace playstate;

Canvas::Canvas(IWindow& window, IInputSystem& inputSystem, std::auto_ptr<IUpdateProcessor> updateProcessor)
	: mWindow(window), mInputSystem(inputSystem), mGroups(offsetof(CanvasGroup, CanvasGroupLink)), mUpdateProcessor(updateProcessor)
{
}



Canvas::~Canvas()
{
	mGroups.DeleteAll();
}

void Canvas::AddCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	mGroups.AddLast(group);
	group->AddedToCanvas(this);
}

void Canvas::RemoveCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	group->RemovingFromCanvas(this);
	mGroups.Remove(group);
}

void Canvas::AttachUpdatable(IUpdatable* updatable)
{
	mUpdateProcessor->AttachUpdatable(updatable);
}

void Canvas::DetachUpdatable(IUpdatable* updatable)
{
	mUpdateProcessor->DetachUpdatable(updatable);
}

void Canvas::AttachTickable(ITickable* tickable)
{
	mUpdateProcessor->AttachTickable(tickable);
}

void Canvas::DetachTickable(ITickable* tickable)
{
	mUpdateProcessor->DetachTickable(tickable);
}

void Canvas::Update()
{
	mUpdateProcessor->Update();
}

bool Canvas::ProcessCanvas(GuiGeometryBuilder& builder)
{
	CanvasGroup* group = mGroups.First();
	while(group != NULL) {
		group->ProcessCanvas(&builder);
		group = group->CanvasGroupLink.Tail;
	}
	return true;
}

Vector2 Canvas::GetMousePositionAsUniform() const
{
	Point size = mWindow.GetSize();
	Point pt = mInputSystem.GetMouseState().GetPosition();
	return Vector2(pt.X / (float32)size.Width, pt.Y / (float32)size.Height);
}

void Canvas::SetSize(const Vector2& size)
{
	mSize = size;
	mProjectionMatrix = Camera::GetOrtho2D(0.f, size.X, size.Y, 0.f);
}

const Vector2& Canvas::GetSize() const
{
	return mSize;
}

const Matrix4x4& Canvas::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

int playstate::Canvas_AddCanvasGroup(lua_State* L)
{
	CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
	if(group != NULL)
		GameRunner::Get().GetCanvas().AddCanvasGroup(group);
	else
		ILogger::Get().Error("Cannot add a CanvasGroup that's nil");

	return 0;
}

int playstate::Canvas_RemoveCanvasGroup(lua_State* L)
{
	CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
	if(group != NULL)
		GameRunner::Get().GetCanvas().RemoveCanvasGroup(group);
	else
		ILogger::Get().Error("Cannot remove a CanvasGroup that's nil");

	return 0;
}

int playstate::Canvas_SetSize(lua_State* L)
{
	Vector2 size = luaM_popvector2(L);
	GameRunner::Get().GetCanvas().SetSize(size);
	return 0;
}
