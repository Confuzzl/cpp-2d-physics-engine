#pragma once
#include "gl.h"

import rendering;
import debug;
import mesh;
import object;
import scene;
import aabb;

static void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id,
                                   GLenum severity, GLsizei length,
                                   const GLchar *message,
                                   const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;
  println("{} {}", severity, message);
}

import glm;

int main() {
  // println("{}", glm::to_string(glm::translate(
  //                   glm::ortho(-10.0, 10.0, -10.0, 10.0), {3, 3, 3})));
  // println("{}", glm::to_string(glm::translate(glm::ortho(-1.0, 1.0,
  // -1.0, 1.0),
  //                                             {3, 3, 3})));

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
    Object obj = Object::ngon({.sides = 3}, {.pos = {0, 0}});
    // println("{} {}", glm::to_string(obj.collider.aabb.min),
    //         glm::to_string(obj.collider.aabb.max));
    const Object &objt = obj;
    const Collider &coll = *objt.collider;
    const AABB &aabb = *coll.aabb;

    println("objt {:x}->{:x}->{:x}", reinterpret_cast<size_t>(&obj),
            reinterpret_cast<size_t>(&(*obj.collider)),
            reinterpret_cast<size_t>(&(*obj.collider->aabb)));
    println("pos {}->{}->{}", glm::to_string(obj.pos),
            glm::to_string(obj.collider->globalPos()),
            glm::to_string(obj.collider->aabb->parent.globalPos()));

    println("coll {:x}<-{:x}->{:x}", reinterpret_cast<size_t>(&(coll.parent)),
            reinterpret_cast<size_t>(&(coll)),
            reinterpret_cast<size_t>(&(*coll.aabb)));
    println("pos {}->{}->{}", glm::to_string(coll.parent.pos),
            glm::to_string(coll.globalPos()),
            glm::to_string(coll.aabb->parent.globalPos()));

    println("aabb {:x}<-{:x}<-{:x}",
            reinterpret_cast<size_t>(&(aabb.parent.parent)),
            reinterpret_cast<size_t>(&(aabb.parent)),
            reinterpret_cast<size_t>(&(aabb)));
    println("pos {}->{}->{}", glm::to_string(aabb.parent.parent.pos),
            glm::to_string(aabb.parent.globalPos()),
            glm::to_string(aabb.parent.globalPos()));

    while (!glfwWindowShouldClose(window)) {
      glClearColor(0.5, 0.5, 0.5, 1);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // renderer.render(obj);
      renderer.render(*obj.collider->aabb, obj.getColor());

      glfwSwapBuffers(window);
      glfwPollEvents();

      glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
  } catch (const std::runtime_error &err) {
    println(err.what());
  }
}
