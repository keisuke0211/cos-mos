//========================================================
// 
// ���S�p�[�e�B�N������
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

//=======================================
// �R���X�g���N�^
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);
}

//=======================================
// �f�X�g���N�^
//=======================================
CEffect_Death::~CEffect_Death()
{
	Manager::EffectMgr()->SubList(this);
}

//=======================================
//�X�V����
//=======================================
void CEffect_Death::Update(void)
{
	//��ޕʍX�V����
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); break;
		case TYPE::INK: UpdateType_Ink();  break;
	}
	
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		->SetBillboard(true)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	//�m�����n���ʍX�V����
	Life();
}

//=======================================
//�y�{�[���z�X�V����
//=======================================
void CEffect_Death::UpdateType_Ball(void)
{
	Move();
	Spin();
}

//=======================================
//�y�C���N�z�X�V����
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{

}

//=======================================
//�m�ړ��n���ʍX�V����
//=======================================
void CEffect_Death::Move(void)
{
	m_Info.pos = m_Info.move;
}

//=======================================
//�m��]�n���ʍX�V����
//=======================================
void CEffect_Death::Spin(void)
{
	m_Info.rot = m_Info.spin;
}

//=======================================
//�m�����n���ʍX�V����
//=======================================
void CEffect_Death::Life(void)
{
	//���������݂��A�s����
	if (m_Info.pLife != NULL &&	--*m_Info.pLife <= 0)
		CObject::Delete();
}