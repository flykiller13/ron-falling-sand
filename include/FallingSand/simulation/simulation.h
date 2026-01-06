
#ifndef FALLINGSAND_SIMULATION_H
#define FALLINGSAND_SIMULATION_H
#include "grid.h"

#include <memory>
#include <random>

class Shader;

class Simulation {
public:
  Simulation(int sim_width, int sim_height);
  void init();
  void update();
  void cleanup();
  void clear();

  unsigned int get_current_ssbo() const { return SSBO[current_ssbo_idx]; }

  int get_grid_width() const { return grid.width; }
  int get_grid_height() const { return grid.height; }

  const std::vector<Cell> &get_cells() const { return grid.cells; }

  void set_cell(int x, int y, CellType type);

  const Cell &getCell(int x, int y) const {
    return grid.cells[x + y * grid.width];
  }

  const uint32_t get_active_cell_count() const;

private:
  // We use a double buffer method - Data is read from grid and written to next_grid
  Grid grid;
  Grid next_grid; // Buffer grid

  // Compute shader
  std::unique_ptr<Shader> compute_shader;
  unsigned int SSBO[2]; // 2 ssbos for double buffering
  unsigned int current_ssbo_idx;

  // random - for choosing a direction
  std::random_device rd;
  std::mt19937 gen;
  std::uniform_int_distribution<> dis;
};

#endif // FALLINGSAND_SIMULATION_H