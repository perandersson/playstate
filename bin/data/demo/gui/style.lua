
local style = { 
	-- The font for all the text in the GUI
	Font = Font.Load("/demo/fonts/consolab.ttf"), 
	
	ShadowOffset = 3, 
	ShadowColor = { 0, 0, 0, 0.3 },
	
	BorderSize = 1,
	BorderColor = "#00",
	
	-- The background color
	BackColor = { 
		Top = "#99", 
		Bottom = "#22"
	},
	
	Label = {
		FontColor = "#FF"
	},
	
	-- Slider style
	Slider = {
		Normal = {
			BackColor = {
				Top = "#BB",
				Bottom = "#DD"
			},
			FontColor = "#00",
			BarColor = {
				Top = "#99",
				Bottom = "#55"
			}
		},
		Hover = {
			BackColor = {
				Top = "#DD",
				Bottom = "#DD"
			},
			FontColor = "#00",
			BarColor = {
				Top = "#99",
				Bottom = "#55"
			}
		},
		Down = {
			BackColor = {
				Top = "#99",
				Bottom = "#77"
			},
			FontColor = "#FF",
			BarColor = {
				Top = "#55",
				Bottom = "#99"
			}
		},
	},
	
	Checkbox = {
		Unchecked = {
			FontColor = "#00"
		},
		Checked = {
			FontColor = "#FF"
		},
		BoxSize = 20,
		Normal = {
			BackColor = {
				Top = "#99", 
				Bottom = "#22"
			}
		},
		Hover = {
			BackColor = {
				Top = "#88", 
				Bottom = "#11"
			}
		},
		Down = {
			BackColor = {
				Top = "#22",
				Bottom = "#99",
			}
		}
	},
	
	-- The front color (for example Font colors)
	FrontColor = "#FF"
}

return style