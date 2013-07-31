#include "../../memory/memory.h"
#include "wavefront_resource_loader.h"
#include "../../math/vector3.h"
#include "../../resources/resource_manager.h"
#include "../../functions.h"
#include "../../uuid.h"

#include <vector>

using namespace playstate;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

WavefrontResourceLoader::WavefrontResourceLoader(ResourceManager& resourceManager, IFileSystem& fileSystem) 
	: mResourceManager(resourceManager), mFileSystem(fileSystem), mDefaultResource(NULL)
{
}

WavefrontResourceLoader::~WavefrontResourceLoader()
{
	if(mDefaultResource != NULL) {
		delete mDefaultResource;
		mDefaultResource = NULL;
	}
}

ResourceObject* WavefrontResourceLoader::Load(IFile& file)
{	
	std::istringstream dataStream = file.Read();
	std::string path = file.GetPath();

	Materials materials;
	
	AABB boundingBox;
	std::vector<WavefrontMesh*> meshes;

	std::string word;
	while(!dataStream.eof() && (dataStream >> word)) {
		if(word == "mtllib") {
			std::string objectName;
			dataStream >> objectName;
			std::auto_ptr<IFile> materialsFile = file.OpenFile(objectName);
			if(materialsFile->Exists()) {
				LoadMaterials(*materialsFile, materials);
			} 
			continue;
		} else if(word == "o") {
			std::string objectName;
			dataStream >> objectName;
			LoadMesh(dataStream, meshes, boundingBox);
		}
	}

	ModelMesh* meshesArray = new ModelMesh[meshes.size()];
	unsigned int size = meshes.size();
	for(unsigned int i = 0; i < size; ++i) {
		meshesArray[i].Vertices = meshes[i]->Vertices;
		meshesArray[i].Indices = NULL;
		meshesArray[i].SpecularCoefficient = 0.0f;
		meshesArray[i].Alpha = 1.0f;
		meshesArray[i].Id = UUID::To32Bit();

		Materials::iterator it = materials.find(meshes[i]->Material);
		if(it != materials.end()) {
			meshesArray[i].DiffuseTexture = it->second->DiffuseTexture;
			meshesArray[i].DiffuseColor = it->second->DiffuseColor;
		}

		delete meshes[i];
	}
	
	// Clean up materials resources - not needed anymore
	Materials::iterator it = materials.begin();
	Materials::const_iterator end = materials.end();
	for(; it != end; ++it) {
		delete it->second;
	}

	Model* model = new Model(boundingBox, meshes.size(), meshesArray);
	return model;
}

