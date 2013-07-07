local class = require "engine.class"
local vector3 = require "engine.core.math.vector3"
local config = require "config"

FollowCameraBehaviour = class(Component, function(self)
	Component.__init(self)
end)

function FollowCameraBehaviour:Update()
	local pos = self:GetNode():GetPosition()
	local eye = vector3.add(pos, config.camera.eye)
	
	ActiveCamera.LookAt(eye, pos, config.camera.up)
end

return FollowCameraBehaviour