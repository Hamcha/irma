#include "Shader.h"

Shader::Shader(const char* shaderSource) {
	// Create the program
	program = glCreateProgram();

	// Create and compile the shader
	shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &shaderSource, 0);
	glCompileShader(shader);

	GLint length, result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		GLchar* log = new GLchar[length + 1];
		glGetShaderInfoLog(shader, length, &result, log);
		std::string logStr(log);
		delete log;
		throw ShaderCompilationException(logStr);
	}

	glAttachShader(program, shader);
	glLinkProgram(program);

	// Create Framebuffer
	GLuint fbo = 0;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Shader::Render(GLuint texture, int w, int h, int scale /* = 1 */) {
	// Bind and format the given render texture
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// Filtering!
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Set the renderTexture as the FBO target
	glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture, 0);
	GLenum DrawBuffers[2] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	// Render!
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glPushAttrib(GL_VIEWPORT_BIT);
	// Use the shader
	glUseProgram(program);

	// Global uniforms
	glUniform2f(glGetUniformLocation(program, "resolution"), w, h);

	glViewport(0, 0, w, h); // Viewport
	glRects(-1, -1, 1, 1);  // Main Rect

	// Disable shader
	glUseProgram(0);

	// De-bind the fbo
	glPopAttrib();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
}

ShaderCompilationException::ShaderCompilationException(std::string what) {
	reason = what;
}

std::string ShaderCompilationException::what() {
	return reason;
}