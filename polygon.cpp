#include "polygon.h"

#include <iostream>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

PoligonoPtr Poligono::Make()
{
	return PoligonoPtr(new Poligono());
}

Poligono::Poligono()
{
    float coord[] = {
       0.25f, 0,
       0.25f, 0.5f,
       -0.25f, 0.5f,
       -0.25f, 0,
    };

    // create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // create coord buffer
    GLuint id[1];
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coord), coord, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  // coord
    glEnableVertexAttribArray(0);
}

Poligono::~Poligono()
{
}

void Poligono::Draw()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}
