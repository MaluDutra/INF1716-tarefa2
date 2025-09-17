#include "disk.h"
#include <vector>
#include <iostream>

DiskPtr Disk::Make(int segmentos) {
    return DiskPtr(new Disk(segmentos));
}

Disk::Disk(int segmentos) : numVertices(segmentos + 2) 
{
    if (segmentos < 3) {
        segmentos = 3;
    }

    std::vector<float> coord;

    coord.push_back(0.0f); 
    coord.push_back(0.0f);      

    float raio = 1.0f;
    for (int i = 0; i <= segmentos; ++i) 
    {
        float angulo = 2.0f * 3.141592653589793 * static_cast<float>(i) / static_cast<float>(segmentos);
        float x = raio * std::cos(angulo);
        float y = raio * std::sin(angulo);
        coord.push_back(x);
        coord.push_back(y);
    }

    // create VAO
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    // create coord buffer
    GLuint id[1];
    glGenBuffers(1, id);
    glBindBuffer(GL_ARRAY_BUFFER, id[0]);
    glBufferData(GL_ARRAY_BUFFER, coord.size() * sizeof(float), coord.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);  // coord
    glEnableVertexAttribArray(0);
}

Disk::~Disk() {
    glDeleteVertexArrays(1, &m_vao);
}

void Disk::Draw() {
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, numVertices);
}