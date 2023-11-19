//========================================
// 
// カメラマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/object-manager.h"

//****************************************
// クラス定義
//****************************************
// カメラマネージャークラス
class CCameraMgr : public CObjectMgr {
public:
	//========== [[[ 関数宣言 ]]]
	CCameraMgr();
	~CCameraMgr();
	void Update(void);
	bool CheckDeletedCamera(CCamera*& camera);
	void AddDeletedCamera(CCamera* camera);
	void ReleaseDeletedCamera(void);

private:
	//========== [[[ 変数宣言 ]]]
	CCamera** m_deletedCamera;
	UShort m_deletedCameraNum;
};