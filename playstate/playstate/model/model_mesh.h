#pragma once

#include "../rendering/vertex_buffer.h"
#include "../rendering/index_buffer.h"
#include "../rendering/texture2d.h"
#include "../math/color.h"

namespace playstate
{
	struct ModelMesh
	{
		VertexBuffer* Vertices;
		IndexBuffer* Indices;
		Resource<Texture2D> DiffuseTexture;
		Resource<Texture2D> AmbientTexture;
		Resource<Texture2D> SpecularTexture;
		Resource<Texture2D> SpecularHighlightTexture;
		Resource<Texture2D> AlphaTexture;
		Resource<Texture2D> BumpMapTexture;
		Resource<Texture2D> DisplacementTexture;
		Color AmbientColor;
		Color DiffuseColor;
		Color SpecularColor;
		float32 SpecularCoefficient;
		float32 Alpha;
		uint32 Id;
	};
}
