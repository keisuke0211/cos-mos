//========================================
// 
// ���P�b�g�p�[�cUI�̏���
// Author: RYUKI FUJIWARA
// 
//========================================
#include "coinUI.h"
#include "../main.h"
#include "../Object/Item/coin.h"

#define FRAME_INTER (Scale2D(128.0f,30.0f))				//�t���[���Ԋu
#define SCALE		(4.0f)								//�g�k�l

//�ÓI�����o�ϐ�

//========================================
// �R���X�g���N�^
//========================================
CCoinUI::CCoinUI(void) {

	m_pos = INITD3DXVECTOR3;
	m_scale = Scale2D(SCALE, SCALE);
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\coin.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\parts_frame.png");
	m_num = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CCoinUI::~CCoinUI(void) {

}

//========================================
// ����������
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Init(D3DXVECTOR3 pos) {

	SetPos(pos);
}

//========================================
// �I������
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Uninit(void) {

}

//========================================
// �X�V����
// Author: RYUKI FUJIWARA
//========================================
void CCoinUI::Update(void) {

	//�R�C��
	RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f))
		->SetSize(m_scale.x, m_scale.y)
		->SetTex(m_TexIdx[0])
		->SetZTest(false);

	//��
	RNLib::Text3D().Put(PRIORITY_UI, CreateText("%d",CCoin::GetNumAll() + CCoin::GetNum()), CText::ALIGNMENT::LEFT, 0, Pos3D(m_pos.x + 4.5f, m_pos.y + 0.8f, m_pos.z), D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f))
		->SetSize(Size2D(3.0f, 3.0f))
		->SetFontIdx(1)
		->SetCol(INITCOLOR)
		->SetZTest(false);
}

//========================================
// ��������
// Author: RYUKI FUJIWARA
//========================================
CCoinUI *CCoinUI::Create(D3DXVECTOR3 pos)
{
	CCoinUI *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CCoinUI;

	// ����������
	pObj->Init(pos);

	return pObj;
}