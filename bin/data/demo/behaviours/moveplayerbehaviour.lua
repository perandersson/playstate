local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

MovePlayerBehaviour = class(Component, function(self, speed)
	Component.__init(self)
	self.speed = speed
	self.prevX = 0
	self.prevY = 0
end)

function MovePlayerBehaviour:Update()
	self:RotateNode()
	self:TranslateNode()
end

function MovePlayerBehaviour:RotateNode()
	local currentX = Mouse.GetX()
	local currentY = Mouse.GetY()
		
	if currentX == self.prevX and currentY == self.prevY then
		return
	end
	
	x, y = self:GetArcballVector(currentX, currentY)
	local angle = math.atan2(x, y)
		
	self:GetNode():SetRotation(Vector3(0, angle, 0))
	
	self.prevX = currentX
	self.prevY = currentY
	
end

function MovePlayerBehaviour:GetArcballVector(x, y)
	if x < 0 then x = 0 end
	if y < 0 then y = 0 end

	px = (x / Window.GetWidth() * 2) - 1.0
	py = (y / Window.GetHeight() * 2) - 1.0
	
	-- Normalize px,py,0
	local length = math.sqrt(px * px + py * py + 0)
	px = px / length
	py = py / length
	
	return px, py
end

function MovePlayerBehaviour:TranslateNode()
	local x = 0.0
	local z = 0.0
	local move = false
		
	if Keys.IsKeyDown(KeyboardKeys.W) then
		z = z - self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.S) then
		z = z + self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.A) then
		x = x - self.speed * GameDeltaTime
		move = true
	end
	
	if Keys.IsKeyDown(KeyboardKeys.D) then
		x = x + self.speed * GameDeltaTime
		move = true
	end
	
	if move then
		self:GetNode():Translate(Vector3(x, 0.0, z))
	end
end

return MovePlayerBehaviour