local class = require "engine.class"

SinusLightBehaviour = class("SinusLightBehaviour", Component)
function SinusLightBehaviour:__init(speed)
	Component.__init(self)
	self.speed = speed
end

function SinusLightBehaviour:Tick()
	local x, y, z = self:GetNodePosition()
	y = math.abs(math.sin(GameTotalTime * self.speed) * 5.0);
	self:SetNodePosition(x, y, z)
end

return SinusLightBehaviour