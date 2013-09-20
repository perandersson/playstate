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
	self:BeginFrame(Rect(1024 - 200, 0, 200, 768), "Example")
		if self:Button(Rect(0, 0, 140, 40), "Start Game") then
			if self.enabled then
				self:StartGame()
				Delete(self)
				return
			else
				print("Start only if enabled!")
			end
		end
		self.enabled = self:Checkbox(Rect(0, 50, 140, 20), self.enabled, "Enable?")
		self:Label(Rect(0, 80, 0, 0), "Hello World Label")
		self.sliderValue = self:Slider(Rect(0, 110, 140, 20), self.sliderValue,
			0.1, 1.0, 0.1)
		self.selectedIndex = self:ComboBox(Rect(0, 140, 140, 20), 
			self.selectedIndex, self.dropdownValues)
		--self:TreeView(...)
	self:EndFrame()
end

function MainMenu:StartGame()
end

return MainMenu