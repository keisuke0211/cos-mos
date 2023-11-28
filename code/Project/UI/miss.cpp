//========================================
// 
// �~�X�̏���
// Author: KOMURO HIROMU
// 
//========================================
#include "miss.h"
#include "../main.h"

#define MAX_MAG_CNT		(60)		// �{���̍ő�J�E���g
#define MAX_SCALE		(Scale2D(512.0f * 0.75f,128.0f * 0.75f))
//========================================
// �R���X�g���N�^
//========================================
CMiss::CMiss()
{
	Manager::StageObjectMgr()->AddList(this);

	// ��ނ̐ݒ�
	m_type = TYPE::MISS;

	m_TexIdx = RNLib::Texture().Load("data\\TEXTURE\\Miss.png");
	m_nMagCnt = 0;
	m_Scale = MAX_SCALE;
}
//========================================
// �f�X�g���N�^
//========================================
CMiss::~CMiss()
{

}

//========================================
// ����������
// Author: KOMURO HIROMU
//========================================
void CMiss::Init(void) {


}

//========================================
// �I������
// Author: KOMURO HIROMU
//========================================
void CMiss::Uninit(void) {

}

//========================================
// �X�V����
// Author: KOMURO HIROMU
//========================================
void CMiss::Update(void) {
	m_nMagCnt++;	// �J�E���g�𑝉�

	// �����v�Z 
	float fCountRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_nMagCnt, MAX_MAG_CNT);

	RNLib::Polygon3D().Put(PRIORITY_UI, Manager::GetMainCamera()->GetPosR(), INITROT3D)
		->SetSize(m_Scale.x * fCountRate, m_Scale.y * fCountRate)
		->SetZTest(false)
		->SetLighting(false)
		->SetTex(m_TexIdx)
		->SetBillboard(true);
}

//========================================
// ��������
// Author: KOMURO HIROMU
//========================================
CMiss *CMiss::Create(void)
{
	CMiss *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMiss;

	// ����������
	pObj->Init();

	return pObj;
}