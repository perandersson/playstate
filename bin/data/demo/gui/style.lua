
-- ShadowOffset = 3 or { 3, 3 } or { 3, 3, 3, 3 }
-- Font = Font.Load("....")
-- BackColor = "#FFFFFF" or "#FF" or { 0.5, 0.5, 0.5, 0.5 } or { Top = "#FF", Bottom = "#FF" }


local style = { 
	-- The font for all the text in the GUI
	Font = Font.Load("/demo/fonts/consolab.ttf"), 
	
	-- The offset of the shadow. It can be: 3 or { 3, 3 } or { 3, 3, 3, 3 }
	ShadowOffset = 3, 
	
	-- The background color
	BackColor = { 
		Top = "#99", 
		Bottom = "#22"
	},
	
	-- The front color (for example Font colors)
	FrontColor = "#FF"
}

return style