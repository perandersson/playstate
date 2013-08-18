local GuiBuilder = require "engine.core.gui.guibuilder"

-- ShadowOffset = 3 or { 3, 3 } or { 3, 3, 3, 3 }
-- Font = Font.Load("....")
-- BackColor = "#FFFFFF" or "#FF" or { 0.5, 0.5, 0.5, 0.5 } or { Top = "#FF", Bottom = "#FF" }

local style = { 
	Font = Font.Load("/demo/fonts/calibri.ttf"), 
	ShadowOffset = 3, 
	BackColor = { 
		Top = "#99", 
		Bottom = "#22"
	}
}

local mainMenu = CanvasGroup()
mainMenu:SetStyle(style)

local window = GuiFrameWidget(mainMenu)
window:SetTitle("Example")
window:SetSize(200, 768)
window:SetPosition(1024 - 200, 0)

local btnLoadLevel = GuiButtonWidget(mainMenu)
btnLoadLevel:SetText("Load Level1.lua")
btnLoadLevel:SetSize(140, 40)
btnLoadLevel:SetPosition(0, 20)
window:AddChildNode(btnLoadLevel)


return mainMenu
--[[
return GuiBuilder.Build({
	Frame = {
		Title = "Example", Width = 300, Height = 200, Position = { 100, 100 },
		Body = {
			{ "", Field = { Value = "Hello World", Hint = "Enter value here!" } },
			{ "size", Slider = { Default = 1.0, Min = 0.0, Max = 10.0, Steps = 0.1 } },
			{ "show?", Checkbox = { Default = true } },
			{ "", Button = { Text = "Start Level1", 
			Action = function(btn) 
				local level1 = SceneGroup.Load("/demo/levels/level1.lua")
				Scene.AddSceneGroup(level1)
			end } }
		}
	}
})]]--