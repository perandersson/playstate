local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"

return {
	DepthTest = false,
	VertexShader = [[
		#version 330

		uniform mat4 ModelMatrix;
		uniform mat4 ProjectionMatrix;
		uniform mat4 ViewMatrix;
		
		layout(location = 0) in vec3 position;
		layout(location = 1) in uint paletteIndex;
		
		out uint palette;

		void main()
		{
			mat4 modelView = ViewMatrix * ModelMatrix;
			gl_Position = ProjectionMatrix * modelView * vec4(position, 1.0);
			palette = paletteIndex;
		}
	]],
	UserInterfaceTexture = {
		MinFilter = MinFilter.LINEAR,
		MagFilter = MagFilter.LINEAR,
		Wrap = {
			S = TextureWrap.CLAMP_TO_EDGE,
			T = TextureWrap.CLAMP_TO_EDGE
		}
	},
	FragmentShader = [[
		#version 330

		//uniform sampler2D UserInterfaceTexture;
		uniform vec4 Color;

		in uint palette;

		void main()
		{
			//vec4 image = texture2D(UserInterfaceTexture, uvCoords);
			gl_FragColor = Color;
		}
	]]
}