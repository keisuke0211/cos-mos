//========================================
// 
// �~�`�G�t�F�N�g����
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "eff-circle.h"
#include "../manager.h"
#include "../resource.h"

//========================================
// �R���X�g���N�^
//========================================
CEffCircle::CEffCircle(const int& life, const Pos3D& pos, const Color& col, const float& startScale, const float& endScale) {

	// ���X�g�ɒǉ�
	Manager::EffectMgr()->AddList(this);

	m_life       = 
	m_maxLife    = life;
	m_pos        = pos;
	m_col        = col;
	m_startScale = startScale;
	m_endScale   = endScale;
}

//========================================
// �f�X�g���N�^
//========================================
CEffCircle::~CEffCircle() {

	// ���X�g����폜
	Manager::EffectMgr()->SubList(this);
}

//========================================
// �X�V����
//========================================
void CEffCircle::Update(void) {

	// �����������Z�o
	float lifeRate = RNLib::Ease().Easing(EASE_TYPE::IN_SINE, m_life, m_maxLife);
	float scale    = (lifeRate * m_startScale) + ((1.0f - lifeRate) * m_endScale);

	// �\��
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(Size2D(scale, scale))
		->SetCol(m_col.GetAlphaApplied(lifeRate))
		->SetAlphaBlendMode(_RNC_DrawState::ALPHA_BLEND_MODE::ADD)
		->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::EFF_CIRCLE])
		->SetZTest(false);

	// �������s�������A���g��j��
	if (--m_life == 0)
		Delete();
}