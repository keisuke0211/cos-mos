//========================================
// 
// カメラマネージャーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]カメラマネージャークラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CCameraMgr::CCameraMgr() {

	m_deletedCamera = NULL;
	m_deletedCameraNum = 0;
}

//========================================
// デストラクタ
//========================================
CCameraMgr::~CCameraMgr() {

	CMemory::Release(&m_deletedCamera);
}

//========================================
// 更新処理
//========================================
void CCameraMgr::Update(void) {

}

//========================================
// 削除済みカメラかチェック
//========================================
bool CCameraMgr::CheckDeletedCamera(CCamera*& camera) {

	if (m_deletedCameraNum == 0)
		return false;

	for (int cntCamera = 0; cntCamera < m_deletedCameraNum; cntCamera++) {
		if (m_deletedCamera[cntCamera] == camera) {
			return true;
		}
	}

	return false;
}

//========================================
// 削除済みカメラとして追加
//========================================
void CCameraMgr::AddDeletedCamera(CCamera* camera) {

	const UShort oldNum = m_deletedCameraNum++;
	CMemory::ReAlloc(&m_deletedCamera, oldNum, m_deletedCameraNum);
	m_deletedCamera[oldNum] = camera;
}

//========================================
// 削除済みカメラを解放
//========================================
void CCameraMgr::ReleaseDeletedCamera(void) {

	CMemory::Release(&m_deletedCamera);
	m_deletedCameraNum = 0;
}