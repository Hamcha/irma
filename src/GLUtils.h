#ifndef _GLUTILS_H
#define _GLUTILS_H

#include <GL/glew.h>

template<typename T> struct GLVec2 { T x, y; };
template<typename T> struct GLVec3 { T x, y, z; };
template<typename T> struct GLVec4 { T x, y, z, w; };

class BaseUniform {
public:
	virtual void Apply(GLint location) {}
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