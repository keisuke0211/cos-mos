//========================================
// 
// �G�t�F�N�g�}�l�[�W���[�̏���
// Author:RYUKI FUJIWARA
// 
// 
//========================================
#include "effect-manager.h"

//========================================
// �ÓI�ϐ�
//========================================

//========================================
// �R���X�g���N�^
//========================================
CEffectMgr::CEffectMgr(void)
{
}

//========================================
// �f�X�g���N�^
//========================================
CEffectMgr::~CEffectMgr()
{
}

//========================================
// �X�V
//========================================
void CEffectMgr::Update(void)
{

}

//========================================
// �G�t�F�N�g
//========================================
CEffect *CEffectMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount)
{
	CEffect *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CEffect;

	// ����������
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);


	CObjectMgr::AddList((CObject*)pObj);

	return pObj;
}
//========================================
// �p�[�e�B�N��
//========================================
CParticle *CEffectMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount)
{
	CParticle *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CParticle;

	// ����������
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);

	CObjectMgr::AddList((CObject*)pObj);

	return pObj;
}