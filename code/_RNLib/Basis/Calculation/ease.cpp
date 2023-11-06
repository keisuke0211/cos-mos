//========================================
// 
// 補間関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]補間クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [静的]補間処理(整数)
//========================================
float CEase::Easing(const TYPE& ease, const int& base, const int& max) {

	return Easing(ease, (float)base, (float)max);
}

//========================================
// [静的]補間処理(浮動小数)
//========================================
float CEase::Easing(const TYPE& type, const float& base, const float& max) {

	float rate     = 0.0f;
	float progress = base;

	// 基準値の制御
	if (progress > max)
		progress = max;
	else if (progress < 0.0f)
		progress = 0.0f;

	switch (type) {
		// [[[ 等速 ]]]
	case TYPE::LINEAR: {
		rate = progress / max;
	}break;
		// [[[ 徐々に加速 ]]]
	case TYPE::IN_SINE: {
		rate = progress / max;
		rate *= rate;
	}break;
		// [[[ 徐々に減速 ]]]
	case TYPE::OUT_SINE: {
		rate = progress / max;
		rate = -rate * (rate - 2.0f);
	}break;
		// [[[ 加速して減速 ]]]
	case TYPE::INOUT_SINE: {
		rate = progress / (max / 2.0f);

		if (rate < 1.0f) {
			rate = 0.5f * rate * rate;
		}
		else {
			rate = rate - 1.0f;
			rate = -0.5f * (rate * (rate - 2.0f) - 1.0f);
		}
	}break;
	}

	return rate;
}