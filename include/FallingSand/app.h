#ifndef FALLINGSAND_APP_H
#define FALLINGSAND_APP_H

#include <glad/glad.h>  // must be included before GLFW
#include <GLFW/glfw3.h>
#include "renderer/renderer.h"
#include "input.h"
#include "ui.h"
#include "simulation/simulation.h"

#define ENGINE_TITLE "Falling Sand"

class App {
public:
  App();
  bool init();
  void run();
  void cleanup();

private:
  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height);

  Input input;
  Simulation sim;
  Renderer renderer;
  UI ui;

  GLFWwindow *window_;
  int window_width = 800;
  int window_height = 800;
  double last_frame = 0.0;
  double current_frame, delta_time;
};


#endif //FALLINGSAND_APP_H