#include "GLUtils.h"

void Uniform<GLint   >::Apply(GLint location) { glUniform1i(location, value); }
void Uniform<GLfloat >::Apply(GLint location) { glUniform1f(location, value); }
void Uniform<GLdouble>::Apply(GLint location) { glUniform1d(location, value); }

void Uniform<GLVec2<GLint   >>::Apply(GLint location) { glUniform2i(location, value.x, value.y); }
void Uniform<GLVec2<GLfloat >>::Apply(GLint location) { glUniform2f(location, value.x, value.y); }
void Uniform<GLVec2<GLdouble>>::Apply(GLint location) { glUniform2d(location, value.x, value.y); }

void Uniform<GLVec3<GLint   >>::Apply(GLint location) { glUniform3i(location, value.x, value.y, value.z); }
void Uniform<GLVec3<GLfloat >>::Apply(GLint location) { glUniform3f(location, value.x, value.y, value.z); }
void Uniform<GLVec3<GLdouble>>::Apply(GLint location) { glUniform3d(location, value.x, value.y, value.z); }

void Uniform<GLVec4<GLint   >>::Apply(GLint location) { glUniform4i(location, value.x, value.y, value.z, value.w); }
void Uniform<GLVec4<GLfloat >>::Apply(GLint location) { glUniform4f(location, value.x, value.y, value.z, value.w); }
void Uniform<GLVec4<GLdouble>>::Apply(GLint location) { glUniform4d(location, value.x, value.y, value.z, value.w); }

void Uniform<Sampler2D>::Apply(GLint location) { 
	glActiveTexture(GL_TEXTURE0 + texid);
	glBindTexture(GL_TEXTURE_2D, value);
	glBindSampler(texid, samplerid);
	glUniform1i(location, texid); 
}