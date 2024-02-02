#pragma once
#include "gl.h"

import rendering;
import debug;
import mesh;
import object;
import scene;
import aabb;

static std::string sourceName(GLenum source) {
  switch (source) {
  case GL_DEBUG_SOURCE_API:
    return "API";
  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    return "WINDOW SOURCE";
  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    return "SHADER COMPILER";
  case GL_DEBUG_SOURCE_THIRD_PARTY:
    return "THIRD PARTY";
  case GL_DEBUG_SOURCE_APPLICATION:
    return "APPLICATION";
  case GL_DEBUG_SOURCE_OTHER:
    return "OTHER";
  }
  return "";
}

static std::string typeName(GLenum type) {
  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    return "ERROR";
  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    return "DEPRECATED BEHAVIOR";
  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    return "UNDEFINED BEHAVIOR";
  case GL_DEBUG_TYPE_PORTABILITY:
    return "PORTABILITY";
  case GL_DEBUG_TYPE_PERFORMANCE:
    return "PERFORMANCE";
  case GL_DEBUG_TYPE_MARKER:
    return "MARKER";
  case GL_DEBUG_TYPE_PUSH_GROUP:
    return "PUSH GROUP";
  case GL_DEBUG_TYPE_POP_GROUP:
    return "POP GROUP";
  case GL_DEBUG_TYPE_OTHER:
    return "OTHER";
  }
  return "";
}

static std::string severityName(GLenum severity) {
  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    return "HIGH";
  case GL_DEBUG_SEVERITY_MEDIUM:
    return "MEDIUM";
  case GL_DEBUG_SEVERITY_LOW:
    return "LOW";
  case GL_DEBUG_SEVERITY_NOTIFICATION:
    return "NOTIFICATION";
  }
  return "";
}

static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar *message,
                                   const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  println("source={} type={} id={} severity={} | {}", sourceName(source),
          typeName(type), id, severityName(severity), message);
}

import glm;

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

  try {
    const Renderer renderer{};
    Object obj = Object::ngon({.sides = 5}, {.pos = {0, 0}});
    Object obj2 = Object::circle({}, {});
    AABB &aabb = *obj.collider->aabb;

    GLuint vboID = aabb.vbo.ID;
    vboID = obj.mesh->vbo.ID;

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.5, 0.5, 0.5, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      renderer.render(obj, {.showAABB = true});

      glfwSwapBuffers(window);
      glfwPollEvents();

      // glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  } catch (const std::runtime_error &err) {
    println(err.what());
  }
}
