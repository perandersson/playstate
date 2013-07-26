local class = require "engine.class"
local config = require "config"

DemoGame = class(IGame, function(self) 
	IGame.__init(self)
end)

function DemoGame:Initialize()
	print("Initializes DemoGame")
	Game.SetRenderPipeline(DeferredRenderPipeline())
	return true
end

function DemoGame:LoadContent()
	print("Loading DemoGame content")
	
	-- Set the actives cmaeras perspective
	local ratio = Window.GetWidth() / Window.GetHeight()
	ActiveCamera.SetPerspective(0.1, 1000.0, 45.0, ratio)
	ActiveCamera.LookAt(config.camera.eye, config.camera.center, config.camera.up)
	
	-- Set ambient lighting on the scene.
	Scene.SetAmbientLight({0.1, 0.1, 0.1})
	
	-- Start the first level
	Game.StartLevel("/demo/levels/level1.lua")
end

function DemoGame:UnloadContent()
	print("Unloading DemoGame content")
end

return DemoGame
