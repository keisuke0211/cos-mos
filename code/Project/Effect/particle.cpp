//========================================
// 
// �G�t�F�N�g����
// Author:RYUKI FUJIWARA
// 
//========================================
// *** block.cpp ***
//========================================
#include "particle.h"
#include "../main.h"

#define PI				(314)		//�~��
#define HARF_PI			(157)		//���~��
#define MAGNI			(100.0f)	//�{��
#define ATTEN_RATE		(0.3f)		//������
#define FLOATUP_RATE	(0.05f)		//������Y

//========================================
// �ÓI�ϐ�
//========================================
int CParticle::m_nNumAll = 0;
D3DXVECTOR3 CParticle::m_rot = INITD3DXVECTOR3;

//========================================
// �R���X�g���N�^
//========================================
CParticle::CParticle(void)
{
	Manager::EffectMgr()->AddList(this);

	m_Info.pos = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.rot = INITD3DXVECTOR3;
	m_Info.scale = INITD3DXVECTOR3;
	m_Info.col = COLOR_WHITE;
	m_move = INITVECTOR3D;

	m_nNumAll++;
}

//========================================
// �f�X�g���N�^
//========================================
CParticle::~CParticle()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// ������
//========================================
HRESULT CParticle::Init(int nTex,int nCount)
{
	if (m_type == TYPE::TYPE_NORMAL)
	{
		m_Info.move = D3DXVECTOR3(
			sinf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % m_Info.rdmMagni - m_Info.rdmMagni * 0.5f),	//x�̈ړ���
			cosf((float)(rand() % PI - HARF_PI) / MAGNI) * (float)(rand() % m_Info.rdmMagni - m_Info.rdmMagni * 0.5f),	//y�̈ړ���
			INITD3DXVECTOR3.z);
	}
	else if (m_type == TYPE::TYPE_SPIN)
	{
		m_Info.move = D3DXVECTOR3(
			m_move.x * sinf(m_Info.rot.z),
			m_move.y * cosf(m_Info.rot.z),
			0.0f);
	}
	else if (m_type == TYPE::TYPE_FLOATUP)
	{
		float rate = 0.01f + RNLib::Number().GetRandomFloat(0.01f);

		m_Info.move = D3DXVECTOR3(
			m_move.x * sinf(m_Info.rot.z) * rate,
			m_move.y * cosf(m_Info.rot.z) * rate,
			0.0f);

		m_Info.rot.x = 0.0f;
		m_Info.rot.y = 0.0f;
	}
	else if (m_type == TYPE::TYPE_STOP)
	{
		m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	
	m_Info.col = COLOR_WHITE;
	m_Info.nTex = nTex;
	m_Info.nCount = m_Info.nCountMax = nCount;

	return S_OK;
}

//========================================
// �I��
//========================================
void CParticle::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CParticle::Update(void)
{
	//�ړ��ʉ��Z
	m_Info.pos += m_Info.move;

	//�����v�Z
	float fCountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_Info.nCount, m_Info.nCountMax);

	D3DXVECTOR3 fScaleRate = m_Info.scale * fCountRate;

	m_Info.scale = fScaleRate;

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetCol(m_Info.col)
		->SetSize(m_Info.scale.x, m_Info.scale.y)
		->SetZTest(true)
		->SetAlphaBlendMode(m_Info.alphamode);

	//�ړ��ʌ���
	if (m_type != TYPE::TYPE_FLOATUP){
		m_Info.move.x += (0.0f - m_Info.move.x) * ATTEN_RATE;
		m_Info.move.y += (0.0f - m_Info.move.y) * ATTEN_RATE;
	}
	else{
		m_Info.move.x += (0.0f - m_Info.move.x) * FLOATUP_RATE;
		m_Info.move.y += (0.0f - m_Info.move.y) * FLOATUP_RATE;
	}

	//�����ɂ��Ă���
	m_Info.col.a = m_Info.col.a * fCountRate;

	//�������炷
	m_Info.nCount--;

	if (m_Info.nCount <= 0)
		CObject::Delete();
}

//========================================
// �`��
//========================================
void CParticle::Draw(void)
{

}