local CullFaces = require "engine.core.cullfaces"
local DepthFunc = require "engine.core.depthfunc"
local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"

return {
	CullFaces = CullFaces.CCW,
	DepthFunc = DepthFunc.LESS,
	VertexShader = [[
		#version 330
		
		uniform mat4 ModelMatrix;
		uniform mat4 ProjectionMatrix;
		uniform mat4 ViewMatrix;

		layout(location = 0) in vec3 position;
		layout(location = 1) in vec3 normal;
		layout(location = 2) in vec2 texcoord;

		out vec3 viewSpacePosition;
		out vec3 worldSpaceNormals;
		out vec2 uvCoords;
		out float depth;

		void main()
		{
			mat4 modelView = ViewMatrix * ModelMatrix;
			gl_Position = ProjectionMatrix * modelView * vec4(position, 1.0);
			depth = gl_Position.z;
			viewSpacePosition = vec3(modelView * vec4(position, 1.0));
				
			// gl_NormalMatrix = transpose inverse ModelMatrix
			mat4 normalMatrix = transpose(inverse(ModelMatrix));	
			worldSpaceNormals = normalize(vec3(normalMatrix * vec4(normal, 0.0)));
			
			uvCoords = texcoord;
		}
	]],
	DiffuseTexture = {
		MinFilter = MinFilter.LINEAR,
		MagFilter = MagFilter.LINEAR,
		Wrap = {
			S = TextureWrap.REPEAT,
			T = TextureWrap.REPEAT
		}
	},
	FragmentShader = [[
		#version 330

		uniform sampler2D DiffuseTexture;
		uniform vec3 DiffuseColor;

		in vec3 viewSpacePosition;
		in vec3 worldSpaceNormals;
		in vec2 uvCoords;
		in float depth;

		void main()
		{
			gl_FragData[0] = vec4(texture2D(DiffuseTexture, uvCoords).rgb, 0);
			gl_FragData[1] = vec4(viewSpacePosition, depth);
			gl_FragData[2] = vec4(worldSpaceNormals, 0);
		}
	]]
}