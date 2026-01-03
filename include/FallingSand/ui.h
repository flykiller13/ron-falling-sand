#ifndef FALLINGSAND_UI_H
#define FALLINGSAND_UI_H
#include <GLFW/glfw3.h>

#include "simulation/grid.h"

enum class CellType;
class Simulation;

class UI {
public:
  UI();
  void init(GLFWwindow *window);
  void update(Simulation &sim);
  void terminate();

  bool show_window = true;
  CellType brush_type = CellType::Sand;
  int brush_size = 1;
  const char *brush_types[5] = {"Empty", "Sand", "Stone", "Water", "Gas"};
};


#endif //FALLINGSAND_UI_H