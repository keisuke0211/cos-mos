//========================================
// 
// �L�т錢�̏���
// Author:KOMURO HIROMU
// Arrange:HIRASAWA SHION
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
#include "../../Character/player.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CExtenddog�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//���E���K�p�T�C�Y
const float CExtenddog::HEAD_HIP_SIZE = CStageObject::SIZE_OF_1_SQUARE * 0.25f;

//���f���p�X
const char *CExtenddog::MODEL_PATH[(int)Parts::Max] = 
{
	"data\\MODEL\\WallDog\\WallDog_Hip.x",  // ���K
	"data\\MODEL\\WallDog\\WallDog_Body.x", // ��
	"data\\MODEL\\WallDog\\WallDog_Head.x", // ��
};
//========================================
// �R���X�g���N�^
//========================================
CExtenddog::CExtenddog(void) {
	Manager::StageObjectMgr()->AddList(this);

	//�������
	m_type = TYPE::EXTEND_DOG;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bElasticity = false;
	m_bInversion = false;
	m_nCntShrink = 0;
	m_nHeightMin = m_nHeightMax = 1;
	m_HeadPos = INITD3DXVECTOR3;
	m_BodyPos = INITD3DXVECTOR3;
	m_BodyPosOld = INITD3DXVECTOR3;
	m_HipPos = INITD3DXVECTOR3;

	m_StartBodyPos = INITD3DXVECTOR3;
	m_StartHeight = 0.0f;

	for (int nCntModel = 0; nCntModel < (int)Parts::Max; nCntModel++)
	{
		m_modelIdx[nCntModel] = RNLib::Model().Load(MODEL_PATH[nCntModel]);
	}
}

//========================================
// �f�X�g���N�^
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// ����������
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Init(void) {

	CFloat Height = SIZE_OF_1_SQUARE * m_bElasticity ? 
		m_nHeightMin : m_nHeightMax;

	CFloat PosDiffY = Height + m_HipPos.y;
	
	//�̂̏����ʒu�ݒ�
	m_StartBodyPos = m_BodyPos = Pos3D(m_HeadPos.x, PosDiffY, m_HeadPos.z);// * 0.5f

	//�����Z�o
	m_StartHeight = m_height = fabsf(Height - m_BodyPos.y) - HEAD_HIP_SIZE;

	if (!m_bInversion) return;

	m_rot.z = D3DX_PI;
	m_StartBodyPos.y *= -1.0f;
	m_BodyPos.y *= -1.0f;
}

//========================================
// �I������
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// �X�V����
// Author:KOMURO HIROMU / HIRASAWA SHION
//========================================
void CExtenddog::Update(void) {

	//�O��ʒu�X�V
	m_BodyPosOld = m_BodyPos;

	//��ԕʍX�V����
	switch (m_state)
	{
		case STATE::NONE:   UpdateState_None(); break;
		case STATE::RETURN: UpdateState_Return(); break;
	}

	// �����v�Z 
	CFloat fCountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);

	const Rot3D Rot = !m_bInversion ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI);

	//�T�C�Y����
	CFloat HeightRate = (m_nHeightMax - m_nHeightMin) * fCountRate;

	//����
	m_height = SIZE_OF_1_SQUARE *((float)m_nHeightMin + HeightRate);
	
	//�����̍���
	CFloat HalfHeight = m_height * 0.5f;

	// �̈ʒu
	m_BodyPos.y = !m_bInversion ?
		((m_HipPos.y + HEAD_HIP_SIZE) + HalfHeight) :
		((m_HipPos.y - HEAD_HIP_SIZE) - HalfHeight);

	//�̃X�P�[��
	m_scale.y = m_height * 10.0f;

	//���p�@�̂̃T�C�Y�Ǝ��g�̃T�C�Y�̍��v
	CFloat HeadSize = HalfHeight + HEAD_HIP_SIZE;

	// ���ʒu
	m_HeadPos.y = !m_bInversion ?
		m_BodyPos.y + HeadSize :
		m_BodyPos.y - HeadSize;

	// ��
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Body], m_BodyPos, Rot, m_scale, false)->SetOutLineIdx(true);

	// ��
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Head], m_HeadPos, Rot, false)->SetOutLineIdx(true);

	// �K
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Hip], m_HipPos, Rot, false)->SetOutLineIdx(true);

