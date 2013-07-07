local MovePlayerBehaviour = require "demo.behaviours.moveplayerbehaviour"
local FollowCameraBehaviour = require "demo.behaviours.followcamerabehaviour"

AddMoreSceneGroupsOnTime = class(Component, function(self, totalTime)
	Component.__init(self)
	self.timeLeft = totalTime
end)

function AddMoreSceneGroupsOnTime:Update()
	self.timeLeft = self.timeLeft - GameDeltaTime
	if self.timeLeft < 0 then
		print("Loading new level")
		local group = Game.LoadLevel("/demo/levels/level2.lua")
		Scene.AddSceneGroup(group)
		Delete(self)
	end
end

-- The group name
level1 = SceneGroup()

-- The nodes inside this group
local playerModel = Model.Load("/demo/models/player/player.obj")
local player1 = SceneNode(level1)
player1:AddComponent(MovePlayerBehaviour(10.0))
player1:AddComponent(FollowCameraBehaviour())
player1:AddComponent(RenderStaticModel(playerModel))
player1:SetPosition({0, 0, 10})

-- Node used to load new levels after a specific time.
-- The purpose of this node is to verify that the entire script engine won't crash when failing to load the new level.
local loaderNode = SceneNode(level1)
loaderNode:AddComponent(AddMoreSceneGroupsOnTime(5))

local chinalampModel = Model.Load("/demo/models/chinalamp/China_lampEX.obj")
for i=1, 20 do
	for j=1, 20 do
		--math.random
		local node = SceneNode(level1)
		node:AddComponent(RenderStaticModel(chinalampModel))
		node:SetPosition({i * 20, 0, j * 20})
	end
end

-- Set ambient lighting on the scene
Scene.SetAmbientLight({0.1, 0.2, 0.3})

-- Add lighting using random positions
math.randomseed( os.time() )
for i=1, 20 do
	for j=1, 20 do
		
	end
end

return level1
