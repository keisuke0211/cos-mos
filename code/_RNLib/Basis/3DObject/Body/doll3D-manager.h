//========================================
// 
// �h�[��3D�}�l�[�W���[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "doll3D.h"
#include "../../Mechanical/object-manager.h"
#include "../../Draw/camera.h"

//****************************************
// �N���X��`
//****************************************
// �h�[��3D�}�l�[�W���[�N���X
class CDoll3DMgr : public CObjectMgr {
public:
	//========== [[[ �֐��錾 ]]]
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
	//========== [[[ �\���̒�` ]]]
	struct EditInfo {
		CDoll3D* editDoll               = NULL;
		bool     isDrawModelVtxIdx      = false;
		UShort   drawModelVtxIdxBoneIdx = 0;
		UInt     drawModelVtxIdxNum     = 0;
		CCamera* editCamera             = NULL;
	};
	
	//========== [[[ �ϐ��錾 ]]]
	EditInfo m_editInfo;
};