#include "../../memory/memory.h"
#include "md5_mesh_resource_loader.h"
#include "../dynamic_model.h"
using namespace playstate;

MD5MeshResourceLoader::MD5MeshResourceLoader(IFileSystem& fileSystem, IResourceManager& resourceManager, IRenderSystem& renderSystem)
	: mFileSystem(fileSystem), mResourceManager(resourceManager), mRenderSystem(renderSystem), mDefaultResource(NULL)
{
}

MD5MeshResourceLoader::~MD5MeshResourceLoader()
{
	if(mDefaultResource != NULL) {
		delete mDefaultResource;
		mDefaultResource = NULL;
	}
}

ResourceObject* MD5MeshResourceLoader::Load(IFile& file)
{
	std::istringstream dataStream = file.Read();
	playstate::string path = file.GetPath();

	MD5MeshJoint* joints = NULL;
	uint32 numJoints = 0;
	uint32 numMeshes = 0;

	ModelMesh* meshes = NULL;
	uint32 meshCount = 0;

	playstate::string word;
	playstate::string junk;
	do {
		dataStream >> word;
		if(word == "MD5Version") {
			uint32 id;
			dataStream >> id;
			if(id != 10) {
				THROW_EXCEPTION(LoadResourceException, "Could not load resource: '%s'. Invalid MD5Version header", path.c_str());
			}
		} else if(word == "commandLine") {
			dataStream >> junk;
		} else if(word == "numJoints") {
			dataStream >> numJoints;
		} else if(word == "numMeshes") {
			dataStream >> numMeshes;
			assert(numMeshes > 0);
			meshes = new ModelMesh[numMeshes];
		} else if(word == "joints") {
			assert(numJoints > 0 && "Why are no joints in the mesh?");
			dataStream >> junk; // Load the '{'
			joints = ReadJoints(dataStream, numJoints);
			assert(joints != NULL);
		} else if(word == "mesh") {
			dataStream >> junk; // Load the '{'

			MD5Mesh* mesh = ReadMesh(dataStream);
			assert(mesh != NULL);

			IVertexBuffer* IVertexBuffer = GenerateIVertexBuffer(mesh, joints);
			IIndexBuffer* IIndexBuffer = GenerateIIndexBuffer(mesh);
			
			// Save neccessary Bone and BB information (so that we can use them from our application as well).
			ModelMesh& modelMesh = meshes[meshCount++];
			modelMesh.Vertices = IVertexBuffer;
			modelMesh.Indices = IIndexBuffer;
			modelMesh.SpecularCoefficient = 0.0f;
			modelMesh.Alpha = 1.0f;
			modelMesh.Id = UUID::To32Bit();

			// Cleanup memory
			delete[] mesh->Triangles;
			delete[] mesh->Vertices;
			delete[] mesh->Weights;
			delete mesh;
		}
		word.clear();
	} while(!dataStream.eof());
	
	delete[] joints;

	return new DynamicModel(numMeshes, meshes);
}

MD5MeshResourceLoader::MD5Mesh* MD5MeshResourceLoader::ReadMesh(std::istringstream& stream)
{
	MD5Mesh* mesh = new MD5Mesh;
	
	playstate::string junk;
	playstate::string part;
	playstate::string shader;
	do {
		stream >> part;
		if(part == "shader") {
			stream >> shader; // shader filename (contains materials, such as textures etc.)
		} else if(part == "numverts") {
			uint32 numVerts = 0;
			stream >> numVerts;
			mesh->NumVertices = numVerts;
			mesh->Vertices = new MD5MeshVertex[numVerts];
			// Vertex buffer. Positions are based on the weights and their corresponding joint positions.
			for(uint32 i = 0; i < numVerts; ++i) {
				uint32 vertexIndex = 0;
				stream >> junk >> vertexIndex;
				MD5MeshVertex& v = mesh->Vertices[vertexIndex];
				stream >> junk >> v.TexCoord.S >> v.TexCoord.T >> junk >> v.StartWeight >> v.WeightCount;
			}
		} else if(part == "numtris") {
			uint32 numTris = 0;
			stream >> numTris;
			mesh->NumTriangles = numTris;
			mesh->Triangles = new MD5MeshTris[numTris];
			for(uint32 i = 0; i < numTris; ++i) {
				uint32 triIndex = 0;
				stream >> junk >> triIndex;
				MD5MeshTris& triangle = mesh->Triangles[triIndex];
				stream >> triangle.P1 >> triangle.P2 >> triangle.P3;
			}
		} else if(part == "numweights") {
			uint32 numWeights = 0;
			stream >> numWeights;
			mesh->NumWeights = numWeights;
			mesh->Weights = new MD5MeshWeight[numWeights];

			for(uint32 i = 0; i < numWeights; ++i) {
				uint32 weightIndex = 0; // necessary?
				stream >> junk >> weightIndex;
				MD5MeshWeight& weight = mesh->Weights[weightIndex];

				stream >> weight.JointID >> weight.Bias >> 
					junk >> weight.Position.X >> weight.Position.Y >> weight.Position.Z >> junk;
			}
		}
	} while(part != "}");
	
	return mesh;
}

