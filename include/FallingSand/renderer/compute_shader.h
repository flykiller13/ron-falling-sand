
#ifndef FALLINGSAND_COMPUTESHADER_H
#define FALLINGSAND_COMPUTESHADER_H

#include <string>
#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <iostream>

class ComputeShader {
public:
  ComputeShader(const char *path);

private:
  void check_compile_errors(unsigned int shader, std::string type);

  unsigned int id_;
};


#endif //FALLINGSAND_COMPUTESHADER_H