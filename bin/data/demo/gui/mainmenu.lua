local class = require "engine.class"

MainMenu = class("MainMenu", CanvasGroup, {})
function MainMenu:__init()
	CanvasGroup.__init(self)
	self:SetStyle(require("demo.gui.style"))
	self.toggled = false
	self.sliderValue = 0.0
	self.selectedIndex = 0
end

function MainMenu:OnProcessCanvas()
	-- Proxy method
	self:OnGUI()
end

function MainMenu:OnGUI()
	self:BeginFrame({ 200, 768 }, { 1024 - 200, 0 }, "Example")
		if self:Button({ 140, 40 }, { 0, 20 }, "Start Game") then
			if self.toggled then
				self:StartGame()
				Delete(self)
				return
			else
				print("Start only if toggled!")
			end
		end
		self.toggled = self:Toggle({ 140, 20 }, { 0, 70 }, self.toggled, "Enable?")
		self.sliderValue = self:Slider({ 140, 20 }, { 0, 100 }, self.sliderValue,
			0.0, 1.0, 0.1)
		self.selectedIndex = self:ComboBox({ 140, 20 }, { 0, 130 }, self.selectedIndex, {
			"Value1", "Value2", "Value3"
		})
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