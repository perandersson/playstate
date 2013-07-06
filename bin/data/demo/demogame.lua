local class = require "engine.class"

DemoGame = class(IGame, function(self) 
	IGame.__init(self)
	
	-- Local variables here
	-- self.something = true
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
	ActiveCamera.LookAt(Vector3(0, 50.0, 50.0), Vector3(0, 0, 0), Vector3(0, 1, 0))
	
	-- Start a new level (in it's own scipt scope, which prevents the game from crashing
	-- if the lua script is invalid
	Game.StartLevel("/demo/levels/level1.lua")
end

function DemoGame:UnloadContent()
	print("Unloading DemoGame content")
end

return DemoGame
