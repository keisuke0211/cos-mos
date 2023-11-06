//========================================
// 
// 補間関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// 補間クラス
class CEase {
public:
	//========== [[[ 列挙型定義 ]]]
	// 種類
	enum class TYPE { LINEAR, IN_SINE, OUT_SINE, INOUT_SINE, MAX, };

	//========== [[[ 関数宣言 ]]]
	static float Easing(const TYPE& ease, const int& base, const int& max);
	static float Easing(const TYPE& ease, const float& base, const float& max);
};