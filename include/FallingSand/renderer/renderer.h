#ifndef FALLINGSAND_RENDERER_H
#define FALLINGSAND_RENDERER_H

#include <memory>

#include "shader.h"
#include "FallingSand/simulation/simulation.h"

// Represents a color in RGBA format
struct Color {
  uint8_t r, g, b, a;
};

class Renderer {
public:
  Renderer();
  void init(const Simulation &sim);
  void render(const Simulation &sim);
  void cleanup();

private:
  unsigned int VAO, VBO, EBO;
  std::unique_ptr<Shader> graphics_shader;
  unsigned int texture_id_;
};

#endif //FALLINGSAND_RENDERER_H