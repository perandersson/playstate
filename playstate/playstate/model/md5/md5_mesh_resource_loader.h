#pragma once

#include "../../resources/resource_loader.h"
#include "../../filesystem/file_system.h"
#include "../../resources/resource_manager.h"
#include "../../rendering/vertex_buffer.h"
#include "../../rendering/index_buffer.h"
#include "../../rendering/vertex_declaration.h"
#include "../../rendering/datatypes/standard_vertex_buffer_types.h"
#include "../../rendering/render_system.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"
#include "../../math/quaternion.h"

namespace playstate
{
	struct MD5MeshVertexData
	{
		Vector3 Position;
		Vector3 Normal;
		Vector2 TexCoord;
		uint32 bone;
		float32 weight;
	};

	static const VertexDeclaration MD5MeshVertexDataVertexDeclaration = {
		{
			{ PositionAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, false }, 
			{ NormalAttribLocation, sizeof(Vector3), 3, VertexElementType::FLOAT, true },
			{ TexCoordAttribLocation, sizeof(Vector2), 2, VertexElementType::FLOAT, false },
			{ 3, sizeof(uint32), 1, VertexElementType::UNSIGNED_INT, false },
			{ 4, sizeof(float32), 1, VertexElementType::FLOAT, false },
			0
		}
	};
	static const VertexDeclarationArrayObjectFactory MD5MeshVertexDataVAOFactory(MD5MeshVertexDataVertexDeclaration);

	class MD5MeshResourceLoader : public IResourceLoader
	{
		struct MD5MeshVertex
		{
			Vector2 TexCoord;
			uint32 StartWeight;
			uint32 WeightCount;
		};

		struct MD5MeshTris
		{
			union 
			{
				struct {
					uint32 P1;
					uint32 P2;
					uint32 P3;
				};
				uint32 Points[3];
			};
		};

		struct MD5MeshWeight
		{
			uint32 JointID;
			float32 Bias;
			Vector3 Position;
		};

		struct MD5MeshJoint
		{
			playstate::character Name[64];
			int32 ParentID;
			Vector3 Position;
			Quaternion Orient;
		};

		struct MD5Mesh
		{
			MD5MeshVertex* Vertices;
			uint32 NumVertices;

			MD5MeshTris* Triangles;
			uint32 NumTriangles;

			MD5MeshWeight* Weights;
			uint32 NumWeights;
		};

	public:
		MD5MeshResourceLoader(IFileSystem& fileSystem, IResourceManager& resourceManager, IRenderSystem& renderSystem);
		virtual ~MD5MeshResourceLoader();

	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		MD5Mesh* ReadMesh(std::istringstream& stream);
		MD5MeshJoint* ReadJoints(std::istringstream& stream, uint32 numJoints);
		IVertexBuffer* GenerateIVertexBuffer(MD5Mesh* mesh, MD5MeshJoint* joints);
		IIndexBuffer* GenerateIIndexBuffer(MD5Mesh* mesh);

	private:
		IFileSystem& mFileSystem;
		IResourceManager& mResourceManager;
		IRenderSystem& mRenderSystem;
		ResourceObject* mDefaultResource;
	};
}
