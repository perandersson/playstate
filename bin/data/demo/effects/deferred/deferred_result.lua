local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"

return {
	DepthTest = false,
	VertexShader = [[
		#version 330

		uniform mat4 ProjectionMatrix;
		
		layout(location = 0) in vec3 position;
		layout(location = 2) in vec2 texcoord;

		out vec2 uvCoords;

		void main()
		{
			gl_Position = ProjectionMatrix * vec4(position.x, position.y, -1.0, 1.0);
			uvCoords = texcoord;
		}
	]],
	DiffuseTexture = {
		MinFilter = MinFilter.LINEAR,
		MagFilter = MagFilter.LINEAR,
		Wrap = {
			S = TextureWrap.CLAMP_TO_EDGE,
			T = TextureWrap.CLAMP_TO_EDGE
		}
	},
	NormalsTexture = {
		MinFilter = MinFilter.LINEAR,
		MagFilter = MagFilter.LINEAR,
		Wrap = {
			S = TextureWrap.CLAMP_TO_EDGE,
			T = TextureWrap.CLAMP_TO_EDGE
		}
	},
	FragmentShader = [[
		#version 330

		uniform sampler2D PositionsTexture;
		uniform sampler2D DiffuseTexture;
		uniform sampler2D NormalsTexture;
		uniform sampler2D LightTexture;

		uniform vec3 AmbientColor;

		in vec2 uvCoords;

		void main()
		{
			vec4 image = texture2D(DiffuseTexture, uvCoords);
			
			vec3 position = texture2D(PositionsTexture, uvCoords).xyz;
			vec3 normal = texture2D(NormalsTexture, uvCoords).xyz;
			normal = normalize(normal);
			
			//image *= texture2D(LightTexture, uvCoords);
			vec4 light = texture2D(LightTexture, uvCoords);
			gl_FragColor = (image + vec4(AmbientColor, 1.0)) * (light + vec4(AmbientColor, 1.0));
			//gl_FragColor = image;
			//gl_FragColor = vec4(normal, 1.0);
		}

	]]
}