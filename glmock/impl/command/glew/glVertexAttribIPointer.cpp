#include "../../../glmock.h"
using namespace glmock;

extern "C" {
	#undef glVertexAttribIPointer
	void CALL_CONV glVertexAttribIPointer(GLuint index, GLint size, GLenum type, GLsizei stride, const GLvoid* pointer) {
	}
	DLL_EXPORT PFNGLVERTEXATTRIBIPOINTERPROC __glewVertexAttribIPointer = &glVertexAttribIPointer;
}
