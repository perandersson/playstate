local class = require "engine.class"

MainMenu = class("MainMenu", CanvasGroup, {})
function MainMenu:__init()
	CanvasGroup.__init(self)
	self:SetStyle(require("demo.gui.style"))
end

function MainMenu:OnProcessCanvas()
	-- Proxy method
	self:OnGUI()
end

function MainMenu:OnGUI()
	self:BeginFrame({ 200, 768 }, { 1024 - 200, 0 }, "Example")
	if self:Button({ 140, 40 }, { 0, 20 }, "Start Game") then
		-- Add the player group
		local playerGroup = SceneGroup.Load("/demo/levels/playergroup.lua")
		Scene.AddSceneGroup(playerGroup)

		local timer = Timer.Start()
		-- Start the first level
		local level1 = SceneGroup.Load("/demo/levels/level1.lua")
		Scene.AddSceneGroup(level1)
		print("Load time: " .. Timer.End(timer) .. " seconds")
		Delete(self)
		return
	end
	self:EndFrame()
end

return MainMenu