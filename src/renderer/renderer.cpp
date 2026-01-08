// GLAD download:
// https://glad.dav1d.de/generated/tmpk7onoi3rglad/

#include "FallingSand/renderer/renderer.h"

#include <cmath>
#include <filesystem>
#include <glad/glad.h> // must be included before GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include "FallingSand/renderer/shader.h"

#include <cstring>

Renderer::Renderer() : VAO(0), VBO(0), EBO(0), texture_id_(0) {
}

void Renderer::init(const Simulation &sim) {
  // Build and compile shader program
  std::filesystem::path projectRoot = std::filesystem::current_path();
  while (!std::filesystem::exists(projectRoot / "shaders") && projectRoot.
         has_parent_path()) {
    projectRoot = projectRoot.parent_path();
  }
  std::filesystem::path shaderPath = projectRoot / "shaders";
  std::string vertexPath = (shaderPath / "shader.vert").string();
  std::string fragmentPath = (shaderPath / "shader.frag").string();
  graphics_shader = std::make_unique<Shader>(vertexPath.c_str(),
                                             fragmentPath.c_str());

  float vertices[] = {
      // positions          // texture coords
      1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top right
      1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom right
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom left
      -1.0f, 1.0f, 0.0f, 0.0f, 1.0f // top left
  };
  unsigned int indices[] = {
      0, 1, 3, // first triangle
      1, 2, 3 // second triangle
  };

  // Create VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO); // Bind vertex array object

  // Create VBO
  glGenBuffers(1, &VBO);
  // Copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Create EBO
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  // tex coords attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // Initialize texture
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Generate mipmaps
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Renderer::render(const Simulation &sim) {
  // clear color buffer
  glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  graphics_shader->use();

  // Bind the current SSBO for the graphics shader to read
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, sim.get_current_ssbo());

  // Set uniforms for grid dimensions
  graphics_shader->set_int("width", sim.get_grid_width());
  graphics_shader->set_int("height", sim.get_grid_height());

  // Draw triangles
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

  // Unbind VAO
  glBindVertexArray(0);
}

void Renderer::cleanup() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteTextures(1, &texture_id_);
}
