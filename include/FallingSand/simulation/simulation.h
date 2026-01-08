
#ifndef FALLINGSAND_SIMULATION_H
#define FALLINGSAND_SIMULATION_H
#include "grid.h"

#include <memory>
#include <random>

class Shader;

class Simulation {
public:
  Simulation(int sim_width, int sim_height);
  void init(); // Compiles the compute shader program and initializes 2 SSBOs

  // Updates the simulation 1 time. Runs the compute shader twice.
  void update();

  void cleanup(); // Deletes buffers

  unsigned int get_current_ssbo() const { return SSBO[current_ssbo_idx]; }

  int get_grid_width() const { return width; }
  int get_grid_height() const { return height; }

  void set_cell(int x, int y, CellType type);

  void clear(); // Clears the simulation - All cells are set to empty.

private:
  int width, height;

  // Compute shader
  std::unique_ptr<Shader> compute_shader;
  unsigned int SSBO[2]; // 2 ssbos for double buffering
  unsigned int current_ssbo_idx;

  // random - for choosing a direction ( NOT USED IN GPU PORT )
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> dis;
};

#endif // FALLINGSAND_SIMULATION_H