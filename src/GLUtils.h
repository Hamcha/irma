#pragma once

#include <GL/glew.h>

template<typename T> struct GLVec2 { T x, y; };
template<typename T> struct GLVec3 { T x, y, z; };
template<typename T> struct GLVec4 { T x, y, z, w; };

typedef GLVec2<GLint   > GLVec2i;
typedef GLVec2<GLfloat > GLVec2f;
typedef GLVec2<GLdouble> GLVec2d;
typedef GLVec3<GLint   > GLVec3i;
typedef GLVec3<GLfloat > GLVec3f;
typedef GLVec3<GLdouble> GLVec3d;
typedef GLVec4<GLint   > GLVec4i;
typedef GLVec4<GLfloat > GLVec4f;
typedef GLVec4<GLdouble> GLVec4d;
typedef GLuint           Sampler2D;

class BaseUniform {
public:
	virtual void Apply(GLint location) {
		throw "This shouldn't be called!";
	}
};

template<typename T>
class Uniform : public BaseUniform {
public:
	T value;
	GLuint texid, samplerid; // Used by samplers and textures

	Uniform() {}
	Uniform(T _val) { value = _val; }
	void Apply(GLint location);
};
