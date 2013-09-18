#include "../memory/memory.h"
#include "canvas.h"
#include "../game/game_runner.h"
#include "../camera/camera.h"
using namespace playstate;

Canvas::Canvas(IWindow& window, IInputSystem& inputSystem, std::auto_ptr<IUpdateProcessor> updateProcessor)
	: mWindow(window), mInputSystem(inputSystem), mGroups(offsetof(CanvasGroup, CanvasGroupLink)), mUpdateProcessor(updateProcessor),
	mLastButtonDown(MouseButtons::NO_BUTTON), mButtonClicked(MouseButtons::NO_BUTTON)
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

CanvasGroup* Canvas::GetCanvasGroupByName(const playstate::string& name)
{
	CanvasGroup* group = mGroups.First();
	while(group != NULL) {
		if(group->GetName() == name) {
			return group;
		}
		group = group->CanvasGroupLink.Tail;
	}
	return group;
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
	IMouseState& state = mInputSystem.GetMouseState();
	if(state.IsDown(MouseButtons::LEFT))
		mLastButtonDown = MouseButtons::LEFT;
	if(state.IsDown(MouseButtons::MIDDLE))
		mLastButtonDown = MouseButtons::MIDDLE;
	if(state.IsDown(MouseButtons::RIGHT))
		mLastButtonDown = MouseButtons::RIGHT;
	
	mButtonClicked = MouseButtons::NO_BUTTON;
	if(state.IsUp(mLastButtonDown)) {
		mButtonClicked = mLastButtonDown;
		mLastButtonDown = MouseButtons::NO_BUTTON;
	}
}

bool Canvas::ProcessCanvas(GuiGeometryBuilder& builder)
{
	CanvasGroup* group = mGroups.First();
	while(group != NULL) {
		CanvasGroup* next = group->CanvasGroupLink.Tail;
		group->ProcessCanvas(&builder);
		group = next;
	}
	return true;
}

Vector2 Canvas::GetMousePositionAsUniform() const
{
	Size size = mWindow.GetSize();
	Point pt = mInputSystem.GetMouseState().GetPosition();
	return Vector2(pt.X / (float32)size.Width, pt.Y / (float32)size.Height);
}

Point Canvas::GetMousePosition() const
{
	const Size size = mWindow.GetSize();
	const Vector2 diff(mSize.Width / (float32)size.Width, mSize.Height / (float32)size.Height);
	Point pt = mInputSystem.GetMouseState().GetPosition();
	return Point(pt.X * diff.X, pt.Y * diff.Y);
}

MouseButtons::Enum Canvas::GetMouseClick() const
{
	return mButtonClicked;
}

MouseButtons::Enum Canvas::GetMouseDown() const
{
	return mLastButtonDown;
}

void Canvas::SetSize(const Size& size)
{
	mSize = size;
	mProjectionMatrix = Camera::GetOrtho2D(0.f, size.Width, size.Height, 0.f);
}

const Size& Canvas::GetSize() const
{
	return mSize;
}

const Matrix4x4& Canvas::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}

int playstate::Canvas_AddCanvasGroup(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 1) {
		luaM_printerror(L, "Expected: Canvas.AddCanvasGroup(group : CanvasGroup)");
		return 0;
	}

	CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
	if(group != NULL)
		GameRunner::Get().GetCanvas().AddCanvasGroup(group);
	else {
		luaM_printerror(L, "Expected: Canvas.AddCanvasGroup(group : CanvasGroup)");
	}

	return 0;
}

int playstate::Canvas_RemoveCanvasGroup(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 1) {
		luaM_printerror(L, "Expected: Canvas.RemoveCanvasGroup(group : CanvasGroup)");
		return 0;
	}

	CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
	if(group != NULL)
		GameRunner::Get().GetCanvas().RemoveCanvasGroup(group);
	else
		luaM_printerror(L, "Expected: Canvas.RemoveCanvasGroup(group : CanvasGroup)");

	return 0;
}

int playstate::Canvas_SetSize(lua_State* L)
{
	Size size = luaM_poppoint(L);
	GameRunner::Get().GetCanvas().SetSize(size);
	return 0;
}

int playstate::Canvas_Show(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 1) {
		luaM_printerror(L, "Expected: Canvas.Show(name : String)");
		return 0;
	}

	const playstate::string name = lua_tostring(L, -1); lua_pop(L, 1);
	Canvas& canvas = GameRunner::Get().GetCanvas();
	CanvasGroup* group = canvas.GetCanvasGroupByName(name);
	if(group != NULL) {
		//group->Show();
	} else {
		luaM_printerror(L, "Could not find a CanvasGroup by the name of: '%s'", name.c_str());
	}
	assert_not_implemented();
	return 0;
}
