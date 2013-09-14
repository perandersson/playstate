#pragma once

#include "../../resources/resource_loader.h"
#include "../../rendering/render_system.h"
#include "../../filesystem/file_system.h"
#include "../model.h"

#include <hash_map>
#include <stdio.h>

namespace playstate
{
	class WavefrontResourceLoader : public IResourceLoader
	{	
		struct WavefrontMaterial
		{
			playstate::string Name;
			Resource<ITexture2D> DiffuseTexture;
			Resource<ITexture2D> AmbientTexture;
			Resource<ITexture2D> SpecularTexture;
			Resource<ITexture2D> SpecularHighlightTexture;
			Resource<ITexture2D> AlphaTexture;
			Resource<ITexture2D> BumpMap;
			Resource<ITexture2D> DisplacementMap;
			Color AmbientColor;
			Color DiffuseColor;
			Color SpecularColor;
			float SpecularCoefficient;
			float Alpha;
		};

		struct WavefrontMesh
		{
			playstate::string Material;
			IVertexBuffer* Vertices;
		};

		typedef std::hash_map<playstate::string, WavefrontMaterial*> Materials;

	public:
		WavefrontResourceLoader(IResourceManager& resourceManager, IFileSystem& fileSystem, IRenderSystem& renderSystem);
		~WavefrontResourceLoader();

	// IResourceLoader
	public:
		virtual ResourceObject* Load(IFile& file);
		virtual ResourceObject* GetDefaultResource();
		virtual bool IsThreadable() const;

	private:
		//
		// Loads the materials from the supplied file and puts the result into the materials hash map
		// @param file the file to the materials file.
		// @param materials the container map where the materials are to be put.
		void LoadMaterials(IFile& file, Materials& materials) const;

		//
		// Loads a mesh from an input stream and puts the result into the meshes list.
		void LoadMesh(std::istringstream& stream, std::vector<WavefrontMesh*>& meshes, AABB& boundingBox) const;

	private:
		IResourceManager& mResourceManager;
		IFileSystem& mFileSystem;
		IRenderSystem& mRenderSystem;

		ResourceObject* mDefaultResource;
	};
}
