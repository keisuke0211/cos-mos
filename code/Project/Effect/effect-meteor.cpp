//========================================
// 
// 覐΃G�t�F�N�g����
// Author:KOMURO HIROMU
// 
//========================================
// *** effect-manager.cpp ***
//========================================
#include "effect-meteor.h"
#include "../main.h"

//========================================
// �ÓI�ϐ�
//========================================
int CEffect_Meteor::m_nNumAll = 0;
int CEffect_Meteor::s_nModelIdx[3] = {};
//========================================
// �R���X�g���N�^
//========================================
CEffect_Meteor::CEffect_Meteor(void)
{
	//Manager::BlockMgr()->AddList(this);

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_Info[nCnt].pos = INITD3DXVECTOR3;
		m_Info[nCnt].rot = INITD3DXVECTOR3;
		m_Info[nCnt].move = INITD3DXVECTOR3;
		m_Info[nCnt].scale = Scale3D(1.0f,1.0f,1.0f);
		m_Info[nCnt].col = INITCOLOR;
	}
	m_pos = INITD3DXVECTOR3;
	m_nCount = 60;
	m_nNumAll++;
}

//========================================
// �f�X�g���N�^
//========================================
CEffect_Meteor::~CEffect_Meteor()
{
	m_nNumAll--;
}

//========================================
// ������
//========================================
HRESULT CEffect_Meteor::Init(void)
{
	s_nModelIdx[0] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_0.x");
	s_nModelIdx[1] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_1.x");
	s_nModelIdx[2] = RNLib::Model().Load("data\\MODEL\\MeteoriteFragment_2.x");


	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		m_Info[nCnt].pos.x = m_pos.x + (rand() % 10 - 5);
		m_Info[nCnt].pos.y = m_pos.y + (rand() % 10 - 5);
		m_Info[nCnt].move.x = (rand() % 4 - 2);
		m_Info[nCnt].move.y = (rand() % 4 - 2);
		m_Info[nCnt].col = INITCOLOR;

	}
	return S_OK;
}

//========================================
// �I��
//========================================
void CEffect_Meteor::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CEffect_Meteor::Update(void)
{
	m_nCount++;

	//�����v�Z
	float fCountRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_nCount,60);

	for (int nCnt = 0; nCnt < 10; nCnt++)
	{
		//m_Info[nCnt].scale = Scale3D(1.0f,1.0f,1.0f) * fCountRate;	// �X�P�[���̔{��

		//���f���z�u
		RNLib::Model().Put(m_Info[nCnt].pos, m_Info[nCnt].rot,m_Info[nCnt].scale, s_nModelIdx[0], true)
			->SetOutLine(true);

		// �ʒu�̑���
		m_Info[nCnt].pos += m_Info[nCnt].move;
	}

	if (m_nCount <= 0)
	{
		//CObject::Delete();
	}
}

//========================================
// �`��
//========================================
void CEffect_Meteor::Draw(void)
{

}