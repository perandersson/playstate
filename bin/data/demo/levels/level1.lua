local MovePlayerBehaviour = require "demo.behaviours.moveplayerbehaviour"

-- The group name
level1 = SceneGroup()

-- The nodes inside this group
local playerModel = Model.Load("/demo/models/chinalamp/China_lampEX.obj")
local player1 = SceneNode(level1)
player1:AddComponent(MovePlayerBehaviour(10.0))
player1:AddComponent(RenderStaticModel(playerModel))
player1:SetPosition(Vector3(0, 0, 10))	

return level1
