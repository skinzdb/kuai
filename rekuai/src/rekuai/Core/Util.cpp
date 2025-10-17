#include "kpch.h"

#include "Util.h"

std::string read_file(const std::string& filename)
{
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    KU_CORE_ERROR("Failed to open file {}", filename);
    return "";
  }

  size_t file_size = (size_t)file.tellg();
  std::vector<char> buffer(file_size);

  file.seekg(0);
  file.read(buffer.data(), file_size);

  file.close();

  return std::string(buffer.begin(), buffer.end());
}