MD5MeshResourceLoader::MD5MeshJoint* MD5MeshResourceLoader::ReadJoints(std::istringstream& stream, uint32 numJoints)
{
	MD5MeshJoint* joints = new MD5MeshJoint[numJoints];
	playstate::string junk;
	for(uint32 i = 0; i < numJoints; ++i) {
		MD5MeshJoint& joint = joints[i];
		stream >> joint.Name >> joint.ParentID >> junk >> joint.Position.X >> joint.Position.Y >> joint.Position.Z
			>> junk >> junk >> joint.Orient.X >> joint.Orient.Y >> joint.Orient.Z >> junk;

		// Calculate W. This only works if the rest of the vector is normalized
		float32 t = 1.0f - (joint.Orient.X * joint.Orient.X) - (joint.Orient.Y * joint.Orient.Y)
			- (joint.Orient.Z * joint.Orient.Z);
		if(t < 0.0f) {
			joint.Orient.W = 0.0f;
		} else {
			joint.Orient.W = -sqrtf(t);
		}

		std::getline(stream, junk);
	}
	stream >> junk; // }
	return joints;
}

IVertexBuffer* MD5MeshResourceLoader::GenerateIVertexBuffer(MD5Mesh* mesh, MD5MeshJoint* joints)
{
	const uint32 numVertices = mesh->NumVertices;
	MD5MeshVertexData* IVertexBufferData = new MD5MeshVertexData[mesh->NumVertices];

	//
	// Positions
	//

	for(uint32 i = 0; i < numVertices; ++i) {
		MD5MeshVertex& meshVertex = mesh->Vertices[i];
		MD5MeshVertexData& data = IVertexBufferData[i];
		data.bone = 0;
		data.weight = 0.0f;
		data.Position = Vector3::Zero;
		data.Normal = Vector3::Zero;

		for(uint32 weightIndex = 0; weightIndex < meshVertex.WeightCount; ++weightIndex) {
			const MD5MeshWeight& weight = mesh->Weights[meshVertex.StartWeight + weightIndex];
			const MD5MeshJoint& joint = joints[weight.JointID];
			
            // Convert the weight position from Joint local space to object space
			const Vector3 rotPos = joint.Orient * weight.Position;
			data.Position += (joint.Position + rotPos) * weight.Bias;
		}
	}

	//
	// Normals
	//
	const uint32 numTriangles = mesh->NumTriangles;
	for(uint32 i = 0; i < numTriangles; ++i) {
		const Vector3& v0 = IVertexBufferData[ mesh->Triangles[i].P1 ].Position;
		const Vector3& v1 = IVertexBufferData[ mesh->Triangles[i].P2 ].Position;
		const Vector3& v2 = IVertexBufferData[ mesh->Triangles[i].P3 ].Position;

		const Vector3 normal = (v2 - v0).CrossProduct(v1 - v0);
		
		IVertexBufferData[ mesh->Triangles[i].P1 ].Normal += normal;
		IVertexBufferData[ mesh->Triangles[i].P2 ].Normal += normal;
		IVertexBufferData[ mesh->Triangles[i].P3 ].Normal += normal;
	}

	for(uint32 i = 0; i < numVertices; ++i) {
		MD5MeshVertexData& data = IVertexBufferData[i];
		data.Normal.Normalize();
	}

	IVertexBuffer* vb = mRenderSystem.CreateStaticBuffer(IVertexBufferData, sizeof(MD5MeshVertexData), MD5MeshVertexDataVertexDeclaration, mesh->NumVertices);
	delete[] IVertexBufferData;
	return vb;
}

IIndexBuffer* MD5MeshResourceLoader::GenerateIIndexBuffer(MD5Mesh* mesh)
{
	//uint32* indices = new uint32[mesh->NumTriangles * 3];
//	memcpy(indices, mesh->Triangles, sizeof(uint32) * mesh->NumTriangles * 3);
	uint32* indices = mesh->Triangles[0].Points;
	return mRenderSystem.CreateStaticBuffer(indices, mesh->NumTriangles * 3);
}

ResourceObject* MD5MeshResourceLoader::GetDefaultResource()
{
	if(mDefaultResource == NULL) {
		std::auto_ptr<IFile> file = mFileSystem.OpenFile("/engine/defaults/md5_mesh.md5mesh");
		assert(file->Exists() && "The default resource must exist");
		mDefaultResource = Load(*file);
	}

	return mDefaultResource;
}

bool MD5MeshResourceLoader::IsThreadable() const
{
	return true;
}
