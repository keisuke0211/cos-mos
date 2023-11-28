//========================================
// 
// �H��̏���
// Author:KOMURO HIROMU
// 
//========================================
#include"Ghost.h"
#include "../../main.h"
#include "../../collision.h"

#define MAX_MAG_COLOR		(60)
//========================================
// �R���X�g���N�^
//========================================
CGhost::CGhost()
{
	Manager::StageObjectMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\Ghost1.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Ghost2.png");
	m_nMagCnt = 0;
	m_nPlayerNumber = 0;
}
//========================================
// �f�X�g���N�^
//========================================
CGhost::~CGhost()
{

}

//========================================
// ����������
// Author: KOMURO HIROMU
//========================================
void CGhost::Init(void) {


}

//========================================
// �I������
// Author: KOMURO HIROMU
//========================================
void CGhost::Uninit(void) {

}

//========================================
// �X�V����
// Author: KOMURO HIROMU
//========================================
void CGhost::Update(void) {
	m_nMagCnt++;	// �J�E���g�𑝉�

	// �����v�Z 
	float fCountRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_nMagCnt, MAX_MAG_COLOR);

	
	RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), INITROT3D)
		->SetSize(m_Scale.x * fCountRate, m_Scale.y * fCountRate)
		->SetZTest(false)
		->SetLighting(false)
		->SetTex(m_TexIdx[m_nPlayerNumber])
		->SetCol(m_color);

	m_color.a *= 255 * fCountRate;
	if (m_nPlayerNumber == 0)
	{
		m_pos.y += 0.1f;

	}
	else
	{
		m_pos.y -= 0.1f;
	}
}

//========================================
// ��������
// Author: KOMURO HIROMU
//========================================
CGhost *CGhost::Create(D3DXVECTOR3 pos, int nplayernumber)
{
	CGhost *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CGhost;

	// ����������
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetPlayer(nplayernumber);

	return pObj;
}