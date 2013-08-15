local GuiBuilder = {}

GuiBuilder.Build = function(properties)
	print("Building GUI")
	
	local group = CanvasGroup()
	
	for key,value in pairs(properties) do
		if key == "Frame" then
			GuiBuilder.AddFrame(group, value)
		end
	end

	return group
end

GuiBuilder.AddFrame = function(group, frame)
	print("Adding frame: " .. frame.Title .. " with width " .. frame.Width .. " and height " .. frame.Height )
	
	local frameNode = GuiFrameWidget(group, frame.Title, frame.Width, frame.Height)
	
	for key, value in pairs(frame.Body) do
		GuiBuilder.AddBodyRow(group, frameNode, value)
	end
end

GuiBuilder.AddBodyRow = function(group, frameNode, bodyItem)
	print("Adding body row item to frame")
	
	--local rowNode = SceneNode(group)
	--frameNode:AddChildNode(rowNode)
	local numColumns = 0
	for key, value in pairs(bodyItem) do
		if type(value) == "string" then
			if value == "" then
				GuiBuilder.AddEmptyColumn(group, rowNode)
			else
				GuiBuilder.AddLabelColumn(group, rowNode, value)
			end
		elseif type(value) == "table" then
			GuiBuilder.AddComplexColumn(group, rowNode, key:lower(), value)
		end
		
		numColumns = numColumns + 1
	end 
	
	--rowNode:AddComponent(GuiRowComponent(numColumns))
end

GuiBuilder.AddEmptyColumn = function(group, rowNode)
	print("Adding empty column")
end

GuiBuilder.AddLabelColumn = function(group, rowNode, label)
	print("Adding label column with value: " .. label)
	--local column = GuiLabelWidget(group)
	--column:AddComponent(GuiLabelComponent(label))
	--rowNode:AddChildNode(column)
end

GuiBuilder.AddComplexColumn = function(group, rowNode, complexType, value)
	if complexType == "field" then
		print("Adding field column")
		--local field = GuiFieldWidget(group)
		-- field:AddComponent(GuiFieldComponent(value.Value, value.Hint))
		--rowNode:AddChildNode(field)
	elseif complexType == "slider" then
		print("Adding slider column")
		--local slider = SceneNode(group)
		-- slider:AddComponent(GuiSliderComponent(value.Min, value.Max, value.Steps, value.Default))
		--rowNode:AddChildNode(slider)
	elseif complexType == "checkbox" then
		print("Adding checkbox column")
		--local checkbox = SceneNode(group)
		-- checkbox:AddComponent(GuiCheckboxComponent(value.Default))
		--rowNode:AddChildNode(checkbox)
	elseif complexType == "button" then
		print("Adding button column")
		--local button = SceneNode(group)
		-- button:AddComponent(GuiButtonComponent(value.Text, value.Action))
		--rowNode:AddChildNode(button)
	else
		print("Adding complex column")
	end
end

return GuiBuilder