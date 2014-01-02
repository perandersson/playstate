--local MovePlayerBehaviour = require "demo.behaviours.moveplayerbehaviour"
local FollowCameraBehaviour = require "demo.behaviours.followcamerabehaviour"
local PlayerMovementSoundEffect = require "demo.behaviours.playermovementsoundeffect"

-- The group name.
playerGroup = SceneGroup({
	RenderProcessor = LinkedListRenderProcessor()
})

-- The player object. Can be moved using keyboard and mouse
local playerModel = Resource.Load("/demo/models/player/player.obj")
local footsteps = {
	Resource.Load("/demo/sound/effects/footstep0.wav"),
	Resource.Load("/demo/sound/effects/footstep1.wav")
}
local player1 = SceneNode()
player1:AddComponent(MovePlayerComponent(10.0))
player1:AddComponent(FollowCameraBehaviour())
player1:AddComponent(PlayerMovementSoundEffect(footsteps))
player1:AddComponent(StaticModelComponent(playerModel))
player1:SetPosition(0, 0, 10)
player1:SetScale(2, 2, 2)
playerGroup:AddChild(player1)

local playerColor = PointLight({0.2, 0.5, 0.7, 1.0}, 10.0, 0.0, 0.0, 0.5)
player1:AddChild(playerColor)

return playerGroup