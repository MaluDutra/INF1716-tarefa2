#define _CRT_SECURE_NO_WARNINGS

#ifdef _WIN32
#include <windows.h>

// Use glad to initialize OpenGL context on Windows
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#else

#include <OpenGL/gl3.h>
#include <GLFW/glfw3.h>

#endif

#include "error.h"
#include "polygon.h"
#include "disk.h"
#include "shader.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <chrono>
#include <ctime>

static PoligonoPtr ponteiros;
static PoligonoPtr linhas;
static DiskPtr circulo;
static ShaderPtr shd;

float anguloSegundos = 0;
float anguloMinutos = 0;
float anguloHoras = 0;

void desenhaPonteiros(float dt)
{
    // inicializa as matrizes
    glm::mat4 MSegundos(1.0f);
    glm::mat4 MMinutos(1.0f);
    glm::mat4 MHoras(1.0f);
    glm::mat4 MCirculoMeio(1.0f);

    // realiza o cálculo dos ângulos com o passar do tempo
    anguloSegundos = anguloSegundos + 6 * dt;
    anguloMinutos = anguloMinutos + 0.1f * dt;
    anguloHoras = anguloHoras + (1.0f / 120.0f) * dt;

    // realiza as transformações
    MSegundos = glm::rotate(MSegundos, glm::radians(anguloSegundos), glm::vec3(0, 0, -1));
    MSegundos = glm::scale(MSegundos, glm::vec3(0.02f, 1.4f, 1.0f));

    MMinutos = glm::rotate(MMinutos, glm::radians(anguloMinutos), glm::vec3(0, 0, -1));
    MMinutos = glm::scale(MMinutos, glm::vec3(0.03f, 1.2f, 1.0f));

    MHoras = glm::rotate(MHoras, glm::radians(anguloHoras), glm::vec3(0, 0, -1));
    MHoras = glm::scale(MHoras, glm::vec3(0.035f, 1.6f, 1.0f));

    MCirculoMeio = glm::scale(MCirculoMeio, glm::vec3(0.03f, 0.03f, 1.0f));

    // seta o shader e desenha os ponteiros
    shd->SetUniform("M", MSegundos);
    shd->SetUniform("icolor", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    ponteiros->Draw();

    shd->SetUniform("M", MMinutos);
    shd->SetUniform("icolor", glm::vec4(0.0f, 0.0f, 0.0f, 0.5f));
    ponteiros->Draw();

    shd->SetUniform("M", MHoras);
    shd->SetUniform("icolor", glm::vec4(0.0f, 0.0f, 0.0f, 0.75f));
    ponteiros->Draw();

    shd->SetUniform("M", MCirculoMeio);
    shd->SetUniform("icolor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    circulo->Draw();
}

void desenhaRelogio()
{
    // desenha o contorno do relógio
    glm::mat4 MRelogioContorno(1.0f);
    MRelogioContorno = glm::scale(MRelogioContorno, glm::vec3(0.915f, 0.915f, 1.0f));
    shd->SetUniform("M", MRelogioContorno);
    shd->SetUniform("icolor", glm::vec4(0, 0, 0, 1.0f));
    circulo->Draw();

    // desenha o círculo do relógio
    glm::mat4 MRelogio(1.0f);
    MRelogio = glm::scale(MRelogio, glm::vec3(0.9f, 0.9f, 1.0f));
    shd->SetUniform("M", MRelogio);
    shd->SetUniform("icolor", glm::vec4(0.95f, 0.95f, 0.95f, 1.0f));
    circulo->Draw();

    // desenha as linhas das horas
    shd->SetUniform("icolor", glm::vec4(0, 0, 0, 1.0f));
    for (int i = 0; i < 12; i++)
    {
        glm::mat4 MLinhasHoras(1.0f);
        MLinhasHoras = glm::rotate(MLinhasHoras, glm::radians(30.0f * i), glm::vec3(0, 0, -1));
        MLinhasHoras = glm::translate(MLinhasHoras, glm::vec3(0, 0.67f, 0));
        MLinhasHoras = glm::scale(MLinhasHoras, glm::vec3(0.06f, 0.4f, 1.0f));
        shd->SetUniform("M", MLinhasHoras);
        linhas->Draw();
    }


    // desenha as linhas entre as horas
    shd->SetUniform("icolor", glm::vec4(0.65f, 0.65f, 0.65f, 1.0f));
    for (int i = 0; i < 60; i++)
    {
        glm::mat4 MLinhasSubHoras(1.0f);
        float angulo = 6.0f * i;

        if (static_cast<int>(std::round(angulo)) % 30 != 0)
        {
            MLinhasSubHoras = glm::rotate(MLinhasSubHoras, glm::radians(angulo), glm::vec3(0, 0, -1));
            MLinhasSubHoras = glm::translate(MLinhasSubHoras, glm::vec3(0, 0.69f, 0));
            MLinhasSubHoras = glm::scale(MLinhasSubHoras, glm::vec3(0.02f, 0.3f, 1.0f));
            shd->SetUniform("M", MLinhasSubHoras);
            linhas->Draw();
        }
        
    }
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(1);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

unsigned int textureID;

static void initialize ()
{
  glClearColor(1.0f,1.0f,1.0f,1.0f);

  ponteiros = Poligono::Make();
  circulo = Disk::Make(1000);
  linhas = Poligono::Make();

  shd = Shader::Make();
  shd->AttachVertexShader("shaders/vertex.glsl");
  shd->AttachFragmentShader("shaders/fragment.glsl");
  shd->Link();

  Error::Check("initialize");
}

static void display (GLFWwindow* win)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shd->UseProgram();
  desenhaRelogio();
  Error::Check("display");
}

void update(float dt)
{
    desenhaPonteiros(dt);
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(500,500,"Tarefa 2 - INF1761",nullptr,nullptr);
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  
  glfwMakeContextCurrent(win);
 
#ifdef __glad_h_
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD OpenGL context\n");
    exit(1);
   }
#endif
#ifdef __glew_h__
  glewInit(); 
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW OpenGL context\n");
    exit(1);
  }
#endif

  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  initialize();

  // calcular posição inicial dos ponteiros do relógio
  auto agora = std::chrono::system_clock::now();
  std::time_t tempo_atual = std::chrono::system_clock::to_time_t(agora);
  std::tm* tempo = std::localtime(&tempo_atual);

  int horas = tempo->tm_hour;
  int minutos = tempo->tm_min;
  int segundos = tempo->tm_sec;

  if (horas >= 12) {
      horas -= 12;
  }

  anguloSegundos = segundos * 6;
  anguloMinutos = (minutos * 6) + (segundos * 0.1);
  anguloHoras = (horas * 30) + (minutos * 0.5) + (segundos * (0.5 / 60.0));

  double t0 = glfwGetTime();
  double t = 0;

  while(!glfwWindowShouldClose(win)) {
    //idle(win);
    t = glfwGetTime();
    display(win);
    update(t - t0);
    t0 = t;
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}
