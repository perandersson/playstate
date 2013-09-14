#include "../../memory/memory.h"
#include "ogl3_state_policy.h"
#include "ogl3_vertex_buffer.h"
#include "ogl3_index_buffer.h"
#include "../state/state_policy.h"
using namespace playstate;

void OGL3StatePolicy::BindIndexBuffer(OGL3IndexBuffer* indexBuffer)
{
	if(indexBuffer != NULL) {
		indexBuffer->Bind();
	} else {
		//_IIndexBuffer->Unbind();
	}
}

void OGL3StatePolicy::BindVertexBuffer(OGL3VertexBuffer* vertexBuffer)
{
	if(vertexBuffer != NULL) {
		vertexBuffer->Bind();
	} else {
		// Unbind?
	}
}

namespace playstate {
	const OGL3VertexBuffer* _vertexBuffer = 0;
	const OGL3IndexBuffer* _indexBuffer = 0;
}

void OGL3StatePolicyGuard::BindIndexBuffer(OGL3IndexBuffer* indexBuffer)
{
	if(_indexBuffer != indexBuffer) {
		OGL3StatePolicy::BindIndexBuffer(indexBuffer);
		_indexBuffer = indexBuffer;
	}
}

void OGL3StatePolicyGuard::BindVertexBuffer(OGL3VertexBuffer* vertexBuffer)
{
	if(_vertexBuffer != vertexBuffer) {
		_vertexBuffer = vertexBuffer;
		OGL3StatePolicy::BindVertexBuffer(vertexBuffer);
	}
}

void OGL3StatePolicyGuard::MarkAsDirty()
{
	_vertexBuffer = 0;
	_indexBuffer = 0;
	StatePolicyGuard::MarkAsDirty();
}
