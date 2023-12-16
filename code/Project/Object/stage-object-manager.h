//========================================
// 
// �X�e�[�W�I�u�W�F�N�g�}�l�[�W���[�̏���
// Author:KEISUKE OTONO
// Arrange:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "Block/block.h"
#include "Block/fill-block.h"
#include "Block/magma-block.h"
#include "Gimmick/Spike.h"
#include "Block/move-block.h"
#include "Gimmick/trampoline.h"
#include "Gimmick/meteor.h"
#include "Gimmick/rocket.h"
#include "Item/Parts.h"
#include "Item/coin.h"
#include "BackGround/Planet.h"
#include "BackGround/Star.h"
#include "BackGround/Shiningwave.h"
#include "Gimmick/RoadTripLaser.h"
#include "Gimmick/extenddog.h"
#include "Gimmick/pile.h"
#include "Gimmick/goalgate.h"
#include "Gimmick/meteor_generator.h"
#include "BackGround/bat.h"
//****************************************
// �N���X
//****************************************
class CStageObjectMgr : public CObjectMgr {
public:
	//========== [[[ �ÓI�ϐ��錾 ]]]
	static const char* BLOCK_INFO_FILE;		// �u���b�N���̃t�@�C���p�X

	//========== [[[ �֐��錾 ]]]
	CStageObjectMgr();
	virtual~CStageObjectMgr();
	virtual void Update(void);	void Load(void);
	CBlock*     BlockCreate(D3DXVECTOR3 pos, CBlock::LOOKS_TYPE type, Color color = COLOR_WHITE, bool isCollision = true);
	CMagmaBlock *MagmaBlockCreate(D3DXVECTOR3 pos,Color color);
	CFillBlock  *FillBlockCreate(D3DXVECTOR3 pos,CFillBlock::BLOCKTYPE type, Color color);
	CSpike      *SpikeCreate	 (D3DXVECTOR3 pos, int nType);
	CTrampoline *TrampolineCreate(D3DXVECTOR3 pos);
	CMeteor     *MeteorCreate    (D3DXVECTOR3 pos, D3DXVECTOR3 move);
	CMoveBlock  *MoveBlockCreate (D3DXVECTOR3 posV, D3DXVECTOR3 posL, D3DXVECTOR3 move, bool bxRetrurn = false);
	CPile       *PileCreate(D3DXVECTOR3 pos, int nNumPile, float fCaveIn);
	CParts      *PartsCreate(D3DXVECTOR3 pos);
	CCoin       *CoinCreate	(D3DXVECTOR3 pos);
	CGoalGate   *GoalGateCreate	 (D3DXVECTOR3 pos,bool bStartGate);
	CRocket     *RocketCreate    (D3DXVECTOR3 pos);
	CPlanet		*PlanetCreate	 (D3DXVECTOR3 pos, D3DXVECTOR3 rot,CPlanet::STAR_TYPE type,Color col);
	CStar		*StarCreate		 (D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	CRoadTripLaser*RoadTripLaserCreate(D3DXVECTOR3 posV, D3DXVECTOR3 posL,D3DXVECTOR3 move,bool bxRetrurn = false);
	CExtenddog	  *ExtenddogCreate(D3DXVECTOR3 fHeadpos, D3DXVECTOR3 fHippos,int HeightMin, int HeightMax, bool bElasticity,bool bReturn);
	CMeteorGenerator *MeteorGeneratorCreate(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nSummonInterval);
	CBat * BatCreate(D3DXVECTOR3 pos);
private:
	// *** �\���� ***
	// ��ޏ��
	struct BlockType
	{
		char	aFileName[TXT_MAX];	// X�t�@�C���̃p�X
		int		nModelIdx;			// ���f���ԍ�
	};

	// *** �ÓI�ϐ� ***
	static int *m_nMaxType;

	// *** �ϐ� ***
	static BlockType *m_pBlockType;	// ��ޏ��
};