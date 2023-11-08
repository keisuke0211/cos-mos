//================================================================================================
//
//覐ΐ�����̏���[meteor_generator.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _METEOR_GENERATOR_H_
#define _METEOR_GENERATOR_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// �O���錾
//****************************************
class CMeteor;
//****************************************
// �N���X��`
//****************************************
// ���e�I�̏��\����
class CMeteorGenerator : public CStageObject {
public:
	//========== [[[ �֐��錾 ]]]
	CMeteorGenerator();
	virtual        ~CMeteorGenerator();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; };	// �ړ��ʂ̐ݒ�
	void SetInterval(int nSummonInteval) { m_nSummoninterval = nSummonInteval; }	// �Ԋu�̐ݒ�
private:
	D3DXVECTOR3 m_move;		// �ړ���
	int m_nSummoninterval;	// ���܂��Ԋu
	int m_nCntSummon;		// ���܂��J�E���g
	bool m_bLive;			// �����Ă邩�ǂ���
};

#endif