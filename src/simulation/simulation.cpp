
#include "FallingSand/simulation/simulation.h"
#include <glad/glad.h>
#include <filesystem>
#include <random>
#include "FallingSand/renderer/shader.h"

Simulation::Simulation(int sim_width, int sim_height)
  : width(sim_width), height(sim_height),
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
  clear(); // Initializes the ssbos with empty cells
}

void Simulation::update() {
  // Run compute shader to update simulation on the GPU
  compute_shader->use();

  // Set uniforms
  compute_shader->set_int("width", get_grid_width());
  compute_shader->set_int("height", get_grid_height());

  // Bind input and output buffers
  unsigned int input_buffer_idx = current_ssbo_idx;
  unsigned int output_buffer_idx = 1 - current_ssbo_idx;
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO[input_buffer_idx]);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, SSBO[output_buffer_idx]);

  // Workgroups are set per cell
  int num_groups_x = (get_grid_width());
  int num_groups_y = (get_grid_height());

  // Pass 1: Process even workgroups
  compute_shader->set_int("pass", 0);
  glDispatchCompute(num_groups_x, num_groups_y, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // Pass 2: Process odd workgroups
  compute_shader->set_int("pass", 1);
  glDispatchCompute(num_groups_x, num_groups_y, 1);
  glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

  // Swap buffers
  current_ssbo_idx = output_buffer_idx;

  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER); // Unbind ssbo
}

void Simulation::cleanup() {
  glDeleteBuffers(2, SSBO);
}

void Simulation::clear() {
  for (int i = 0; i < 2; i++) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, width * height * sizeof(Cell),
                 nullptr,
                 GL_DYNAMIC_DRAW);
  }
}

void Simulation::set_cell(int x, int y, CellType type) {
  CellType new_cell = {type};
  size_t offset = (y * width + x) * sizeof(Cell);

  // Update BOTH buffers so the change persists across the swap
  for (int i = 0; i < 2; ++i) {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO[i]);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, sizeof(Cell), &new_cell);
  }
  // Unbind buffer
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}


