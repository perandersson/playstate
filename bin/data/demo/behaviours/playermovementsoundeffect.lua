local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

PlayerMovementSoundEffect = class("PlayerMovementSoundEffect", Component)
function PlayerMovementSoundEffect:__init(effect)
	Component.__init(self)
	self.timeSinceLastReplay = 0
	self.soundEffect = effect
end

function PlayerMovementSoundEffect:Update()
	self.timeSinceLastReplay = self.timeSinceLastReplay - GameDeltaTime
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
		self.timeSinceLastReplay = SoundEffect.GetDuration(self.soundEffect)
		Sound.Play(self.soundEffect)
	end
end

return PlayerMovementSoundEffect