#include "smpch.h"
#include "FileUtil.h"

std::string FileUtil::Load(const char* filename)
{
	std::ifstream file(filename);
	std::stringstream ss;

	if (file.is_open())
		ss << file.rdbuf();
	else
		SM_CORE_ERROR("Could not open file: {0}", filename);

	file.close();

	return ss.str();
}
