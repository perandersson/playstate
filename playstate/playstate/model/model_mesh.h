#pragma once

#include "../rendering/vertex_buffer.h"
#include "../rendering/index_buffer.h"
#include "../rendering/texture2d.h"
#include "../math/color.h"

namespace playstate
{
	struct ModelMesh
	{
		IVertexBuffer* Vertices;
		IIndexBuffer* Indices;
		Resource<ITexture2D> DiffuseTexture;
		Resource<ITexture2D> AmbientTexture;
		Resource<ITexture2D> SpecularTexture;
		Resource<ITexture2D> SpecularHighlightTexture;
		Resource<ITexture2D> AlphaTexture;
		Resource<ITexture2D> BumpMapTexture;
		Resource<ITexture2D> DisplacementTexture;
		Color AmbientColor;
		Color DiffuseColor;
		Color SpecularColor;
		float32 SpecularCoefficient;
		float32 Alpha;
		uint32 Id;
	};
}
