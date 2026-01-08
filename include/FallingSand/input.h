#ifndef FALLINGSAND_INPUT_H
#define FALLINGSAND_INPUT_H
#include "renderer/renderer.h"

#include <GLFW/glfw3.h>

#include "simulation/simulation.h"


class Input {
public:
  Input();
  void init(GLFWwindow *glfw_window);
  // Sets the GLFW window for input processing
  void update(Simulation &sim, CellType brush_type, int brush_size);
  // Processes inputs
  void apply_brush(GLFWwindow *window, Simulation &sim, int brush_size,
                   CellType type); // Draws the selected brush on the sim

private:
  GLFWwindow *window_;
  const int stone_brush_size = 3;
};


#endif //FALLINGSAND_INPUT_H