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
#include "Effect/eff-manager.h"
#include "System\StageEditor.h"
#include "System\words\font.h"
#include "UI/transition.h"

//****************************************
// 列挙型定義
//****************************************
typedef enum {
	PRIORITY_BACKGROUND_DEPTH,
	PRIORITY_BACKGROUND,
	PRIORITY_PLAYER,
	PRIORITY_OBJECT,
	PRIORITY_OBJECT2,
	PRIORITY_SWAP_CURTAIN,
	PRIORITY_EFFECT,
	PRIORITY_UI,
	PRIORITY_TEXT,
	PRIORITY_MAX,
}PRIORITY;

//****************************************
// 名前空間
//****************************************
namespace Manager {
	//========== [[[ 関数宣言 ]]]
	void             Init          (const CMode::TYPE mode);
	void             Uninit        (void);
	void             Update        (void);
	void             Draw          (void);
	void             SetMode       (const CMode::TYPE newMode);
	CMode::TYPE      GetMode       (void);
	void             Transition    (const CMode::TYPE newMode, const CTransition::TYPE transType);
	CCamera*         GetMainCamera (void);
	CCamera*         GetSubCamera  (void);
	CStageObjectMgr* StageObjectMgr(void);
	CObjectMgr*      BGMgr         (void);
	CEffMgr*         EffectMgr     (void);
	CStageEditor*    StgEd         (void);
	CFont*           Font          (void);
	CTransition&     Transition    (void);
	short&           GetLightIdx   (UShort lightIdx);
};