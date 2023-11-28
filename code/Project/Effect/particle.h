//========================================
// 
// �p�[�e�B�N������
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"

//****************************************
// �N���X
//****************************************
class CParticle : public CObject {
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 rot;		// �ړ���
		D3DXVECTOR3 scale;		// �g�k
		Color col;				// ���_�J���[
		int nCount;
		int nCountMax;
		int nTex;
		int rdmMagni;
		CDrawState::ALPHA_BLEND_MODE alphamode;
	};

	// ���ʏ��
	enum class TYPE
	{
		TYPE_NORMAL = 0,
		TYPE_SPIN,
		MAX
	};

	// *** �֐� ***
	CParticle();
	~CParticle();

	/* ������	*/HRESULT Init(int nTex,int nCount);
	/* �I��		*/void Uninit(void);
	/* �X�V		*/void Update(void);
	/* �`��		*/void Draw(void);

	// -- �ݒ� ---------------------------------------------
	/* �F		*/void SetColor(const Color col) { m_Info.col = col; }
	/* �ʒu		*/void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* �ʒu		*/void SetRot(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	/* �g�k		*/void SetScale(const D3DXVECTOR3 scale) { m_Info.scale = scale; }
	/* �g�k		*/void SetType(const TYPE type) { m_type = type; }
	/* �g�k		*/void SetRdmMagni(const int magni) { m_Info.rdmMagni = magni; }
	/* �g�k		*/void SetAlphaMode(const CDrawState::ALPHA_BLEND_MODE  alphamode) { m_Info.alphamode = alphamode; }

	// -- ���� ---------------------------------------------
	/* �F		*/Color GetColor(void) { return m_Info.col; }
	/* �ʒu		*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* �g�k		*/D3DXVECTOR3 GetScale(void) { return m_Info.scale; }

private:

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����
	static D3DXVECTOR3 m_rot;	// �ړ���

	// *** �ϐ� ***
	Info m_Info;		// ���ʏ��
	TYPE m_type;
};
