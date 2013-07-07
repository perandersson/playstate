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
local playerModel = Model.Load("/demo/models/chinalamp/China_lampEX.obj")
local player1 = SceneNode(level1)
player1:AddComponent(MovePlayerBehaviour(10.0))
player1:AddComponent(FollowCameraBehaviour())
player1:AddComponent(RenderStaticModel(playerModel))
player1:SetPosition({0, 0, 10})

-- Node used to load new levels after a specific time
local loaderNode = SceneNode(level1)
loaderNode:AddComponent(AddMoreSceneGroupsOnTime(5))

return level1
