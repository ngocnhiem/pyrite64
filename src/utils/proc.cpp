/**
* @copyright 2025 - Max Bebök
* @license MIT
*/
#include "proc.h"

#include <fstream>
#include <memory>

namespace
{
  constexpr uint32_t BUFF_SIZE = 128;
}

std::string Utils::Proc::runSync(const std::string &cmd)
{
  std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
  if(!pipe)return "";

  char buffer[BUFF_SIZE];
  std::string result{};

  while(!feof(pipe.get()))
  {
    if(fgets(buffer, BUFF_SIZE, pipe.get()) != nullptr) {
      result += buffer;
    }
  }
  return result;
}
