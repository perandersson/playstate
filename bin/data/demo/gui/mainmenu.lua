local class = require "engine.class"
local Rect = require "engine.core.math.rect"

MainMenu = class("MainMenu", CanvasGroup)
function MainMenu:__init()
	CanvasGroup.__init(self)
	self:SetName("MainMenu")
	self:SetStyle(require("demo.gui.style"))
	self.enabled = false
	self.sliderValue = 0.5
	
	self.selectedIndex = 0
	self.dropdownValues = { "Value1", "Value2", "Value3" }
end

function MainMenu:OnProcessCanvas()
	local width = 1024 - 200
	local height = 768 - 200
	self:BeginFrame(Rect(100, 100, width, height), "Demo Game")
		if self:Button(Rect(50, 50, 140, 40), "Start Game") then
			self:StartGame()
			Delete(self)
			return
		end
		if self:Button(Rect(50, 105, 140, 40), "Exit") then
			Game.Shutdown()
			return
		end
	self:EndFrame()
end

function MainMenu:StartGame()
	-- Add the player group
	local playerGroup = SceneGroup.Load("/demo/levels/playergroup.lua")
	Scene.AddSceneGroup(playerGroup)

	local timer = Timer.Start()
	-- Start the first level
	local level1 = SceneGroup.Load("/demo/levels/level1.lua")
	Scene.AddSceneGroup(level1)
	print("Load time: " .. Timer.End(timer) .. " seconds")
end

return MainMenu