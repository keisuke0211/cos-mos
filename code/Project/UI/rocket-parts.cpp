//========================================
// 
// ���P�b�g�p�[�cUI�̏���
// Author: RYUKI FUJIWARA
// 
//========================================
#include "rocket-parts.h"
#include "../main.h"
#include "../Character/player.h"

#define NUM (2)	//��

//�ÓI�����o�ϐ�
bool CRocketPartsUI::m_bRocketStg = false;
//========================================
// �R���X�g���N�^
//========================================
CRocketPartsUI::CRocketPartsUI(void) {

	m_pos = INITD3DXVECTOR3;
	m_scale = Scale2D(50.0f, 50.0f);
	m_TexIdx = RNLib::Texture().Load("data\\TEXTURE\\rocket_001.png");
	m_colorA = 100;
	m_num = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CRocketPartsUI::~CRocketPartsUI(void) {

}

//========================================
// ����������
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Init(void) {

	m_pos = D3DXVECTOR3(1000.0f, 50.0f, 0.0f);

	for (int nUI = 0; nUI < NUM; nUI++)
	{
		m_state[nUI] = STATE::NONE;
	}
}

//========================================
// �I������
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Uninit(void) {

}

//========================================
// �X�V����
// Author: RYUKI FUJIWARA
//========================================
void CRocketPartsUI::Update(void) {

	if (m_bRocketStg == true)
	{
		CPlayer *pPlayer = CMode_Game::GetPlayer();

		if (pPlayer->GetNumParts() > m_num)
		{
			m_state[m_num++] = STATE::OBTAIN;
		}

		for (int nUI = 0; nUI < NUM; nUI++)
		{
			if (m_state[nUI] == STATE::NONE)
			{
				m_colorA = 100;
			}
			else if (m_state[nUI] == STATE::OBTAIN)
			{
				m_colorA = INITCOLOR.a;
			}

			RNLib::Polygon2D().Put(D3DXVECTOR3(m_pos.x + m_scale.x * (nUI + 1), m_pos.y, m_pos.z), 0.0f)
				->SetSize(m_scale.x, m_scale.y)
				->SetTex(m_TexIdx)
				->SetCol(Color{ INITCOLOR.r,INITCOLOR.g,INITCOLOR.b,(int)m_colorA });
		}
	}
}

//========================================
// ��������
// Author: RYUKI FUJIWARA
//========================================
CRocketPartsUI *CRocketPartsUI::Create(void)
{
	CRocketPartsUI *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRocketPartsUI;

	// ����������
	pObj->Init();
	pObj->SetRocketstg(true);

	return pObj;
}