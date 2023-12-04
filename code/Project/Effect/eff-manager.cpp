//========================================
// 
// �G�t�F�N�g�}�l�[�W���[�̏���
// Author:RYUKI FUJIWARA
// 
// 
//========================================
#include "eff-manager.h"
#include "../main.h"
#include "../collision.h"
#include "../resource.h"

//========================================
// �ÓI�ϐ�
//========================================

//========================================
// �R���X�g���N�^
//========================================
CEffMgr::CEffMgr(void):CObjectMgr("EffectMgr")
{
}

//========================================
// �f�X�g���N�^
//========================================
CEffMgr::~CEffMgr()
{
}

//========================================
// �X�V
//========================================
void CEffMgr::Update(void)
{

}

//========================================
// �G�t�F�N�g
//========================================
CEff *CEffMgr::EffectCreate(int nTex, D3DXVECTOR3 pos, D3DXVECTOR3 scale,Color col, int nCount, D3DXVECTOR3 spin, D3DXVECTOR3 move,bool bbilborad, D3DXVECTOR3 scalemag)
{
	CEff *pObj = NULL;

	pObj = new CEff;

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
// ���f���G�t�F�N�g
//========================================
CEffect_Model *CEffMgr::ModelEffectCreate(int nIdx, D3DXVECTOR3 pos,D3DXVECTOR3 rot, D3DXVECTOR3 scale, Color col, int nCount, D3DXVECTOR3 move)
{
	CEffect_Model *pObj = NULL;

	pObj = new CEffect_Model;

	// ����������
	pObj->SetPos(pos);
	pObj->Init(nIdx,nCount,move);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetColor(col);

	return pObj;
}
//========================================
// 覐΃G�t�F�N�g
//========================================
CEffect_Meteor * CEffMgr::EffectMeteorCreate(D3DXVECTOR3 pos)
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
CParticle *CEffMgr::ParticleCreate(int nTex, D3DXVECTOR3 pos,D3DXVECTOR3 scale,Color col, CParticle::TYPE type ,int nCount, D3DXVECTOR3 rot,int magni, CDrawState::ALPHA_BLEND_MODE alphamode)
{
	CParticle *pObj = NULL;

	pObj = new CParticle;

	// ����������
	pObj->SetType(type);
	pObj->SetRdmMagni(magni);
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->SetScale(scale);
	pObj->SetAlphaMode(alphamode);
	pObj->Init(nTex, nCount);
	pObj->SetColor(col);

	return pObj;
}

//========================================
// ���S�p�[�e�B�N��
// Author:HIRASAWA SHION
//========================================
CEffect_Death *CEffMgr::DeathParticleCreate(int nIdx, Vector3D pos, Vector3D move, Vector3D rot, Vector3D spin, float size, Color color, CEffect_Death::TYPE type)
{
	//�C���X�^���X����
	CEffect_Death *pEff = new CEffect_Death;

	//���ݒ�
	if (pEff != NULL)
		pEff->SetInfo(pos, pos, move, rot, spin, size, color, NONEDATA, nIdx, type);

	//�C���X�^���X��Ԃ�
	return pEff;
}

//========================================
// �f�X�C���N
// Author:KEISUKE OTONO
//========================================
void CEffMgr::DeathInk(D3DXVECTOR3 pos,int Tex) {

	for (int nInk = 0; nInk < 3; nInk++) {

		bool bSet = false;

		while (1)
		{
			float rot = (float)(rand() % ((int)(100 * 2 * D3DX_PI) + 1) - (int)(D3DX_PI * 100)) / (float)100;
			float scale = (float)(rand() % (int)12 + 3);
			float distance = (float)(rand() % (int)16 + 16);

			D3DXVECTOR3 InkPos = pos;
			D3DXVECTOR3 move = INITD3DXVECTOR3;

			move = D3DXVECTOR3(distance * sinf(rot), distance * cosf(rot),0.0f);

			InkPos += move;

			// �I�u�W�F�N�g1��1�����Ă���
			CObject* obj = NULL;
			while (Manager::StageObjectMgr()->ListLoop(&obj)) {

				// �擾�����I�u�W�F�N�g���L���X�g
				CStageObject* stageObj = (CStageObject*)obj;

				// ��ގ擾
				CStageObject::TYPE type = stageObj->GetType();

				if (type == CStageObject::TYPE::BLOCK || type == CStageObject::TYPE::FILLBLOCK) {

					D3DXVECTOR3 ObjPos = stageObj->GetPos();
					D3DXVECTOR3 ObjSize = D3DXVECTOR3(stageObj->GetWidth(), stageObj->GetHeight(), 0.0f);

					if ((InkPos.x + scale/2) > (ObjPos.x - ObjSize.x/2) &&
						(InkPos.x - scale/2) < (ObjPos.x + ObjSize.x/2) &&
						(InkPos.y + scale/2) > (ObjPos.y - ObjSize.y/2) &&
						(InkPos.y - scale/2) < (ObjPos.y + ObjSize.y/2))
					{// �C���N��������ɂ��鎞�A

						bSet = true;
						Manager::EffectMgr()->EffectCreate(Tex, InkPos, D3DXVECTOR3(scale,scale,0.0f), Color{ 255,0,255,255 },-44);
						break;
					}
				}

			}

			if (bSet)	break;
		}

	}
}