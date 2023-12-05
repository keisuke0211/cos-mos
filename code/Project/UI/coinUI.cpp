//========================================
// 
// ���P�b�g�p�[�cUI�̏���
// Author: RYUKI FUJIWARA
// 
//========================================
#include "coinUI.h"
#include "../main.h"
#include "../Object/Item/coin.h"

#define FRAME_INTER (Scale2D(256.0f,64.0f))				//�t���[���Ԋu
#define SCALE		(50.0f)								//�g�k�l
#define INITPOS		(D3DXVECTOR3(1000.0f,50.0f, 0.0f))	//�����ʒu

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
void CCoinUI::Init(void) {

	m_pos = INITPOS;
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

	//UI�t���[��
	RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_pos.x + 48.0f, m_pos.y), 0.0f)
		->SetSize(FRAME_INTER.x, FRAME_INTER.y)
		->SetTex(m_TexIdx[1]);

	//��
	RNLib::Text2D().Put(PRIORITY_UI, CreateText("%d",CCoin::GetNumAll() + CCoin::GetNum()), CText::ALIGNMENT::LEFT, 0, Pos2D(m_pos.x + 30.0f, m_pos.y), 0.0f, true)
		->SetSize(Size2D(32.0f, 32.0f))
		->SetCol(INITCOLOR);

	//�R�C��
	RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR2(m_pos.x, m_pos.y), 0.0f)
		->SetSize(m_scale.x, m_scale.y)
		->SetTex(m_TexIdx[0]);
}

//========================================
// ��������
// Author: RYUKI FUJIWARA
//========================================
CCoinUI *CCoinUI::Create(void)
{
	CCoinUI *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CCoinUI;

	// ����������
	pObj->Init();

	return pObj;
}