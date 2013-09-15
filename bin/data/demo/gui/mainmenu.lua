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
		-- Load level
	end
	self:EndFrame()
end

return MainMenu