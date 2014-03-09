local class = require "engine.class"
local Vector3 = require "engine.core.math.vector3"
local config = require "config"

FollowCameraBehaviour = class("FollowCameraBehaviour", Component)

function FollowCameraBehaviour:__init()
	Component.__init(self)
end

function FollowCameraBehaviour:Update()
	local x, y, z = self:GetNodePosition()
	local pos = {x, y, z}
	local eye = Vector3.Add(pos, config.camera.eye)
	
	ActiveCamera.LookAt(eye, pos, config.camera.up)
end

return FollowCameraBehaviour
