#pragma once
#pragma once

#include <memory>

#ifdef _WIN32
#include <glad/glad.h>
#else
#include <OpenGL/gl3.h>
#endif

class Disk;

using DiskPtr = std::shared_ptr<Disk>;

class Disk {
public:
    static DiskPtr Make(int segmentos);
    ~Disk();

    void Draw();

private:
    Disk(int segmentos);

    GLuint m_vao;
    int numVertices;
};