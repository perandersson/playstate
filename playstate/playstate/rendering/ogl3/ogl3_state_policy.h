#pragma once

namespace playstate
{
	class OGL3VertexBuffer;
	class OGL3IndexBuffer;

	class OGL3StatePolicy
	{
	public:
		static void BindIndexBuffer(OGL3IndexBuffer* indexBuffer);
		static void BindVertexBuffer(OGL3VertexBuffer* vertexBuffer);
	};

	class OGL3StatePolicyGuard
	{
	public:
		static void BindIndexBuffer(OGL3IndexBuffer* indexBuffer);
		static void BindVertexBuffer(OGL3VertexBuffer* vertexBuffer);

		//
		// Mark all states as dirty.
		static void MarkAsDirty();
	};
}
