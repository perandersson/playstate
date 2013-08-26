local class = require "engine.class"
local KeyboardKeys = require "engine.core.keyboardkeys"

PlayerMovementSoundEffect = class("PlayerMovementSoundEffect", Component)
function PlayerMovementSoundEffect:__init(effects)
	Component.__init(self)
	self.timeSinceLastReplay = 0
	self.soundEffects = effects
	self.playIndex = 1
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
		local currentEffect = self.soundEffects[self.playIndex]
		if self.playIndex == 1 then
			self.playIndex = 2
		else
			self.playIndex = 1
		end
		self.timeSinceLastReplay = SoundEffect.GetDuration(currentEffect)
		Sound.Play(currentEffect)
	end
end

return PlayerMovementSoundEffect