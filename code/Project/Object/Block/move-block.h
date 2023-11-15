//========================================
// 
// �����u���b�N���� [move-block.h]
// Author:HIROMU KOMURO
// 
//========================================

#pragma once

#include "../../../_RNLib/RNlib.h"

//========================================
// �N���X
//========================================
class CMoveBlock : public CStageObject
{
public:

	// ���ʏ��
	struct Info
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 posOld;		// �ʒu(�O��)
		D3DXVECTOR3 m_posV;		// �n�_�ʒu
		D3DXVECTOR3 m_posL;		// �I�_�ʒu
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3	size;		// �T�C�Y
		float frefdef;			// ���˂��鋗��
		int nType;				// ���
		int nID;				// ID
		int nModelIdx;
		bool bXReturn;
	};

	//�֐�
	CMoveBlock();
	~CMoveBlock();
	
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�ݒ�
	void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = m_Info.posOld = pos; }
	void SetPosInfo(const D3DXVECTOR3 posV, const D3DXVECTOR3 posL) { m_Info.m_posV = posV, m_Info.m_posL = posL; }	// �ʒu�̊e��ݒ�
	void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	void SetRefdef(const float refdef) { m_Info.frefdef = refdef; }
	void SetRot(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	void SetSize(const D3DXVECTOR3 size) { m_Info.size = size; }
	void SetXReturn(const bool bXReturn) { m_Info.bXReturn = bXReturn; }

	/* ���f���ԍ�	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	//�擾
	D3DXVECTOR3 GetPos(void)	{ return m_Info.pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_Info.posOld; }
	D3DXVECTOR3 GetRot(void)	{ return m_Info.rot; }
	D3DXVECTOR3 GetMove(void)	{ return m_Info.move; }
	D3DXVECTOR3 GetSize(void)	{ return m_Info.size; }

private:

	//�ÓI�ϐ�
	static int m_nNumAll;		// ����

	//�ϐ�
	Info m_Info;		// ���ʏ��

	int nModelIdx;

};
