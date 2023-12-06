//========================================
// 
// �G�t�F�N�g����
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"
//****************************************
// �N���X
//****************************************
class CEffect_Model : public CObject {
public:

	// *** �֐� ***
	CEffect_Model();
	~CEffect_Model();

	/* ������	*/HRESULT Init(int nIdx, int nCount,D3DXVECTOR3 move);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	// -- �ݒ� ---------------------------------------------
	/* �F		*/void SetColor(const Color col) { m_col = col; }
	/* �ʒu		*/void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	/* �ʒu		*/void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	/* �ʒu		*/void SetScale(const D3DXVECTOR3 scale) { m_scale = scale; }

	// -- ���� ---------------------------------------------
	/* �F		*/Color GetColor(void) { return m_col; }
	/* �ʒu		*/D3DXVECTOR3 GetPos(void) { return m_pos; }
	/* �ʒu		*/D3DXVECTOR3 GetScale(void) { return m_scale; }

private:

	void CollisionBound(void);

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����

	// *** �ϐ� ***
	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posold;	// �O��ʒu
	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_spin;		// ��]��
	D3DXVECTOR3 m_scale;		// �g�k
	Color m_col;				// ���_�J���[
	int m_nCount;
	int m_nCountMax;
	int m_nIdx;
};
