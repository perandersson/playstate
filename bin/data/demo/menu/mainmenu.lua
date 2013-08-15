local GuiBuilder = require "engine.core.gui.guibuilder"

return GuiBuilder.Build({
	Frame = {
		Title = "Example", Width = 300, Height = 200,
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
})