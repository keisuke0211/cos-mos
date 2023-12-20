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
	Pos3D  basePos;
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
#define FISH_NUM (4)
static Fish fishes[FISH_NUM];
static const FishData fishDatas[FISH_NUM] = {
	// ���p�X�������Ȃ�"NONEDATA"�ƋL������
	// �Z�b�g�A�b�v�p�X �� ���̔{�� / ���[�V�����p�X / ���[��3D�p�X / 1���ɂ����鎞�� / �J�E���g�J�n����
	{ "data\\SETUP\\Whale.txt", 3.0f, Pos3D(-100.0f,300.0f,200.0f), "data\\MOTION\\Whale.txt", "data\\RAIL3D\\Whale.txt", 1800, 0 },
	{ "data\\SETUP\\Iruka.txt", 1.5f, Pos3D(0.0f,-150.0f,500.0f), "data\\MOTION\\Iruka.txt", "data\\RAIL3D\\Iruka.txt", 2040, 0 },
	{ "data\\SETUP\\Small_Fish.txt", 1.0f, Pos3D(0.0f,0.0f,30.0f), "NONEDATA", "data\\RAIL3D\\Small_Fish.txt", 720, 0 },
	{ "data\\SETUP\\Small_Fish.txt", 1.0f, Pos3D(0.0f,50.0f,30.0f), "NONEDATA", "data\\RAIL3D\\Small_Fish.txt", 720, 60 },
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
		fishes[cnt].counter = (fishes[cnt].counter + 1) % fishDatas[cnt].time;

		Matrix mtx = fishes[cnt].rail->GetMtx(fishes[cnt].counter / (float)fishDatas[cnt].time, true);
		fishes[cnt].doll->SetPos(RNLib::Matrix().ConvMtxToPos(mtx) + fishDatas[cnt].basePos);
		fishes[cnt].doll->SetRot(RNLib::Matrix().ConvMtxToRot(mtx));
	}
}