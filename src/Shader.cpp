#include "Shader.h"

Shader::Shader() {
	// Create the program
	program = glCreateProgram();

	const GLchar* vshader[] =
	{
		"#version 140\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vshader, NULL);
	glCompileShader(vertexShader);
	glAttachShader(program, vertexShader);

	// Create Frame buffer and Render buffer
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rb);
}

void Shader::SetShader(const char* shaderSource) {
	//TODO Detach current shader (if anything)

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderSource, NULL);
	glCompileShader(fragmentShader);

	GLint length, result;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetShaderInfoLog(fragmentShader, length, &result, log);
		std::string logStr(log);
		delete log;
		throw ShaderCompilationException(logStr);
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
		throw ShaderLinkException(logStr);
	}

	vertAttrib = glGetAttribLocation(program, "LVertexPos2D");
}

void Shader::Render(GLuint texture, int w, int h, int scale /* = 1 */) {
	// Bind framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w / scale, h / scale);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb);

	// Bind and format the given render texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w / scale, h / scale, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Filtering!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set the renderTexture as the FBO target
	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	// Enable Shader
	glPushAttrib(GL_VIEWPORT_BIT);
	glUseProgram(program);

	// Global uniforms
	glUniform2f(glGetUniformLocation(program, "uResolution"), w, h);

	// Render!
	quad.Draw(vertAttrib);

	// Disable shader
	glUseProgram(0);

	// De-bind FBO, Render buffer and texture
	glPopAttrib();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
}

ShaderCompilationException::ShaderCompilationException(std::string what) { reason = what; }
std::string ShaderCompilationException::what() { return reason; }

ShaderLinkException::ShaderLinkException(std::string what) { reason = what; }
std::string ShaderLinkException::what() { return reason; }