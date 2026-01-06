
#include "FallingSand/input.h"
#include "FallingSand/simulation/grid.h" // For CellType
#include "imgui.h"

void Input::apply_brush(GLFWwindow *window, Simulation &sim, int brush_size,
                        CellType type) {
  // Check if the mouse is over an ImGui window
  // If it is, we don't want to draw pixels behind the menu
  if (ImGui::GetIO().WantCaptureMouse)
    return;

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  int x_ratio = width / 400;
  int y_ratio = height / 400;
  int grid_x = static_cast<int>(xpos) / x_ratio;
  int grid_y = static_cast<int>(height - ypos) / y_ratio;

  sim.set_cell(grid_x, grid_y, type);
}

Input::Input() : window_(nullptr) {
}

void Input::init(GLFWwindow *glfw_window) {
  window_ = glfw_window;
}

void Input::update(Simulation &sim, CellType brushType, int brushSize) {
  int width, height;
  glfwGetWindowSize(window_, &width, &height);

  // ESC - Close window
  if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window_, true);

  // LMB - Draw selected material
  if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    apply_brush(window_, sim, brushSize, brushType);
  // RMB - Draw Stone
  if (glfwGetMouseButton(window_, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    apply_brush(window_, sim, brushSize, CellType::Stone);
}