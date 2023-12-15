
#include "resource.h"

short CResources::TEXTURE_IDXES[(int)TEXTURE::MAX] = {};
const char* CResources::TEXTURE_PATHS[(int)TEXTURE::MAX] = {
	"data\\TEXTURE\\BackGround\\Wilderness.png",
	"data\\TEXTURE\\BackGround\\Forest.png",
	"data\\TEXTURE\\BackGround\\Cave.png",
	"data\\TEXTURE\\BackGround\\Ocean.png",
	"data\\TEXTURE\\BackGround\\City.png",
	"data\\TEXTURE\\BackGround\\Cloud.png",
	"data\\TEXTURE\\WaitBalloon.PNG",
	"data\\TEXTURE\\BlackEye.PNG",
};

short CResources::SOUND_IDXES[(int)SOUND::MAX] = {};
const char* CResources::SOUND_PATHS[(int)SOUND::MAX] = {
	"data\\SOUND\\SE\\ok.wav",
	"data\\SOUND\\SE\\scratch_00.wav",
	"data\\SOUND\\SE\\jamp_01.wav",
	"data\\SOUND\\SE\\landing\\jamp.wav",
	"data\\SOUND\\SE\\swap_now.wav",
	"data\\SOUND\\SE\\swap_end.wav",
	"data\\SOUND\\SE\\death_expand.wav",
	"data\\SOUND\\SE\\death_explosion.wav",
	"data\\SOUND\\SE\\heron2.wav",
	"data\\SOUND\\SE\\shrink.wav",
	"data\\SOUND\\SE\\extend.wav",
	"data\\SOUND\\SE\\vibration.wav",
	"data\\SOUND\\SE\\rocket\\parts_get.wav",
	"NONEDATA",
};