//========================================
// 
// ライト3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// クラス定義
//****************************************
// ライト3Dエディタークラス
class CLight3DEditor {
public:
	//========== [[[ 関数宣言 ]]]
	CLight3DEditor ();
	~CLight3DEditor();
	void Init      (void);
	void Uninit    (void);
	void Update    (void);

private:
	//========== [[[ 変数宣言 ]]]
	CCamera*  m_camera;
	CLight3D* m_light;
	bool      m_isLockSubLinearLight;
	UShort    m_selectLinearLightIdx;
};