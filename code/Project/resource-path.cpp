
#include "resource.h"

short CResources::TEXTURE_IDXES[(int)TEXTURE::MAX] = {};
const char* CResources::TEXTURE_PATHS[(int)TEXTURE::MAX] = {
	"data\\TEXTURE\\BackGround\\Wilderness.png",
	"data\\TEXTURE\\BackGround\\Forest.png",
	"data\\TEXTURE\\BackGround\\Cave.png",
};

short CResources::SOUND_IDXES[(int)SOUND::MAX] = {};
const char* CResources::SOUND_PATHS[(int)SOUND::MAX] = {
	"data\\SOUND\\SE\\ok.wav",
	"data\\SOUND\\SE\\scratch.wav",
};