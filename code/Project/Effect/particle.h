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
		_RNC_DrawState::ALPHA_BLEND_MODE alphamode;
	};

	// ���ʏ��
	enum class TYPE
	{
		TYPE_NORMAL = 0,	//�X�^���_�[�h
		TYPE_SPIN,			//��]����
		TYPE_FLOATUP,		//��ɍs��
		TYPE_STOP,			//�Ƃǂ܂�
		TYPE_FIXED,
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
	/* �F			*/void SetColor(const Color col) { m_Info.col = col; }
	/* �ʒu			*/void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* �ړ���		*/void SetMove(const D3DXVECTOR3 move) { m_move = move; }
	/* ����			*/void SetRot(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	/* �g�k			*/void SetScale(const D3DXVECTOR3 scale) { m_Info.scale = scale; }
	/* ���			*/void SetType(const TYPE type) { m_type = type; }
	/* �{��			*/void SetRdmMagni(const int magni) { m_Info.rdmMagni = magni; }
	/* ���Z����		*/void SetAlphaMode(const _RNC_DrawState::ALPHA_BLEND_MODE  alphamode) { m_Info.alphamode = alphamode; }
	/* Ztest		*/void SetZtest(const bool bZtest) { m_bZtest = bZtest; }
	/* ����Vec		*/void SetVec3D(const bool bVec3D) { m_bVec3D = bVec3D; }


	// -- ���� ---------------------------------------------
	/* �F		*/Color GetColor(void) { return m_Info.col; }
	/* �ʒu		*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* �g�k		*/D3DXVECTOR3 GetScale(void) { return m_Info.scale; }

private:

	// *** �ÓI�ϐ� ***
	static int m_nNumAll;		// ����
	static D3DXVECTOR3 m_rot;	// �ړ���
	D3DXVECTOR3 m_move;			// �ړ���

	// *** �ϐ� ***
	Info m_Info;		// ���ʏ��
	TYPE m_type;
	bool m_bZtest;
	bool m_bVec3D;
};
