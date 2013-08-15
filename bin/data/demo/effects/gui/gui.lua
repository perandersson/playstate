local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"
local DestFactor = require "engine.core.destfactor"
local SrcFactor = require "engine.core.srcfactor"

return {
	DepthTest = true,
	Blend = true,
	SrcFactor = SrcFactor.SRC_ALPHA,
	DestFactor = DestFactor.ONE_MINUS_SRC_ALPHA,
	VertexShader = [[
		#version 330

		uniform mat4 ProjectionMatrix;
		
		layout(location = 0) in vec2 position;
		layout(location = 1) in vec2 texCoord;
		layout(location = 2) in uint paletteIndex;
		
		const vec4 colors[5] = vec4[](
			vec4(0.0, 0.0, 0.0, 0.5),
			vec4(0.93, 0.93, 0.93, 1),
			vec4(0.7, 0.7, 0.7, 1),
			vec4(0.9, 0.9, 0.9, 1),
			vec4(0.3, 0.3, 0.3, 1)
		);

		out vec4 color;

		void main()
		{
			gl_Position = ProjectionMatrix * vec4(position.x, position.y, 0.0, 1.0);
			color = colors[paletteIndex];
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