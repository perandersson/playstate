#include "../../memory/memory.h"
#include "gfx_program_factory.h"
#include "../gfx_program.h"
#include "../exception/gfx_program_exception.h"
using namespace playstate;

GfxProgramFactory::GfxProgramFactory(RenderSystem& renderSystem, ScriptSystem& scriptSystem)
	: mRenderSystem(renderSystem), mScriptSystem(scriptSystem)
{
}

GfxProgramFactory::~GfxProgramFactory()
{
}

GfxProgram* GfxProgramFactory::Create(const playstate::string& fileName)
{
	std::auto_ptr<Script> script = mScriptSystem.CompileFile(fileName);
	ScriptCollection c = script->ReadCollection();
	
	playstate::string vertexShader = c.FindString("VertexShader");
	playstate::string fragmentShader = c.FindString("FragmentShader");
	playstate::string geometryShader = c.FindString("GeometryShader");

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	const char* vv = vertexShader.c_str();
	glShaderSource(vs, 1, &vv, NULL);
	glCompileShader(vs);

	GLint status = 0;
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(!status) {
		GLchar infoLogg[2048];
		glGetShaderInfoLog(vs, 2048, NULL, infoLogg);
		glDeleteShader(vs);

		THROW_EXCEPTION(GfxProgramException, "Could not compile vertex shader for resource: '%s'. Reason: %s", 
			fileName.c_str(), infoLogg);
	}

	
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* ff = fragmentShader.c_str();
	glShaderSource(fs, 1, &ff, NULL);
	glCompileShader(fs);

	status = 0;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(!status) {
		GLchar infoLogg[2048];
		glGetShaderInfoLog(fs, 2048, NULL, infoLogg);
		glDeleteShader(vs);
		glDeleteShader(fs);

		THROW_EXCEPTION(GfxProgramException, "Could not compile fragment shader for resource: '%s'. Reason: %s", 
			fileName.c_str(), infoLogg);
	}

	GLuint gs = 0;
	
	if(geometryShader.length() > 0) {
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gg = geometryShader.c_str();
		glShaderSource(gs, 1, &gg, NULL);
		glCompileShader(gs);

		status = 0;
		glGetShaderiv(gs, GL_COMPILE_STATUS, &status);
		if(!status) {
			GLchar infoLogg[2048];
			glGetShaderInfoLog(fs, 2048, NULL, infoLogg);
			glDeleteShader(gs);
			glDeleteShader(vs);
			glDeleteShader(fs);

			THROW_EXCEPTION(GfxProgramException, "Could not compile geometry shader for resource: '%s'. Reason: %s", 
				fileName.c_str(), infoLogg);
		}
	}

	GLuint program = glCreateProgram();
	if(gs != 0) 
		glAttachShader(program, gs);
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// Detach the shaders when done
	// @see http://www.opengl.org/wiki/GLSL_Object
	glDetachShader(program, fs);
	glDeleteShader(fs);
	glDetachShader(program, vs);
	glDeleteShader(vs);
	if(gs != 0) {
		glDetachShader(program, gs);
		glDeleteShader(gs);
	}

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	GLchar infoLogg[2048] = {0};
	glGetProgramInfoLog(program, sizeof(infoLogg) - 1, NULL, infoLogg);
	if(!status) {
		glDeleteShader(vs);
		glDeleteShader(fs);
		glDeleteProgram(program);

		THROW_EXCEPTION(GfxProgramException, "Could not link the shaders programs for resource: '%s'. Reason: %s", 
			fileName.c_str(), infoLogg);
	}

	if(infoLogg != NULL && strlen(infoLogg) > 40) {
		ILogger::Get().Debug("Shader program was linked, and is working, but had build messages:\n%s", infoLogg);
	}

	// Load all attributes

	GfxProgram* g = new GfxProgram(program, mRenderSystem, c);
	
	bool blend = c.FindBool("Blend", false);
	bool depthTest = c.FindBool("DepthTest", true);
	CullFaces::Enum cullFaces = (CullFaces::Enum)c.FindInt("CullFaces", CullFaces::Default);
	DepthFunc::Enum depthFunc = (DepthFunc::Enum)c.FindInt("DepthFunc", DepthFunc::Default);
	SrcFactor::Enum srcFactor = (SrcFactor::Enum)c.FindInt("SrcFactor", SrcFactor::Default);
	DestFactor::Enum destFactor = (DestFactor::Enum)c.FindInt("DestFactor", DestFactor::Default);

	g->EnableBlend(blend);
	g->EnableDepthTest(depthTest);
	g->SetCullFaces(cullFaces);
	g->SetDepthFunc(depthFunc);
	g->SetBlendFunc(srcFactor, destFactor);

	return g;
}
