#pragma once
#include "gl.h"

import rendering;
import debug;
import mesh;
import object;
import scene;
import aabb;
import app;

int main() {
  MAIN_APP.start();

  // glfwInit();
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // GLFWwindow *window = glfwCreateWindow(Renderer::WIDTH, Renderer::HEIGHT,
  //                                       "2D Physics Engine", NULL, NULL);
  // if (window == NULL) {
  //   glfwTerminate();
  //   return 1;
  // }
  // glfwMakeContextCurrent(window);

  // gladLoadGL();
  // glViewport(0, 0, Renderer::WIDTH, Renderer::HEIGHT);

  // glPointSize(10);
  // glLineWidth(5);
  // glPolygonMode(GL_FRONT, GL_FILL);
  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_DEPTH_TEST);
  // glEnable(GL_CULL_FACE);

  // glEnable(GL_DEBUG_OUTPUT);
  // glDebugMessageCallback(debugCallback, 0);

  // try {
  //   const Renderer renderer{};
  //   Object obj = Object::ngon({.sides = 5}, {.pos = {0, 0}});
  //   Object obj2 = Object::circle({}, {});
  //   AABB &aabb = *obj.collider->aabb;

  //  GLuint vboID = aabb.vbo.ID;
  //  vboID = obj.mesh->vbo.ID;

  //  while (!glfwWindowShouldClose(window)) {
  //    glClearColor(0.5, 0.5, 0.5, 1);
  //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //    renderer.render(obj, {.showAABB = true});

  //    glfwSwapBuffers(window);
  //    glfwPollEvents();

  //    // glfwSetWindowShouldClose(window, GLFW_TRUE);
  //  }
  //} catch (const std::runtime_error &err) {
  //  println(err.what());
  //}
}
