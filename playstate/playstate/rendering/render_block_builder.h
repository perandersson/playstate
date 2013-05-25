#pragma once

#include "../types.h"
#include "../search/find_result_set.h"
#include "../math/color.h"

#include <vector>
#include <deque>

namespace playstate
{
	class VertexBuffer;
	class IndexBuffer;
	class Texture2D;

	//
	// This block contains all the data neccessary for the pipeline
	// to draw the object onto the screen.
	struct RenderBlock
	{
		uint32 Id;
		VertexBuffer* VertexBuffer;
		IndexBuffer* IndexBuffer;
		Matrix4x4 ModelMatrix;
		Texture2D* DiffuseTexture;
		Color DiffuseColor;
		const Texture2D* AmbientTexture;
		const Texture2D* SpecularTexture;
		const Texture2D* SpecularHighlightTexture;
		const Texture2D* AlphaTexture;
		const Texture2D* BumpMapTexture;
		const Texture2D* DisplacementTexture;
	};
	
	class RenderBlockBuilder
	{
	public:
		RenderBlockBuilder();
		~RenderBlockBuilder();

		//
		// Sort and save the supplied render blocks
		bool SortAndSave(FindResultSet<RenderBlock>* target);

		//
		// Allocate a new RenderBlock instance and return it
		// @return A valid RenderBlock instance pointer.
		RenderBlock* NewBlock(uint32 id);

		//
		// Clean up the allocated memory
		void Clean();

	private:
		void QuickSort(RenderBlock** arr, int32 left, int32 right);

	private:
		uint32 mNumBlocks;
		RenderBlock* mBlocks;
	};

}
