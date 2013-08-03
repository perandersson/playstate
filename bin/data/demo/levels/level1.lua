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
		-- Try to load a non-existing level. This is to see that
		-- the game doesn't crash if this fail.
		local group = SceneGroup.Load("/demo/levels/level2.lua")
		Scene.AddSceneGroup(group)
		Delete(self)
	end
end

TestMoveLight = class(Component, function(self, speed)
	Component.__init(self)
	self.speed = speed
end)

function TestMoveLight:Update()
	local x, y, z = self:GetNodePosition()
	y = math.abs(math.sin(GameTotalTime * self.speed) * 5.0);
	self:SetNodePosition({x, y, z})
end

-- The group name.
level1 = SceneGroup()

-- The player object. Can be moved using keyboard and mouse
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
		local node = SceneNode(level1)
		node:AddComponent(RenderStaticModel(chinalampModel))
		node:SetPosition({i * 20, 0, j * 20})
	end
end

-- Add lighting using random positions
math.randomseed( os.time() )
for i=1, 20 do
	for j=1, 20 do
		local node = SceneNode(level1)
		local position = {math.random(0, 2000) / 10.0, 0.0, math.random(0, 2000) / 10.0}
		node:SetPosition(position)
		
		local color = {math.random(0, 100) / 100.0, math.random(0, 100) / 100.0, math.random(0, 100) / 100.0}
		local radius = 10.0
		local constantAttenuation = 0.0
		local linearAttenuation = 0.0
		local quadricAttenuation = 0.5
		node:AddComponent(PointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation))
		node:AddComponent(TestMoveLight(math.random(0, 100) / 100.0 * 5.0))
	end
end

-- Add ground
local ground1 = Model.Load("/demo/models/ground1/ground1.obj")
for i=1, 20 do
	for j=1, 20 do
		local node = SceneNode(level1)
		node:AddComponent(RenderStaticModel(ground1))
		node:SetPosition({i * 20, 0, j * 20})
	end
end

return level1
