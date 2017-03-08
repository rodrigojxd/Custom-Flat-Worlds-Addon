#include <jni.h>
#include <string>
#include "substrate.h"

const std::string defaultLayer = "[7,3,3,2]";

class FlatLevelSource
{
public:
	static std::string DEFAULT_LAYERS;
};

enum class GeneratorType : int
{
	OLD,
	INFINITE,
	FLAT,
	NETHER
};

class WorldSettingsScreenController
{
public:
	std::string _getWorldName(void);
	GeneratorType _getWorldType(void);
	void _setWorldName(std::string);
};

void(*createLevel)(WorldSettingsScreenController*);
void createLevel_(WorldSettingsScreenController* wssc)
{
	if (wssc->_getWorldType() == GeneratorType::FLAT)
	{
		std::string name = wssc->_getWorldName();
		size_t pos = name.find_last_of('[');
		if (pos != std::string::npos)
		{
			FlatLevelSource::DEFAULT_LAYERS = name.substr(pos);
			wssc->_setWorldName(name.substr(0U, pos));
		}
		else
		{
			FlatLevelSource::DEFAULT_LAYERS = defaultLayer;
		}
	}
	createLevel(wssc);
}

void(*setWorldType)(WorldSettingsScreenController*, GeneratorType);
void setWorldType_(WorldSettingsScreenController* wssc, GeneratorType worldType)
{
	if (worldType == GeneratorType::FLAT)
	{
		std::string name = wssc->_getWorldName();
		size_t pos = name.find_last_of('[');
		if (pos == std::string::npos)
		{
			wssc->_setWorldName(name + " " + defaultLayer);
		}
	}
	setWorldType(wssc, worldType);
}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	Hook("_ZN29WorldSettingsScreenController12_createWorldEv", (void*)&createLevel_, (void**)&createLevel);
	Hook("_ZN29WorldSettingsScreenController13_setWorldTypeEi", (void*)&setWorldType_, (void**)&setWorldType);
	return JNI_VERSION_1_6;
}