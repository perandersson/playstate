local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"
local DestBlend = require "engine.core.destblend"
local SrcBlend = require "engine.core.srcblend"

return {
	DepthTest = true,
	Blend = true,
	SrcBlend = SrcBlend.SRC_ALPHA,
	DestBlend = DestBlend.ONE_MINUS_SRC_ALPHA,
	VertexShader = [[
		#version 330

		uniform mat4 ProjectionMatrix;
		
		layout(location = 0) in vec2 position;
		layout(location = 1) in int paletteIndex;
		
		const vec4 colors[3] = vec4[](
			vec4(0.0, 0.0, 0.0, 0.1),
			vec4(1.0, 0, 0, 1),
			vec4(1.0, 0.7, 0.7, 1)
		);

		out vec4 color;

		void main()
		{
			gl_Position = ProjectionMatrix * vec4(position.x, position.y, 0.0, 1.0);
			color = colors[paletteIndex % 3];
			//color = vec4(1.0, 1.0, 1.0, 0.1);
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
		
		in vec4 color;

		void main()
		{
			gl_FragColor = color;
		}
	]]
}