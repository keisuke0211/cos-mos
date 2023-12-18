//========================================
// 
// 補間処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// 補間クラス
class _RNC_Ease {
public:
	//========== [[[ 列挙型定義 ]]]
	// 種類
	enum class TYPE { LINEAR, IN_SINE, OUT_SINE, INOUT_SINE, MAX, };

	//========== [[[ 関数宣言 ]]]
	_RNC_Ease   ();
	~_RNC_Ease  ();
	void  Init  (void);
	void  Uninit(void);
	void  Update(void);
	float Easing(const TYPE& ease, const int& base, const int& max);
	float Easing(const TYPE& ease, const float& base, const float& max);
};