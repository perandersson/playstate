local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

MovePlayerBehaviour = class(Component, function(self, speed)
	Component.__init(self)
	self.speed = speed
	self.prevPos = {0, 0}
end)

function MovePlayerBehaviour:Update()
	self:RotateNode()
	self:TranslateNode()
end

function MovePlayerBehaviour:RotateNode()
	local currentPos = Mouse.GetPosition()
	if currentPos[1] == self.prevPos[1] and currentPos[2] == self.prevPos[2] then
		return
	end
	
	x, y = self:GetArcballVector(currentPos[1], currentPos[2])
	local angle = math.atan2(x, y)
		
	self:GetNode():SetRotation({0, angle, 0})
	
	self.prevPos = currentPos
end

function MovePlayerBehaviour:GetArcballVector(x, y)
	if x < 0 then x = 0 end
	if y < 0 then y = 0 end

	px = (x / Window.GetWidth() * 2) - 1.0
	py = (y / Window.GetHeight() * 2) - 1.0
	
	-- Normalize px,py,0
	local length = math.sqrt(px * px + py * py + 0)
	return px / length, py / length
end

function MovePlayerBehaviour:TranslateNode()
	local velocity = {0, 0, 0}
	local move = false
		
	if Keys.IsKeyDown(KeyboardKeys.W) then
		velocity[3] = velocity[3] - self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.S) then
		velocity[3] = velocity[3] + self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.A) then
		velocity[1] = velocity[1] - self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.D) then
		velocity[1] = velocity[1] + self.speed * GameDeltaTime
		move = true
	end
	
	if move then
		self:GetNode():Translate(velocity)
	end
end

return MovePlayerBehaviour