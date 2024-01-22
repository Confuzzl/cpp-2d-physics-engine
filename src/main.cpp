#include "gl.h"

import rendering;
import debug;

static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar *message,
                                   const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  println("{} {}", severity, message);
}

int main() {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(Renderer::WIDTH, Renderer::HEIGHT,
                                        "2D Physics Engine", NULL, NULL);
  if (window == NULL) {
    glfwTerminate();
    return 1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, Renderer::WIDTH, Renderer::HEIGHT);

  glPointSize(10);
  glLineWidth(5);
  glPolygonMode(GL_FRONT, GL_FILL);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(debugCallback, 0);

  const Renderer renderer{};

  while (!glfwWindowShouldClose(window)) {
    glClearColor(0.5, 0.5, 0.5, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    renderer.text("FOO", 100, 100);

    glfwSwapBuffers(window);
    glfwPollEvents();

    // glfwSetWindowShouldClose(window, GLFW_TRUE);
  }
}
