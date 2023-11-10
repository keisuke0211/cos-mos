//========================================
// 
// �f�t�H���g�f�[�^�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f�t�H���g�f�[�^�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �萔��`
//****************************************
const char* CDefaultData::SOUND_PATH[(int)SOUND::MAX] = {
	"RNData\\Sound\\TestBGM.wav",
	"RNData\\Sound\\TestSE.wav",
};
const char* CDefaultData::MODEL_PATH[(int)MODEL::MAX] = {
	"RNData\\Model\\Sphere.x",
	"RNData\\Model\\Observer.x",
	"RNData\\Model\\Prufen\\Head.x",
	"RNData\\Model\\Prufen\\Body.x",
	"RNData\\Model\\Prufen\\Limb_Up.x",
	"RNData\\Model\\Prufen\\Limb_Down.x",
	"RNData\\Model\\Prufen\\Limb_Left.x",
	"RNData\\Model\\Prufen\\Limb_Right.x",
};

//========================================
// �R���X�g���N�^
//========================================
CDefaultData::CDefaultData(void) {

	for (int cntSound = 0; cntSound < (int)SOUND::MAX; m_soundIdxes[cntSound] = NONEDATA, cntSound++);
	for (int cntModel = 0; cntModel < (int)MODEL::MAX; m_modelIdxes[cntModel] = NONEDATA, cntModel++);
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
		m_soundIdxes[cntSound] = RNLib::Sound().Load(SOUND_PATH[cntSound]);

	// ���f���ǂݍ���
	for (int cntModel = 0; cntModel < (int)MODEL::MAX; cntModel++)
		m_modelIdxes[cntModel] = RNLib::Model().Load(MODEL_PATH[cntModel]);
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