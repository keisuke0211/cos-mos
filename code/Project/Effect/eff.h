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
class CEff : public CObject{
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 spin;		// ��]��
		D3DXVECTOR3 scale;		// �g�k
		D3DXVECTOR3 scalemag;	// �g�k�{��
		Color col;				// ���_�J���[
		int nCount;
		int nCountMax;
		int nTex;
		bool Billboard;
	};

	// *** �֐� ***
	CEff();
	~CEff();

	/* ������	*/HRESULT Init(int nTex, int nCount, D3DXVECTOR3 spin);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	// -- �ݒ� ---------------------------------------------
	/* �F		*/void SetColor(const Color col) { m_Info.col = col; }
	/* �ʒu		*/void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* �ʒu		*/void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	/* �ʒu		*/void SetScale(const D3DXVECTOR3 scale) { m_Info.scale = scale; }
	/* �X�P�[���̔{��		*/void SetScalemag(const D3DXVECTOR3 scale) { m_Info.scalemag = scale; }
	/* �ʒu		*/void SetBillboard(const bool billboard) { m_Info.Billboard = billboard; }

	// -- ���� ---------------------------------------------
	/* �F		*/Color GetColor(void) { return m_Info.col; }
	/* �ʒu		*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* �ʒu		*/D3DXVECTOR3 GetScale(void) { return m_Info.scale; }

private:

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����

	// *** �ϐ� ***
	Info m_Info;		// ���ʏ��
};
