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
CEffect *CEffectMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount, D3DXVECTOR3 spin, D3DXVECTOR3 move,bool bbilborad, D3DXVECTOR3 scalemag)
{
	CEffect *pObj = NULL;

	pObj = new CEffect;

	// ����������
	pObj->Init(nTex, nCount, spin);
	pObj->SetPos(pos);
	pObj->SetScale(scale);
	pObj->SetColor(col);
	pObj->SetMove(move);
	pObj->SetScalemag(scalemag);
	pObj->SetBillboard(bbilborad);

	return pObj;
}
//========================================
// 覐΃G�t�F�N�g
//========================================
CEffect_Meteor * CEffectMgr::EffectMeteorCreate(D3DXVECTOR3 pos)
{
	CEffect_Meteor *pObj = NULL;

	pObj = new CEffect_Meteor;

	// ����������
	pObj->SetPos(pos);
	pObj->Init();


	return pObj;
}
//========================================
// �p�[�e�B�N��
//========================================
CParticle *CEffectMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos,D3DXVECTOR3 scale,Color col, CParticle::TYPE type ,int nCount, D3DXVECTOR3 rot)
{
	CParticle *pObj = NULL;

	pObj = new CParticle;

	// ����������
	pObj->SetType(type);
	pObj->Init(nTex, nCount);
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetColor(col);
	

	return pObj;
}
