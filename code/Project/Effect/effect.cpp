//========================================
// 
// �G�t�F�N�g����
// Author:RYUKI FUJIWARA
// 
//========================================
// *** block.cpp ***
//========================================
#include "effect.h"
#include "../main.h"

//========================================
// �ÓI�ϐ�
//========================================
int CEffect::m_nNumAll = 0;

//========================================
// �R���X�g���N�^
//========================================
CEffect::CEffect(void)
{
	Manager::EffectMgr()->AddList(this);

	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.scalemag = INITD3DXVECTOR3;
	m_Info.rot =INITD3DXVECTOR3;
	m_Info.col = INITCOLOR;
	m_nNumAll++;
}

//========================================
// �f�X�g���N�^
//========================================
CEffect::~CEffect()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// ������
//========================================
HRESULT CEffect::Init(int nTex, int nCount, D3DXVECTOR3 spin)
{
	m_Info.nTex = nTex;
	m_Info.nCount = m_Info.nCountMax = nCount;
	m_Info.spin = spin;

	return S_OK;
}

//========================================
// �I��
//========================================
void CEffect::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CEffect::Update(void)
{
	// �ߋ��̈ʒu
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetBillboard(m_Info.Billboard)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x, m_Info.scale.y)
		->SetAlphaBlendMode(CDrawState::ALPHA_BLEND_MODE::ADD)
		->SetZTest(false);

	m_Info.nCount--;

	//�����v�Z
	float fCountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_Info.nCount, m_Info.nCountMax);

	m_Info.col.a = m_Info.col.a * fCountRate;

	m_Info.rot += m_Info.spin * fCountRate;

	m_Info.pos += m_Info.move;

	m_Info.scale.x *= m_Info.scalemag.x;
	m_Info.scale.y *= m_Info.scalemag.y;

	if (m_Info.nCount <= 0)
	{
		CObject::Delete();
	}
}

//========================================
// �`��
//========================================
void CEffect::Draw(void)
{
}