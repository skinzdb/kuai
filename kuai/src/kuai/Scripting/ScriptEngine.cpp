#include "kpch.h"
//#include "ScriptEngine.h"
//
//#include "mono/jit/jit.h"
//#include "mono/metadata/assembly.h"
//
//namespace kuai {
//
//	struct ScriptEngineData
//	{
//		MonoDomain* rootDomain = nullptr;
//		MonoDomain* appDomain = nullptr;
//	};
//
//	static ScriptEngineData* data;
//
//	void ScriptEngine::init()
//	{
//		data = new ScriptEngineData();
//
//		mono_set_assemblies_path("mono/lib");
//
//		MonoDomain* rootDom = mono_jit_init("SmongineScriptEngine");
//		KU_CORE_ASSERT(rootDom, "Failed to initialise scripting engine");
//
//		data->rootDomain = rootDom;
//
//		data->appDomain = mono_domain_create_appdomain("SmongineScriptRuntime", nullptr);
//		mono_domain_set(data->appDomain, true);
//	}
//
//	void ScriptEngine::cleanup()
//	{
//		delete data;
//	}
//
//	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
//	{
//		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);
//
//		if (!stream)
//		{
//			// Failed to open the file
//			return nullptr;
//		}
//
//		std::streampos end = stream.tellg();
//		stream.seekg(0, std::ios::beg);
//		uint32_t size = end - stream.tellg();
//
//		if (size == 0)
//		{
//			// File is empty
//			return nullptr;
//		}
//
//		char* buffer = new char[size];
//		stream.read((char*)buffer, size);
//		stream.close();
//
//		*outSize = size;
//		return buffer;
//	}
//
//	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
//	{
//		uint32_t fileSize = 0;
//		char* fileData = ReadBytes(assemblyPath, &fileSize);
//
//		// NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
//		MonoImageOpenStatus status;
//		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);
//
//		if (status != MONO_IMAGE_OK)
//		{
//			const char* errorMessage = mono_image_strerror(status);
//			// Log some error message using the errorMessage data
//			return nullptr;
//		}
//
//		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
//		mono_image_close(image);
//
//		// Don't forget to free the file data
//		delete[] fileData;
//
//		return assembly;
//	}
//
//}