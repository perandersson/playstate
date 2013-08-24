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
		layout(location = 2) in vec4 color;
		
		out vec2 fsCoords;
		out vec4 fsColor;

		void main()
		{
			gl_Position = ProjectionMatrix * vec4(position.x, position.y, 0.0, 1.0);
			fsColor = color;
			fsCoords = texCoord;
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
		
		uniform sampler2D Texture;
		
		in vec4 fsColor;
		in vec2 fsCoords;
		
		void main()
		{
			vec4 textureColor = texture2D(Texture, fsCoords);
			gl_FragColor = fsColor * vec4(textureColor.r, textureColor.r, textureColor.r, textureColor.r);
		}
	]]
}