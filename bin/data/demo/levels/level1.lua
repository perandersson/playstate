local MovePlayerBehaviour = require "demo.behaviours.moveplayerbehaviour"
local FollowCameraBehaviour = require "demo.behaviours.followcamerabehaviour"
local SinusLightBehaviour = require "demo.behaviours.sinuslightbehaviour"

-- The group name.
level1 = SceneGroup()

-- The player object. Can be moved using keyboard and mouse
local playerModel = Model.Load("/demo/models/player/player.obj")
local player1 = SceneNode()
player1:AddComponent(MovePlayerBehaviour(10.0))
player1:AddComponent(FollowCameraBehaviour())
player1:AddComponent(RenderStaticModel(playerModel))
player1:SetPosition(0, 0, 10)
level1:AddNode(player1)

local playerColor = SceneNode()
playerColor:AddComponent(PointLight({0.2, 0.5, 0.7, 1.0}, 10.0, 0.0, 0.0, 0.5))
player1:AddNode(playerColor)

local i = 0
local j = 0

local chinalampModel = Model.Load("/demo/models/chinalamp/China_lampEX.obj")
for i=0, 19 do
	for j=0, 19 do
		local node = SceneNode()
		node:AddComponent(RenderStaticModel(chinalampModel))
		node:SetPosition(i * 20, 0, j * 20)
		level1:AddNode(node)
	end
end

-- Add lighting using random positions
math.randomseed( os.time() )
for i=0, 19 do
	for j=0, 19 do
		local node = SceneNode()
		node:SetPosition(math.random(0, 2000) / 10.0, 0.0, math.random(0, 2000) / 10.0)
		
		local color = {math.random(0, 100) / 100.0, math.random(0, 100) / 100.0, math.random(0, 100) / 100.0}
		local radius = 10.0
		local constantAttenuation = 0.0
		local linearAttenuation = 0.0
		local quadricAttenuation = 0.5
		node:AddComponent(PointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation))
		node:AddComponent(SinusLightBehaviour(math.random(0, 100) / 100.0 * 5.0))
		level1:AddNode(node)
	end
end

-- Add ground
local ground1 = Model.Load("/demo/models/ground1/ground1.obj")
for i=0, 19 do
	for j=0, 19 do
		local node = SceneNode()
		node:AddComponent(RenderStaticModel(ground1))
		node:SetPosition(i * 20, 0, j * 20)
		level1:AddNode(node)
	end
end

return level1