//========================================
// 
// �f�t�H���g�f�[�^�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f�t�H���g�f�[�^�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �萔��`
//****************************************
const char* CDefaultData::SOUND_PATHS[(int)SOUND::MAX] = {
	"RNData\\Sound\\TestBGM.wav",
	"RNData\\Sound\\TestSE.wav",
};
const char* CDefaultData::TEXTURE_PATHS[(int)TEXTURE::MAX] = {
	"RNData\\Texture\\Tile.PNG",
	"RNData\\Texture\\Grid.PNG",
	"RNData\\Texture\\Stripe.PNG",
	"RNData\\Texture\\Border.PNG",
	"RNData\\Texture\\Smoke.PNG",
};
const char* CDefaultData::MODEL_PATHS[(int)MODEL::MAX] = {
	"RNData\\Model\\Cube.x",
	"RNData\\Model\\Sphere.x",
	"RNData\\Model\\Dome_Up.x",
	"RNData\\Model\\Dome_Down.x",
	"RNData\\Model\\Observer.x",
	"RNData\\Model\\Monitor.x",
	"RNData\\Model\\Prufen\\Head.x",
	"RNData\\Model\\Prufen\\Body.x",
	"RNData\\Model\\Prufen\\Limb_Up.x",
	"RNData\\Model\\Prufen\\Limb_Down.x",
	"RNData\\Model\\Prufen\\Limb_Left.x",
	"RNData\\Model\\Prufen\\Limb_Right.x",
};
const char* CDefaultData::SETUP3D_PATHS[(int)SETUP3D::MAX] = {
	"RNData\\SetUp3D\\Capsule.txt",
};
const char* CDefaultData::MOTION3D_PATHS[(int)MOTION3D::MAX] = {
	"RNData\\Motion3D\\CapsuleStretch.txt",
};

//========================================
// �R���X�g���N�^
//========================================
CDefaultData::CDefaultData(void) {

	for (int cntSound    = 0; cntSound    < (int)SOUND   ::MAX; m_soundIdxes   [cntSound   ] = NONEDATA, cntSound++  );
	for (int cntTexture  = 0; cntTexture  < (int)TEXTURE ::MAX; m_textureIdxes [cntTexture ] = NONEDATA, cntTexture++);
	for (int cntModel    = 0; cntModel    < (int)MODEL   ::MAX; m_modelIdxes   [cntModel   ] = NONEDATA, cntModel++  );
	for (int cntSetUp3D  = 0; cntSetUp3D  < (int)SETUP3D ::MAX; m_setUp3DIdxes [cntSetUp3D ] = NONEDATA, cntSetUp3D++);
	for (int cntMotion3D = 0; cntMotion3D < (int)MOTION3D::MAX; m_motion3DIdxes[cntMotion3D] = NONEDATA, cntMotion3D++);
}

//========================================
// �f�X�g���N�^
//========================================
CDefaultData::~CDefaultData(void) {

}

//========================================
// ����������
//========================================
void CDefaultData::Init(void) {

	// �T�E���h�ǂݍ���
	for (int cntSound = 0; cntSound < (int)SOUND::MAX; cntSound++)
		m_soundIdxes[cntSound] = RNLib::Sound().Load(SOUND_PATHS[cntSound]);

	// �e�N�X�`���ǂݍ���
	for (int cntTexture = 0; cntTexture < (int)TEXTURE::MAX; cntTexture++)
		m_textureIdxes[cntTexture] = RNLib::Texture().Load(TEXTURE_PATHS[cntTexture]);

	// ���f���ǂݍ���
	for (int cntModel = 0; cntModel < (int)MODEL::MAX; cntModel++)
		m_modelIdxes[cntModel] = RNLib::Model().Load(MODEL_PATHS[cntModel]);

	// �Z�b�g�A�b�v3D�ǂݍ���
	for (int cntSetUp3D = 0; cntSetUp3D < (int)SETUP3D::MAX; cntSetUp3D++)
		m_setUp3DIdxes[cntSetUp3D] = RNLib::SetUp3D().Load(SETUP3D_PATHS[cntSetUp3D]);

	// ���[�V����3D�ǂݍ���
	for (int cntMotion3D = 0; cntMotion3D < (int)MOTION3D::MAX; cntMotion3D++)
		m_motion3DIdxes[cntMotion3D] = RNLib::Motion3D().Load(MOTION3D_PATHS[cntMotion3D]);
}

//========================================
// �I������
//========================================
void CDefaultData::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CDefaultData::Update(void) {

}