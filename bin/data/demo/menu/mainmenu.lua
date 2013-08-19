local class = require "engine.class"

-- ShadowOffset = 3 or { 3, 3 } or { 3, 3, 3, 3 }
-- Font = Font.Load("....")
-- BackColor = "#FFFFFF" or "#FF" or { 0.5, 0.5, 0.5, 0.5 } or { Top = "#FF", Bottom = "#FF" }

local style = { 
	--Font = Font.Load("/demo/fonts/calibri.ttf"), 
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
