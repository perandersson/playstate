#include "../memory/memory.h"
#include "model_mesh.h"
#include "../uuid.h"

using namespace playstate;

ModelMesh::ModelMesh()
	: mVertices(NULL), mIndices(NULL), mSpecularCoefficient(0.0f), mAlpha(1.0f), mId(UUID::To32Bit()),
	Vertices(mVertices), Indices(mIndices),
	DiffuseTexture(mDiffuseTexture), AmbientTexture(mAmbientTexture),
	SpecularTexture(mSpecularTexture), SpecularHighlightTexture(mSpecularHighlightTexture),
	AlphaTexture(mAlphaTexture), BumpMap(mBumpMap), DisplacementTexture(mDisplacementTexture),
	AmbientColor(mAmbientColor), DiffuseColor(mDiffuseColor), SpecularColor(mSpecularColor),
	Alpha(mAlpha), SpecularCoefficient(mSpecularCoefficient), Id(mId)
{
}

ModelMesh::~ModelMesh()
{
	if(mVertices != NULL) {
		delete mVertices;
		mVertices = NULL;
	}

	if(mIndices != NULL) {
		delete mIndices;
		mIndices = NULL;
	}
}

void ModelMesh::SetVertices(VertexBuffer* vertices)
{
	if(mVertices != NULL)
		delete mVertices;

	mVertices = vertices;
}

void ModelMesh::SetIndices(IndexBuffer* indices)
{
	if(mIndices != NULL)
		delete mIndices;

	mIndices = mIndices;
}

void ModelMesh::SetDiffuseColor(const Color& diffuseColor)
{
	mDiffuseColor = diffuseColor;
}

void ModelMesh::SetDiffuseTexture(Resource<Texture2D> diffuseTexture)
{
	mDiffuseTexture = diffuseTexture;
}

