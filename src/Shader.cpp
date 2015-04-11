#include "Shader.h"

Shader::Shader() {
	// Create the program
	program = glCreateProgram();

	const char* vshader = "attribute vec4 vPosition; void main() { gl_Position = vPosition; }";

	// Create and compile the vertex shader
	vertexShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShader, 1, &vshader, 0);
	glCompileShader(vertexShader);

	// Create Framebuffer and Renderbuffer
	GLuint fbo = 0;
	glGenFramebuffers(1, &fbo);
	glGenRenderbuffers(1, &rb);
}

void Shader::SetShader(const char* shaderSource) {
	//TODO Detach current shader (if anything)

	// Create and compile the fragment shader
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &shaderSource, 0);
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

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

}

void Shader::Render(GLuint texture, int w, int h, int scale /* = 1 */) {
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

	// De-bind FBO, Render buffer and texture
	glPopAttrib();
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);
}

ShaderCompilationException::ShaderCompilationException(std::string what) {
	reason = what;
}

std::string ShaderCompilationException::what() {
	return reason;
}