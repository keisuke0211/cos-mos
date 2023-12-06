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

	//�Y���߂荞�܂���Œፂ��
	static const float CAVEIN_DIFF_HEIGHT;

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

	//===============================
	//�߂荞�ݗʔ��f�����iCave in  ==  �ւ���
	//===============================
	void CaveInTrunkHeight(float fCaveInHeight);

	//===============================
	//�ʒu�擾�i�߂荞�ݗʂ��܂߂�j
	//===============================
	D3DXVECTOR3 GetPosCaveIn(void);

	//===============================
	//�O��ʒu�擾�i�߂荞�ݗʂ��܂߂�j
	//===============================
	D3DXVECTOR3 GetPosOldCaveIn(void);

private:
	void SetOld(float fCaveInHeight);	//�O�����ۑ�
	void PutModel(void);//���f���z�u

	int m_TrunkModelID;	//�������̃��f���ԍ�
	Pos3D m_PilePos;	//�{�̍��W
	int   m_NumTrunk;	//���̐��i�Œ�R�j
	float m_fEvenTrunkCorrHeight;//���̐��������������ꍇ�̕␳�l�i��Ȃ�0.0f
	float m_TrunkHeight;	//�����W
	float m_TrunkHeightOld;	//�O��̊����W
	float m_StartTrunkHeight;

	//�G�t�F�N�g
	int m_nTex[2];			//�e�N�X�`���ԍ�
};