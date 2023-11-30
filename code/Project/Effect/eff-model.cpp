//========================================
// 
// ���f���G�t�F�N�g����
// Author:RYUKI FUJIWARA
// 
//========================================
#include "eff-model.h"
#include "../main.h"

#define PI				(314)		//�~��
#define MAGNI			(100.0f)	//�{��

const char* s_nIdx[] =
{
	"data\\MODEL\\MeteoriteFragment_0.x",
};

//�ۑ��p

/*if (RNLib::Input().GetKeyPress(DIK_RETURN))
{
Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(m_aInfo[0].pos.x, m_aInfo[0].pos.y -7.0f * cosf(m_aInfo[0].rot.z), m_aInfo[0].pos.z), m_aInfo[0].rot, INITSCALE3D * 0.2f, INITCOLOR);
}*/


//========================================
// �ÓI�ϐ�
//========================================
int CEffect_Model::m_nNumAll = 0;

//========================================
// �R���X�g���N�^
//========================================
CEffect_Model::CEffect_Model(void)
{
	Manager::EffectMgr()->AddList(this);

	m_pos = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_scale = INITD3DXVECTOR3;
	m_scalemag = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_col = INITCOLOR;
	m_nNumAll++;
}

//========================================
// �f�X�g���N�^
//========================================
CEffect_Model::~CEffect_Model()
{
	Manager::EffectMgr()->SubList(this);

	m_nNumAll--;
}

//========================================
// ������
//========================================
HRESULT CEffect_Model::Init(int nIdx,int nCount,D3DXVECTOR3 move)
{
	m_nIdx = RNLib::Model().Load(s_nIdx[nIdx]);
	m_nCount = m_nCountMax = nCount;

	m_move = D3DXVECTOR3(
		sinf((float)(rand() % PI  / MAGNI)) + (float)(rand() % (int)move.x - move.x * 0.5f),	//x�̈ړ���
		(move.y - (float)(rand() %(int)move.y)) * (m_pos.y / fabsf(m_pos.y)),		//y�̈ړ���
		move.z);									//z�̈ړ���

	return S_OK;
}

//========================================
// �I��
//========================================
void CEffect_Model::Uninit(void)
{

}

//========================================
// �X�V
//========================================
void CEffect_Model::Update(void)
{
	//���f���z�u
	RNLib::Model().Put(PRIORITY_EFFECT, m_nIdx, m_pos, m_rot,m_scale, false)
		->SetCol(m_col);

	if (m_nCount != 0) {
		m_nCount--;

		CollisionBound();

		//�����v�Z
		float fCountRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_nCount, m_nCountMax);

		m_move.y -= 0.49f * (m_pos.y / fabsf(m_pos.y));

		m_col.a = m_col.a * fCountRate;
		m_pos += m_move;

		m_move.x += (0.0f - m_move.x) * 0.1f;
		m_move.y += (0.0f - m_move.y) * 0.5f;

		if (m_nCount <= 0)
			Delete();
	}
}

//========================================
// �`��
//========================================
void CEffect_Model::Draw(void)
{
}

//========================================
// �����蔻��(�o�E���h)
//========================================
void CEffect_Model::CollisionBound(void)
{
	// �I�u�W�F�N�g1��1�����Ă���
	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {

		// �擾�����I�u�W�F�N�g���L���X�g
		CStageObject* stageObj = (CStageObject*)obj;

		// ��ގ擾
		const CStageObject::TYPE type = stageObj->GetType();

		switch (type) {
		case CStageObject::TYPE::BLOCK: {
			CBlock* pBlock = (CBlock*)stageObj;
			if (!pBlock->GetCollision())
				continue;

			if (m_pos.x >= pBlock->GetPos().x - pBlock->GetWidth() * 0.5f
				&& m_pos.x <= pBlock->GetPos().x + pBlock->GetWidth() * 0.5f
				&&m_pos.y >= pBlock->GetPos().y - pBlock->GetHeight() * 0.5f
				&& m_pos.y <= pBlock->GetPos().y + pBlock->GetHeight() * 0.5f)
			{
				m_move.y *= -8;
			}
		}break;
		}
	}
}