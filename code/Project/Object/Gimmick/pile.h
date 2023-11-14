#pragma once
//================================================================================================
// 
// �Y����[pile.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"

// �Y�N���X
class CPile : public CStageObject
{
public:
	//�Œኲ��
	static const int MIN_TRUNK = 3;

	CPile();
	virtual      ~CPile();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);

	//===============================
	//�ݒ菈��
	//�����ɕK�v�����i�Œ�3�j��z�u���W���w�肷��
	//-------------------------------
	//�����P  pos�F�z�u���W
	//�����Q  NumTrunk�F���̐��i�Œ�R�� MIN_TRUNK�j
	//�����R  TrunkHeight�F���̂߂荞�ݍ��W�i�߂荞�݂Ȃ� = 0.0f�j
	//===============================
	void Set(Pos3D pos, int NumTrunk, float TrunkHeight);

private:
	void SetModel(void);//���f���z�u

	int m_BodyModelID;	//�{�̂̃��f���ԍ�
	int m_TrunkModelID;	//�������̃��f���ԍ�
	Pos3D m_pos;		//�{�̍��W
	float m_TrunkHeight;//�����W
	int   m_NumTrunk;	//���̐��i�Œ�R�j
};