void WavefrontResourceLoader::LoadMaterials(IFile& file, Materials& materials) const
{
	std::istringstream dataStream = file.Read();

	WavefrontMaterial* currentMaterial = NULL;
	std::string word;
	while(!dataStream.eof() && (dataStream >> word)) {
		if(word == "newmtl") {
			if(currentMaterial != NULL)
				materials.insert(std::make_pair(currentMaterial->Name, currentMaterial));

			currentMaterial = new WavefrontMaterial();
			currentMaterial->Alpha = 1.0f;
			currentMaterial->AmbientColor = Color::White;
			currentMaterial->SpecularColor = Color::White;
			currentMaterial->DiffuseColor = Color::White;

			dataStream >> currentMaterial->Name;
			continue;
		} else if(word == "Ka") {
			dataStream >> currentMaterial->AmbientColor.Red;
			dataStream >> currentMaterial->AmbientColor.Green;
			dataStream >> currentMaterial->AmbientColor.Blue;
			continue;
		} else if(word == "Kd") {
			dataStream >> currentMaterial->DiffuseColor.Red;
			dataStream >> currentMaterial->DiffuseColor.Green;
			dataStream >> currentMaterial->DiffuseColor.Blue;
			continue;
		} else if(word == "Ks") {
			dataStream >> currentMaterial->SpecularColor.Red;
			dataStream >> currentMaterial->SpecularColor.Green;
			dataStream >> currentMaterial->SpecularColor.Blue;
			continue;
		} else if(word == "Ns") {
			dataStream >> currentMaterial->SpecularCoefficient;
			continue;
		} else if((word == "d") || (word == "Tr")) {
			dataStream >> currentMaterial->Alpha;
			continue;
		} else if(word == "map_Ka") {
			std::string ambientTexture;
			std::getline(dataStream, ambientTexture);
			if(ambientTexture.length() > 0) {
				currentMaterial->AmbientTexture = mResourceManager.GetResource<Texture2D>(ambientTexture);
			}
			continue;
		} else if(word == "map_Kd") {
			std::string diffuseTexture;
			dataStream >> diffuseTexture;
			if(diffuseTexture.length() > 0) {
				std::auto_ptr<IFile> textureFile = file.OpenFile(diffuseTexture);
				std::string path = "/defaults/texture2d.png";
				if(textureFile->Exists()) {
					path = textureFile->GetPath();
				}
				currentMaterial->DiffuseTexture = mResourceManager.GetResource<Texture2D>(path);
			}
			continue;
		} else if(word == "map_Ks") {
			std::string specularTexture;
			dataStream >> specularTexture;
			if(specularTexture.length() > 0) {
				currentMaterial->SpecularTexture = mResourceManager.GetResource<Texture2D>(specularTexture);
			}
			continue;
		} else if(word == "map_Ns") {
			std::string specularHighlightTexture;
			std::getline(dataStream, specularHighlightTexture);
			if(specularHighlightTexture.length() > 0) {
				currentMaterial->SpecularHighlightTexture = mResourceManager.GetResource<Texture2D>(specularHighlightTexture);
			}
			continue;
		} else if((word == "map_d") || (word == "map_Tr")) {
			std::string alphaTexture;
			std::getline(dataStream, alphaTexture);
			if(alphaTexture.length() > 0) {
				currentMaterial->AlphaTexture = mResourceManager.GetResource<Texture2D>(alphaTexture);
			}
			continue;
		} else if(word == "bump") {
			std::string bumpMap;
			std::getline(dataStream, bumpMap);
			if(bumpMap.length() > 0) {
				currentMaterial->BumpMap = mResourceManager.GetResource<Texture2D>(bumpMap);
			}
			continue;
		} else if(word == "disp") {
			std::string displacementMap;
			std::getline(dataStream, displacementMap);
			if(displacementMap.length() > 0) {
				currentMaterial->DisplacementMap = mResourceManager.GetResource<Texture2D>(displacementMap);
			}
			continue;
		} else if(word == "decal") {
			// TODO??
			continue;
		}
	}

	if(currentMaterial != NULL) {
		materials.insert(std::make_pair(currentMaterial->Name, currentMaterial));
	}
}

ResourceObject* WavefrontResourceLoader::GetDefaultResource()
{
	if(mDefaultResource == NULL) {
		std::auto_ptr<IFile> file = mFileSystem.OpenFile("/engine/defaults/wavefront_default.obj");
		assert(file->Exists() && "The default resource must exist");
		mDefaultResource = Load(*file);
	}

	return mDefaultResource;
}

bool WavefrontResourceLoader::IsThreadable() const
{
	return true;
}

