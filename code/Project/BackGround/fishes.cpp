//========================================
// 
// ���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "fishes.h"
#include "../../_RNLib/RNlib.h"
#include "../manager.h"

//****************************************
// �\���̂̒�`
//****************************************
struct FishData {
	String setUpPath;
	float  scale;
	String motionPath;
	String railPath;
	int    time;
	int    startTime;
};
struct Fish {
	CDoll3D* doll;
	CRail3D* rail;
	int counter;
};

//****************************************
// ���̒�`
//****************************************
#define FISH_NUM (1)
static Fish fishes[FISH_NUM];
static const FishData fishDatas[FISH_NUM] = {
	// ���p�X�������Ȃ�"NONEDATA"�ƋL������
	// �Z�b�g�A�b�v�p�X �� ���̔{�� / ���[�V�����p�X / ���[��3D�p�X / 1���ɂ����鎞�� / �J�E���g�J�n����
	{ "data\\SETUP\\Whale.txt", 1.0f, "data\\MOTION\\Whale.txt", "data\\RAIL3D\\Whale.txt", 360, 0 },
};

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ���̊֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �J�n����
//========================================
void Fishes::Start(void) {

	for (int cnt = 0; cnt < FISH_NUM; cnt++) {
		fishes[cnt].doll = new CDoll3D(PRIORITY_BACKGROUND, RNLib::SetUp3D().Load(fishDatas[cnt].setUpPath));
		fishes[cnt].doll->SetMotion(RNLib::Motion3D().Load(fishDatas[cnt].motionPath));
		fishes[cnt].doll->SetScale(Scale3D(fishDatas[cnt].scale, fishDatas[cnt].scale, fishDatas[cnt].scale));
		fishes[cnt].rail = new CRail3D(fishDatas[cnt].railPath);
		fishes[cnt].counter = fishDatas[cnt].startTime;
	}
}

//========================================
// �I������
//========================================
void Fishes::End(void) {

	for (int cnt = 0; cnt < FISH_NUM; cnt++) {
		delete fishes[cnt].doll;
		delete fishes[cnt].rail;
	}
}

//========================================
// �X�V����
//========================================
void Fishes::Update(void) {

	for (int cnt = 0; cnt < FISH_NUM; cnt++) {
		if (++fishes[cnt].counter > fishDatas[cnt].time) {
			fishes[cnt].counter = fishDatas[cnt].time;
		}

		Matrix mtx = fishes[cnt].rail->GetMtx(fishes[cnt].counter / (float)fishDatas[cnt].time, true);
		fishes[cnt].doll->SetPos(RNLib::Matrix().ConvMtxToPos(mtx));
		fishes[cnt].doll->SetRot(RNLib::Matrix().ConvMtxToRot(mtx));
	}
}