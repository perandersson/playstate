local class = require "engine.class"
local config = require "config"

DemoGame = class("DemoGame", IGame)

function DemoGame:__init()
	IGame.__init(self)
	
	self.numFrames = 0
	self.totalTime = 0
end

function DemoGame:Initialize()
	print("Initializes DemoGame")
	Game.SetRenderPipeline(DeferredRenderPipeline())
	
	-- Set the canvas size. This is needed so that the attached widgets get
	-- the correct uniform size
	Canvas.SetSize(1024, 768)
	
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
	
	-- Add the player group
	local playerGroup = SceneGroup.Load("/demo/levels/playergroup.lua")
	Scene.AddSceneGroup(playerGroup)

	local timer = Timer.Start()
	-- Start the first level
	local level1 = SceneGroup.Load("/demo/levels/level1.lua")
	Scene.AddSceneGroup(level1)
	print("Load time: " .. Timer.End(timer) .. " seconds")

	-- Load the main menu (this will start the first level later)
	local mainMenu = CanvasGroup.Load("/demo/menu/mainmenu.lua")
	Canvas.AddCanvasGroup(mainMenu)
end

function DemoGame:Update()
	self.totalTime = self.totalTime + GameDeltaTime
	self.numFrames = self.numFrames + 1
	if self.totalTime > 1.0 then
		Window.SetTitle(config.window.title .. self.numFrames)
		self.totalTime = 1.0 - self.totalTime
		self.numFrames = 0
	end
	
	--local x,y = Mouse.GetPosition()
	--print("x,y = " .. x .. "," .. y)
end

function DemoGame:UnloadContent()
	print("Unloading DemoGame content")
end

return DemoGame
