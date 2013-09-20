
local style = { 
	Font = Font.Load("/demo/fonts/consolab.ttf"), 
	FontColor = "#00",
	
	ShadowOffset = 3, 
	ShadowColor = { 0, 0, 0, 0.3 },
	
	BackColor = { 
		Top = "#99", 
		Bottom = "#22"
	},
	
	Label = {
		FontColor = "#FF"
	},
	
	Frame = {
		Title = {
			Height = 25,
			FontColor = "#00",
			BackColor = {
				Top = "#EE",
				Bottom = "#BB"
			}
		},
		Body = {
			BackColor = {
				Top = "#55",
				Bottom = "#99"
			}
		}
	},
	Slider = {
		Normal = {
			BackColor = {
				Top = "#CC",
				Bottom = "#99"
			},
			FontColor = "#00",
			BarColor = {
				Top = "#99",
				Bottom = "#55"
			}
		},
		Hover = {
			BackColor = {
				Top = "#EE",
				Bottom = "#BB"
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
	Button = {
		Normal = {
			BackColor = {
				Top = "#CC",
				Bottom = "#99"
			}
		},
		Hover = {
			BackColor = {
				Top = "#EE",
				Bottom = "#BB"
			}
		},
		Down = {
			FontColor = "#FF",
			BackColor = {
				Top = "#99",
				Bottom = "#77"
			}
		},
	}
}

return style