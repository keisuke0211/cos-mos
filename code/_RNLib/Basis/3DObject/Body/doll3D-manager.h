//========================================
// 
// ドール3Dマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "doll3D.h"
#include "../../Mechanical/object-manager.h"
#include "../../Draw/camera.h"

//****************************************
// クラス定義
//****************************************
// ドール3Dマネージャークラス
class CDoll3DMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CDoll3DMgr                                 ();
	~CDoll3DMgr                                ();
	void      Update                           (void);
	void      SetEditDoll                      (CDoll3D*& editDoll)                   { m_editInfo.editDoll = editDoll; }
	CDoll3D*& GetEditDoll                      (void)                                 { return m_editInfo.editDoll; }
	void      SetEditDollIsDrawModelVtxIdx     (const bool& isDrawModelVtxIdx)        { m_editInfo.isDrawModelVtxIdx = isDrawModelVtxIdx; }
	bool&     GetEditDollIsDrawModelVtxIdx     (void)                                 { return m_editInfo.isDrawModelVtxIdx; }
	void      SetEditDollDrawModelVtxIdxBoneIdx(const UShort& drawModelVtxIdxBoneIdx) { m_editInfo.drawModelVtxIdxBoneIdx = drawModelVtxIdxBoneIdx; }
	UShort&   GetEditDollDrawModelVtxIdxBoneIdx(void)                                 { return m_editInfo.drawModelVtxIdxBoneIdx; }
	void      SetEditDollDrawModelVtxIdxNum    (const short& drawModelVtxIdxNum)      { m_editInfo.drawModelVtxIdxNum = drawModelVtxIdxNum; }
	UInt&     GetEditDollDrawModelVtxIdxNum    (void)                                 { return m_editInfo.drawModelVtxIdxNum; }
	void      SetEditCamera                    (CCamera*& editCamera)                 { m_editInfo.editCamera = editCamera; }
	CCamera*& GetEditCamera                    (void)                                 { return m_editInfo.editCamera; }

public:
	//========== [[[ 構造体定義 ]]]
	struct EditInfo {
		CDoll3D* editDoll               = NULL;
		bool     isDrawModelVtxIdx      = false;
		UShort   drawModelVtxIdxBoneIdx = 0;
		UInt     drawModelVtxIdxNum     = 0;
		CCamera* editCamera             = NULL;
	};
	
	//========== [[[ 変数宣言 ]]]
	EditInfo m_editInfo;
};