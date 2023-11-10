//========================================
// 
// マネージャー処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "Mode/mode.h"
#include "../_RNLib/RNlib.h"
#include "Object/stage-object-manager.h"
#include "Effect/effect-manager.h"
#include "System\StageEditor.h"
#include "System\words\font.h"

//****************************************
// 名前空間
//****************************************
namespace Manager {
	//========== [[[ 関数宣言 ]]]
	void        Init         (const CMode::TYPE mode);
	void        Uninit       (void);
	void        Update       (void);
	void        Load         (void);
	void        Save         (void);
	void        SetMode      (const CMode::TYPE newMode);
	CMode::TYPE GetMode      (void);
	void        Transition   (const CMode::TYPE newMode, const CTransition::TYPE transType);
	CCamera*    GetMainCamera(void);
	CStageObjectMgr*  BlockMgr   (void);
	CObjectMgr*  BGMgr(void);
	CEffectMgr*       EffectMgr(void);
	CStageEditor* StgEd(void);
	CFont* Font(void);
};