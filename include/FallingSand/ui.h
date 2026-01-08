#ifndef FALLINGSAND_UI_H
#define FALLINGSAND_UI_H
#include <GLFW/glfw3.h>

#include "simulation/grid.h"

enum class CellType;
class Simulation;

class UI {
public:
  UI();
  void init(GLFWwindow *window); // Sets up ImGui
  void update(Simulation &sim); // Draws the UI
  void terminate(); // Terminates ImHui

  bool show_window = true; // Used to close the UI window
  CellType brush_type = CellType::Sand;
  int brush_size = 1;
  const char *brush_types[5] = {"Empty", "Stone", "Sand", "Water", "Gas"};
};


#endif //FALLINGSAND_UI_H