local class = require "engine.class"
local Rect = require "engine.core.math.rect"

Editor = class("Editor", CanvasGroup)
function Editor:__init()
	CanvasGroup.__init(self)
	self:SetName("Editor")
	self:SetStyle(require("demo.gui.style"))
	self.enabled = false
	self.sliderValue = 0.5
	
	self.selectedIndex = 0
	self.dropdownValues = { "Value1", "Value2", "Value3" }
end

function Editor:OnProcessCanvas()
	self:BeginFrame(Rect(1024 - 200, 0, 200, 768), "Title here")
		if self:Button(Rect(0, 0, 140, 40), "Button Text") then
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

function Editor:StartGame()
end

return Editor