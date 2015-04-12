#ifndef _GLUTILS_H
#define _GLUTILS_H

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
	Uniform() {}
	Uniform(T _val) { value = _val; }
	void Apply(GLint location);
};

#endif