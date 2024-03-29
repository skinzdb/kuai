#include "kpch.h"
#include "FileUtil.h"

std::string FileUtil::load(const std::string& filename)
{
	std::ifstream file(filename);
	std::stringstream ss;

	if (file.is_open())
		ss << file.rdbuf();
	else
		KU_CORE_ERROR("Could not open file: {0}", filename);

	file.close();

	return ss.str();
}