void WavefrontResourceLoader::LoadMesh(std::istringstream& stream, std::vector<WavefrontMesh*>& meshes, AABB& boundingBox) const
{
	std::vector<Vector3> vertices;
	std::vector<Vector3> normals;
	std::vector<Vector2> texCoords;
	std::vector<PositionNormalTextureData> items;

	PositionNormalTextureData data[10000];
	PositionNormalTextureData* ptr = data;
	unsigned int numVertices = 0;
	
	float maxWidth = -FLT_MAX;
	float maxDepth = -FLT_MAX;
	float maxHeight = -FLT_MAX;
	float minWidth = FLT_MAX;
	float minDepth = FLT_MAX;
	float minHeight = FLT_MAX;

	std::string currentMaterial;
	std::string word;
	while(!stream.eof() && (stream >> word)) {
		if(word == "v") {
			Vector3 vertex;
			stream >> vertex.X;
			stream >> vertex.Y;
			stream >> vertex.Z;
			vertices.push_back(vertex);
			
			if(vertex.X > maxWidth)
				maxWidth = vertex.X;

			if(vertex.X < minWidth)
				minWidth = vertex.X;

			if(vertex.Y > maxHeight)
				maxHeight = vertex.Y;

			if(vertex.Y < minHeight)
				minHeight = vertex.Y;

			if(vertex.Z > maxDepth)
				maxDepth = vertex.Z;

			if(vertex.Z < minDepth)
				minDepth = vertex.Z;

		} else if(word == "vt") {
			Vector2 texCoord;
			stream >> texCoord.X;
			stream >> texCoord.Y;
			texCoords.push_back(texCoord);
		} else if(word == "vn") {
			Vector3 normal;
			stream >> normal.X;
			stream >> normal.Y;
			stream >> normal.Z;
			normals.push_back(normal);
		} else if(word == "usemtl") {
			if(currentMaterial.length() > 0) {
				WavefrontMesh* mesh = new WavefrontMesh();
				mesh->Material = currentMaterial;
				mesh->Vertices = VertexBuffer::CreateStatic(data, numVertices);
				meshes.push_back(mesh);
				
				numVertices = 0;
				ptr = data;
			}

			stream >> currentMaterial;
		} else if(word == "f") {
			for(int i = 0; i < 3; ++i) {
				int v = -1, vt = -1, vn = -1;

				std::string face;
				if(!(stream >> face))
					continue;

				std::vector<std::string> parts = Split(face, '/');
				std::stringstream ss(parts[0]);
				
				ss >> v;
				v--;

				if(parts.size() > 1) {
					ss = std::stringstream(parts[1]);

					ss >> vt;
					vt--;
				}
				
				if(parts.size() > 2) {
					ss = std::stringstream(parts[2]);

					ss >> vn;
					vn--;
				}

				ptr->Position = vertices[v];
				ptr->TexCoord = texCoords[vt];
				ptr->Normal = normals[vn];
				
				ptr++;
				numVertices++;
			}
		}
	}

	if(currentMaterial.length() > 0) {
		WavefrontMesh* mesh = new WavefrontMesh();
		mesh->Material = currentMaterial;
		mesh->Vertices = VertexBuffer::CreateStatic(data, numVertices);
		meshes.push_back(mesh);
	}

	boundingBox = AABB(Vector3::Zero, maxWidth - minWidth, maxHeight - minHeight, maxDepth - minDepth);
}


	/*	
void WavefrontResourceLoader::CreateWavefrontModelMesh(IVertexBuffer* vertexBuffer, std::vector<ModelMesh*>& meshes, const WavefrontMaterial* material) const
{
	ModelMesh* mesh = new ModelMesh(vertexBuffer, NULL, material->DiffuseTexture, material->DiffuseColor);
	meshes.push_back(mesh);

	if(!material->AmbientTexture.empty())
		mesh->SetAmbientTexture(mResourceManager.GetResource<ITexture2D>(material->AmbientTexture));
	
	if(!material->SpecularTexture.empty())
		mesh->SetSpecularTexture(mResourceManager.GetResource<ITexture2D>(material->SpecularTexture));
	
	if(!material->SpecularHighlightTexture.empty())
		mesh->SetSpecularHighlightTexture(mResourceManager.GetResource<ITexture2D>(material->SpecularHighlightTexture));
	
	if(!material->AlphaTexture.empty())
		mesh->SetAlphaTexture(mResourceManager.GetResource<ITexture2D>(material->AlphaTexture));
	
	if(!material->BumpMap.empty())
		mesh->SetBumpMap(mResourceManager.GetResource<ITexture2D>(material->BumpMap));
	
	if(!material->DisplacementMap.empty())
		mesh->SetDisplacementMap(mResourceManager.GetResource<ITexture2D>(material->DisplacementMap));

	mesh->SetAmbientColor(material->AmbientColor);
	mesh->SetDiffuseColor(material->DiffuseColor);
	mesh->SetSpecularColor(material->SpecularColor);
	mesh->SetSpecularCoefficient(material->SpecularCoefficient);
	mesh->SetAlpha(material->Alpha);
	
	model->AddMesh(mesh);
}
	*/
