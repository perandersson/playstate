local CullFaces = require "engine.core.cullfaces"
local DepthFunc = require "engine.core.depthfunc"
local SrcBlend = require "engine.core.srcblend"
local DestBlend = require "engine.core.destblend"
local MinFilter = require "engine.core.minfilter"
local MagFilter = require "engine.core.magfilter"
local TextureWrap = require "engine.core.texturewrap"

return {
	Blend = true,
	SrcBlend = SrcBlend.ONE,
	DestBlend = DestBlend.ONE,
	DepthTest = false,
	
	VertexShader = [[
		#version 330

		uniform mat4 ProjectionMatrix;
		uniform mat4 ViewMatrix;
		uniform mat4 ModelMatrix;

		// Light properties
		uniform float LightRadius;
		uniform vec3 LightPosition;

		layout(location = 0) in vec3 position;
		layout(location = 2) in vec2 texCoord;

		out vec2 texCoords;
		out vec3 lightPosInWorldSpace;
		out vec2 screenTexCoord;

		void main()
		{
			mat4 modelView = ViewMatrix * ModelMatrix;
			
			vec4 texPos = vec4(position.xyz, 1.0);
			vec4 pos = modelView * texPos;
			pos.xy += position.xy * LightRadius;
			
			gl_Position = ProjectionMatrix * pos;
			
			// Don't take billboard rotation into consideration - thus only use ViewMatrix
			lightPosInWorldSpace = vec3(ViewMatrix * vec4(LightPosition, 1.0));
			
			vec3 eyePosition = pos.xyz;
			vec4 screenPos = ProjectionMatrix * vec4(eyePosition, 1.0);
			screenPos /= screenPos.w;
			screenPos.z = (screenPos.z > -1.0) ? screenPos.z : -1.0;
			screenTexCoord = screenPos.xy * 0.5 + 0.5;
			
			texCoords = texCoord;
		}
	]],
	LightTexture = {
		MinFilter = MinFilter.LINEAR,
		MagFilter = MagFilter.LINEAR,
		Wrap = {
			S = TextureWrap.CLAMP_TO_EDGE,
			T = TextureWrap.CLAMP_TO_EDGE
		}
	},
	FragmentShader = [[
		#version 330

		// Texture samples to decide if the light should be rendered
		uniform sampler2D PositionsTexture;
		uniform sampler2D NormalsTexture;
		uniform sampler2D DiffuseTexture;

		// How the texture looks like
		uniform sampler2D LightTexture;

		// Global ambient color
		//uniform vec3 AmbientColor;

		// Variables used by the point light
		uniform float LightRadius;
		uniform vec3 LightColor;
		uniform vec3 LightPosition;
		uniform float ConstantAttenuation;
		uniform float LinearAttenuation;
		uniform float QuadraticAttenuation;

		// Position of the pixel
		in vec2 texCoords;
		in vec2 screenTexCoord;
		in vec3 lightPosInWorldSpace;

		//
		// Use: Quadratic Attenuation
		// https://developer.valvesoftware.com/wiki/Constant-Linear-Quadratic_Falloff
		// http://www.lighthouse3d.com/tutorials/glsl-tutorial/point-light-per-pixel/
		// http://blog.slindev.com/2011/01/10/natural-light-attenuation/
		// http://www.paulsprojects.net/opengl/ppatten/ppatten.html
		//

		void main()
		{
			vec4 color = texture2D(LightTexture, texCoords) * vec4(LightColor, 1.0);
			if(color.a == 0)
				discard;
				
			vec3 normal = texture2D(NormalsTexture, screenTexCoord).xyz * 2.0 - 1.0;
			normal = normalize(normal);
			
			vec4 mat = texture2D(DiffuseTexture, screenTexCoord);
			vec3 position = texture2D(PositionsTexture, screenTexCoord).xyz;
			
			vec3 lightDir = lightPosInWorldSpace - position;
			float dist = length(lightDir);
			lightDir /= dist;
			
			float Ia = 1.0 - (dist * dist) / (LightRadius * LightRadius);
					
			gl_FragColor = color * Ia;
		}
	]]
}