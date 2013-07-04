local class = require "engine.class"

-- Include the demo game class and configuration
local config = require "config"
local DemoGame = require "demo.demogame"

-- Function called at the start of the game
function main()
	print("Starting game engine")
	
	print("Starting game")
	Game.Start(DemoGame(), config)
	print("Stopping game")
		
	print("Stopping game engine")
end
