//========================================
// 
// 当たり判定の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// 当たり判定クラス
class _RNC_HitTest {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_HitTest                ();
	~_RNC_HitTest               ();
	bool XZ_OverlapPointToSquare(const Pos3D& basePos, const Pos3D& targetPos, const Scale2D& targetScale);

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_Calculation;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};