local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

MovePlayerBehaviour = class("MovePlayerBehaviour", "Component")
function MovePlayerBehaviour:__init(speed)
	Component.__init(self)
	self.speed = speed
	self.prevMouseX = 0
	self.prevMouseY = 0
end

function MovePlayerBehaviour:Update()
	self:RotateThis()
	self:TranslateThis()
end

function MovePlayerBehaviour:RotateThis()
	local mouseX, mouseY = Mouse.GetPosition()
	if mouseX == self.prevMouseX and mouseY == self.prevMouseY then
		return
	end
	
	x, y = self:GetArcballVector(mouseX, mouseY)
	local angle = math.atan2(x, y)
		
	self:SetNodeRotation({0, angle, 0})
	
	self.prevMouseX = mouseX
	self.prevMouseY = mouseY
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

function MovePlayerBehaviour:TranslateThis()
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
		self:TranslateNode(velocity)
	end
end

return MovePlayerBehaviour