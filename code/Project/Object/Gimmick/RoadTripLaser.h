//========================================
// 
// �������郌�[�U�[�̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// �N���X��`
//****************************************
// ���[�U�[�̏��\����
class CRoadTripLaser : public CStageObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class ROT {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CRoadTripLaser();
	virtual        ~CRoadTripLaser();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static void	Collision(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 Laserpos,D3DXVECTOR2 Lasersize);
	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void	SetPosInfo(const D3DXVECTOR3 posV, const D3DXVECTOR3 posL) { m_posV = posV, m_posL = posL; }	// �ʒu�̊e��ݒ�
	void	SetCol(Color col) { m_col = col; }
	void	SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void	SetLaserSize(const D3DXVECTOR2 size) { m_LaserSize = size; }
	void	SetXMoveReturn(const bool xReturn) { m_bXMoveReturn = xReturn; }

	D3DXVECTOR3 GetBlockpos(void) { return m_pos; }
	D3DXVECTOR3 GetLaserPos(void) { return m_LaserPos; }
	D3DXVECTOR2	GetLaserSize(void) { return m_LaserSize; }
	D3DXVECTOR3 GetMove(void) { return m_move; }


protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	D3DXMATRIX mtx;				// �}�g���b�N�X
	D3DXVECTOR3 m_posV;			// �n�_�ʒu
	D3DXVECTOR3 m_posL;			// �I�_�ʒu
	D3DXVECTOR3 m_move;			// �ړ���
	Color m_col;				// �F
	float m_fGroundDis;			// �n�ʂ܂ł̋���
	D3DXVECTOR3 m_LaserPos;		// ���[�U�[�̈ʒu
	D3DXVECTOR2 m_LaserSize;	// ���[�U�[�̑傫��
	int ModelIdx;
	bool m_bXMoveReturn;			// x�̈ړ��̔��]
};
