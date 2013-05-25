local class = require "class"
local KeyboardKeys = require "core.keyboardkeys"

MovePlayerBehaviour = class(Component, function(self, speed)
	Component.__init(self)
	self.speed = speed
end)

function MovePlayerBehaviour:Update()
	if Input.IsKeyDown(KeyboardKeys.W) then
		
	end
end

FollowCameraBehaviour = class(Component, function(self)
	Component.__init(self)
end)

function FollowCameraBehaviour:Update()
end

Level1 = class(SceneGroup, function(self)
	SceneGroup.__init(self)
	
	--
	-- Put level data here!
	--
	
	local playerModel = Model("/models/chinalamp/China_lampEX.obj")
	local player1 = SceneNode(self)
	player1:AddComponent(MovePlayerBehaviour(10.0))
	player1:AddComponent(FollowCameraBehaviour())
	player1:AddComponent(RenderStaticModel(playerModel))
	player1:SetPosition(Vector3(0, 0, 10))	
	
end)

return Level1()
