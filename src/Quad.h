#ifndef _QUAD_H
#define _QUAD_H

#include <GL/glew.h>

class Quad {
private:
	GLuint vbo, ibo;
public:
	void Init(const float x = 0, const float y = 0, const float w = 1, const float h = 1);
	void Draw();
	void Draw(const GLuint vertexAttrib);
};

#endif