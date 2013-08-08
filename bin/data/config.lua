local config = {}

-- Window configuration
config.window = {}
config.window.width = 1024
config.window.height = 768
config.window.fullscreen = false
config.window.title = "Demo FPS: "
config.window.bpp = 32

-- Camera configuration
config.camera = {}
config.camera.eye = {0, 50.0, 50.0}
config.camera.center = {0, 0, 0}
config.camera.up = {0, 1, 0}

-- Development mode (will prevent crashes on bad shader code)
config.graphics = {}
config.graphics.developmentmode = true

return config