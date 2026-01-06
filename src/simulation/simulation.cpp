
#include "FallingSand/simulation/simulation.h"
#include <glad/glad.h>
#include <filesystem>
#include <random>
#include "FallingSand/renderer/shader.h"

Simulation::Simulation(int sim_width, int sim_height)
  : grid(sim_width, sim_height), next_grid(sim_width, sim_height),
    current_ssbo_idx(0),
    gen(rd()), dis(0, 1) {
}

void Simulation::init() {
  // Build and compile shader program
  std::filesystem::path projectRoot = std::filesystem::current_path();
  while (!std::filesystem::exists(projectRoot / "shaders") && projectRoot.
         has_parent_path()) {
    projectRoot = projectRoot.parent_path();
  }
  std::filesystem::path shaderPath = projectRoot / "shaders";
  std::string compute_path = (shaderPath / "shader.comp").string();
  compute_shader = std::make_unique<Shader>(compute_path.c_str());

  // Create 2 SSBOs
  glGenBuffers(2, SSBO);
  for (int i = 0; i < 2; i++) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
    glBufferData(
        GL_SHADER_STORAGE_BUFFER,
        get_grid_width() * get_grid_height() * sizeof(Cell), nullptr,
        GL_DYNAMIC_DRAW); // why dynamic copy?
  }

  // Initialize SSBOs with simulation data
  // const std::vector<Cell> &cells = sim.get_cells();
  // glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[0]);
  // glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0,
  //                 cells.size() * sizeof(Cell), cells.data());
}

void Simulation::update() {
  // Run compute shader to update simulation on the GPU
  compute_shader->use();

  // Set uniforms
  compute_shader->set_int("width", get_grid_width());
  compute_shader->set_int("height", get_grid_height());
  compute_shader->set_int("debugWorkgroupBorders", 1);

  // Bind input and output buffers
  unsigned int input_buffer_idx = current_ssbo_idx;
  unsigned int output_buffer_idx = 1 - current_ssbo_idx;
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO[input_buffer_idx]);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO[output_buffer_idx]);

  // Dispatch compute shader
  int num_groups_x = (get_grid_width());
  int num_groups_y = (get_grid_height());

  // Pass 1: Process even workgroups (checkerboard pattern)
  compute_shader->set_int("pass", 0);
  glDispatchCompute(num_groups_x, num_groups_y, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // Pass 2: Process odd workgroups (checkerboard pattern)
  // compute_shader->set_int("pass", 1);
  // glDispatchCompute(num_groups_x, num_groups_y, 1);
  // glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // Swap buffers
  current_ssbo_idx = output_buffer_idx;

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); // Unbind ssbo
}

void Simulation::cleanup() {
  glDeleteBuffers(2, SSBO);
}

void Simulation::clear() {
  for (int x = 0; x < grid.width; x++) {
    for (int y = 0; y < grid.height; y++) {
      grid.set_cell(x, y, CellType::Empty);
    }
  }
}

void Simulation::set_cell(int x, int y, CellType type) {
  CellType new_cell = {type};
  size_t offset = (y * 400 + x) * sizeof(Cell);

  // Update BOTH buffers so the change persists across the swap
  for (int i = 0; i < 2; ++i) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(Cell), &new_cell);
  }
  // Unbind buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

const uint32_t Simulation::get_active_cell_count() const {
  uint32_t active_cell_count = 0;
  for (int x = 0; x < grid.width; x++) {
    for (int y = 0; y < grid.height; y++) {
      if (getCell(x, y).type != CellType::Empty)
        active_cell_count++;
    }
  }
  return active_cell_count;
}

