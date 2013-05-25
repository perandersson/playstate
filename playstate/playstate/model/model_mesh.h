#pragma once

#include "../rendering/vertex_buffer.h"
#include "../rendering/index_buffer.h"
#include "../rendering/texture2d.h"
#include "../math/color.h"

namespace playstate
{
	class ModelMesh
	{
	public:
		//
		// Constructor
		ModelMesh();

		//
		// Destructor
		~ModelMesh();

		void SetVertices(VertexBuffer* vertices);
		void SetIndices(IndexBuffer* indices);
		void SetDiffuseColor(const Color& diffuseColor);
		void SetDiffuseTexture(Resource<Texture2D> diffuseTexture);

	public:
		// Read-only property for the vertex buffer
		VertexBuffer*& const Vertices;

		// Read-only property for the index buffer
		IndexBuffer*& const Indices;

		// Read-only property for the diffuse texture
		Resource<Texture2D>& const DiffuseTexture;
		
		// Read-only property for the ambient texture
		Resource<Texture2D>& const AmbientTexture;
		
		// Read-only property for the specular texture
		Resource<Texture2D>& const SpecularTexture;
		
		// Read-only property for the specular highlight texture
		Resource<Texture2D>& const SpecularHighlightTexture;
		
		// Read-only property for the alpha texture
		Resource<Texture2D>& const AlphaTexture;

		// Read-only property for the bump map
		Resource<Texture2D>& const BumpMap;

		// Read-only property for the displacement texture
		Resource<Texture2D>& const DisplacementTexture;

		// Read-only property for the ambient color
		const Color& AmbientColor;
		
		// Read-only property for the diffuse color
		const Color& DiffuseColor;

		// Read-only property for the specular color
		const Color& SpecularColor;

		// Read-only property for the alpha 
		const float& Alpha;

		// Read-only property for the specular coefficient
		const float& SpecularCoefficient;

		// Read-only property for retrieving the unique Id of this object
		const uint32& Id;

	private:
		VertexBuffer* mVertices;
		IndexBuffer* mIndices;
		Resource<Texture2D> mDiffuseTexture;
		Resource<Texture2D> mAmbientTexture;
		Resource<Texture2D> mSpecularTexture;
		Resource<Texture2D> mSpecularHighlightTexture;
		Resource<Texture2D> mAlphaTexture;
		Resource<Texture2D> mBumpMap;
		Resource<Texture2D> mDisplacementTexture;
		Color mAmbientColor;
		Color mDiffuseColor;
		Color mSpecularColor;
		float mSpecularCoefficient;
		float mAlpha;
		uint32 mId;
	};
}
