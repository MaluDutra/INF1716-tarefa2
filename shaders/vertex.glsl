#version 410

layout (location = 0) in vec4 vertex;

uniform mat4 M;
uniform vec4 icolor;

out vec4 color;

void main (void)
{
  color = icolor;
  gl_Position = M * vertex;
}

