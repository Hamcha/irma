#include "Shader.h"

const GLchar* vshader[] =
{
	"varying vec2 LVertexPos2D;"
	"void main() {"
	"	gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 );"
	"}"
};


void Shader::Init() {
	// Create the program
	program = glCreateProgram();

	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vshader, NULL);
	glCompileShader(vertexShader);
	GLint length, result;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetShaderInfoLog(vertexShader, length, &result, log);
		std::string logStr(log);
		delete log;
		throw ShaderException(SHADER_COMPILE_FAILURE, "vertexShader", logStr);
	}

	glAttachShader(program, vertexShader);

	// Create Frame buffer and Render buffer
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rb);

	quad.Init();
}

void Shader::SetShader(const std::string shaderSource, const std::string shaderName) {
	//TODO Detach current shader (if anything)

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* src = shaderSource.c_str();
	const int srcLength = shaderSource.length();
	glShaderSource(fragmentShader, 1, &src, &srcLength);
	glCompileShader(fragmentShader);

	GLint length, result;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetShaderInfoLog(fragmentShader, length, &result, log);
		std::string logStr(log);
		delete log;
		throw ShaderException(SHADER_COMPILE_FAILURE, shaderName, logStr);
	}

	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glGetShaderiv(program, GL_LINK_STATUS, &result);
	if (result == GL_FALSE) {
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetProgramInfoLog(program, length, &result, log);
		std::string logStr(log);
		delete log;
		throw ShaderException(PROGRAM_LINK_FAILURE, "programLink", logStr);
	}

	vertAttrib = glGetAttribLocation(program, "LVertexPos2D");
}

void Shader::Render(const GLuint texture, const int w, const int h) {
	// Bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);

	// Bind and format the given render texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Filtering!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set the renderTexture as the FBO target
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	// Draw on texture
	Draw();

	// De-bind FBO, Render buffer and texture
	glPopAttrib();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Shader::Draw() {
	// Enable Shader
	glUseProgram(program);

	// Apply uniforms
	for (auto& item : uniforms) {
		GLint uniformLoc = GetUniform(item.first);
		item.second->Apply(uniformLoc);
	}

	// Render!
	quad.Draw(vertAttrib);

	// Disable shader
	glUseProgram(0);
}

GLint Shader::GetUniform(const std::string name) {
	if (uniformCache.find(name) == uniformCache.end()) {
		uniformCache[name] = glGetUniformLocation(program, name.c_str());
	}
	return uniformCache[name];
}

void Shader::Dispose() {
	for (auto& item : uniforms) {
		delete item.second;
	}
}