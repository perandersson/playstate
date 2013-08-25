local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

PlayerMovementSoundEffect = class("PlayerMovementSoundEffect", Component)
function PlayerMovementSoundEffect:__init(effect)
	Component.__init(self)
	self.timeSinceLastReplay = 0
	self.soundEffect = effect
end

function PlayerMovementSoundEffect:Tick()
	self.timeSinceLastReplay = self.timeSinceLastReplay - SecondsPerTick
	local move = false
	if Keys.IsKeyDown(KeyboardKeys.W) then
		move = true
	elseif Keys.IsKeyDown(KeyboardKeys.S) then
		move = true
	elseif Keys.IsKeyDown(KeyboardKeys.A) then
		move = true
	elseif Keys.IsKeyDown(KeyboardKeys.D) then
		move = true
	end
	
	if self.timeSinceLastReplay < 0 and move then
		self.timeSinceLastReplay = 2.4
		Sound.PlayAtPosition(self.soundEffect, {0, 0, 0})
	end
end

return PlayerMovementSoundEffect