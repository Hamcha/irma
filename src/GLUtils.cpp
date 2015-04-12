#include "GLUtils.h"

void Uniform<GLint   >::Apply(GLint location) { glUniform1i(location, value); }
void Uniform<GLfloat >::Apply(GLint location) { glUniform1f(location, value); }
void Uniform<GLdouble>::Apply(GLint location) { glUniform1d(location, value); }

void Uniform<GLVec2i>::Apply(GLint location) { glUniform2i(location, value.x, value.y); }
void Uniform<GLVec2f>::Apply(GLint location) { glUniform2f(location, value.x, value.y); }
void Uniform<GLVec2d>::Apply(GLint location) { glUniform2d(location, value.x, value.y); }

void Uniform<GLVec3i>::Apply(GLint location) { glUniform3i(location, value.x, value.y, value.z); }
void Uniform<GLVec3f>::Apply(GLint location) { glUniform3f(location, value.x, value.y, value.z); }
void Uniform<GLVec3d>::Apply(GLint location) { glUniform3d(location, value.x, value.y, value.z); }

void Uniform<GLVec4i>::Apply(GLint location) { glUniform4i(location, value.x, value.y, value.z, value.w); }
void Uniform<GLVec4f>::Apply(GLint location) { glUniform4f(location, value.x, value.y, value.z, value.w); }
void Uniform<GLVec4d>::Apply(GLint location) { glUniform4d(location, value.x, value.y, value.z, value.w); }

void Uniform<Sampler2D>::Apply(GLint location) { 
	glActiveTexture(GL_TEXTURE0 + texid);
	glBindTexture(GL_TEXTURE_2D, value);
	glBindSampler(texid, samplerid);
	glUniform1i(location, texid); 
}