#if 0
	const Pos3D Slide = Pos3D(SIZE_OF_1_SQUARE, 0.0f, 0.0f);
	const Pos3D slidebody = Pos3D(20.0f, 0.0f, 0.0f);

	// �K
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HipPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 0.5f)
		->SetCol(Color{ 255,0,0,255 })
		->SetZTest(false);

	// ��
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HeadPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 0.5f)
		->SetCol(Color{ 0,255,0,255 })
		->SetZTest(false);

	// ��
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_BodyPos + slidebody, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, m_height)
		->SetCol(Color{ 0,0,255,255 })
		->SetZTest(false);

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(3.0f, 3.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HipPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HeadPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_BodyPos + slidebody, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);

	if (!m_bInversion)
	{
		CFloat IDealMax = m_HeadPos.y + SIZE_OF_1_SQUARE * 0.5f;
		CFloat IDealMin = m_HipPos.y - SIZE_OF_1_SQUARE * 0.5f;
		RNLib::Text2D().PutDebugLog(String("���z��Y:%.1f, ���z��Y:%.1f  ���z����:%.1f  ���ۍ���%.1f",
									IDealMax, IDealMin, IDealMax - IDealMin, m_height));
	}
	else
	{
		CFloat IDealMax = m_HipPos.y + SIZE_OF_1_SQUARE * 0.5f;
		CFloat IDealMin = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.5f;
		RNLib::Text2D().PutDebugLog(String("���z��Y:%.1f, ���z��Y:%.1f  ���z����:%.1f  ���ۍ���%.1f",
									IDealMax, IDealMin, IDealMax - IDealMin, m_height));
	}
#endif
}

//========================================
// ��ԁy�����Ȃ��z�X�V����
// Author:HIRASAWA SHION
//========================================
void CExtenddog::UpdateState_None(void)
{
	if (m_bElasticity == false)
	{
		// �k�ރJ�E���g
		m_nCntShrink--;
		if (m_nCntShrink <= 0)
			m_nCntShrink = 0;
	}
	else
	{
		// �k�ރJ�E���g
		m_nCntShrink++;
		if (m_nCntShrink >= MAX_COUNT)
			m_nCntShrink = MAX_COUNT;
	}
}

//========================================
// ��ԁy�߂�z�X�V����
// Author:HIRASAWA SHION
//========================================
void CExtenddog::UpdateState_Return(void)
{
	if (m_bElasticity == false)
	{
		// �k�ރJ�E���g
		m_nCntShrink++;
		if (m_nCntShrink >= MAX_COUNT){
			m_nCntShrink = MAX_COUNT;
			m_state = STATE::NONE;
		}
	}
	else
	{
		// �k�ރJ�E���g
		m_nCntShrink--;
		if (m_nCntShrink <= 0) {
			m_nCntShrink = 0;
			m_state = STATE::NONE;
		}
	}
}

//========================================
//�����蔻��p�̍����擾
// Author:HIRASAWA SHION
//========================================
float CExtenddog::GetColliHeight(void)
{
	//�ō��E�Œ���W
	float MaxY = 0.0f;
	float MinY = 0.0f;

	//���E���K����ō��E�Œ���W���Z�o�i���]���Ă��瓪�E���K���t
	if (!m_bInversion)
	{
		MaxY = m_HeadPos.y + SIZE_OF_1_SQUARE * 0.25f;
		MinY = m_HipPos.y - SIZE_OF_1_SQUARE * 0.25f;
	}
	else
	{
		MaxY = m_HipPos.y + SIZE_OF_1_SQUARE * 0.25f;
		MinY = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.25f;
	}

	//�ō��ƍŒ�̔�����Ԃ�
	return (MaxY - MinY) * 0.5f;
}

//========================================
//���K�̕������擾
//========================================
int CExtenddog::GetHipRot(void)
{
	//���]���Ă��Ȃ��Ȃ牺���A���]���Ă�Ȃ�㑤
	if (!m_bInversion) return (int)CCollision::ROT::UNDER;
	else return (int)CCollision::ROT::OVER;
}