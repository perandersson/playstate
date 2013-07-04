local class = require "engine.class"

DemoGame = class(IGame, function(self) 
	IGame.__init(self)
	
	-- Local variables here
	-- self.something = true
end)

function DemoGame:Initialize()
	print("Init")
	Game.SetRenderPipeline(DeferredRenderPipeline())
	print("Init2")
	return true
end

function DemoGame:LoadContent()
	print("Loading content")
	
	-- Set the actives cmaeras perspective
	local ratio = Window.GetWidth() / Window.GetHeight()
	ActiveCamera.SetPerspective(0.1, 1000.0, 45.0, ratio)
	ActiveCamera.LookAt(Vector3(0, 50.0, 50.0), Vector3(0, 0, 0), Vector3(0, 1, 0))
	
	local level1 = Game.LoadLevel("/demo/levels/level1.lua")
	Scene.AddSceneGroup(level1)
end

function DemoGame:UnloadContent()
	print("Unloading content")
end

return DemoGame
