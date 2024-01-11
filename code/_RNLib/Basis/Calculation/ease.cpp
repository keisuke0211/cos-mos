//========================================
// 
// 補間処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]補間クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Ease::_RNC_Ease() {

}

//========================================
// デストラクタ
//========================================
_RNC_Ease::~_RNC_Ease() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Ease::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Ease::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Ease::Update(void) {

}

//========================================
// 補間処理(整数)
//========================================
float _RNC_Ease::Easing(const EASE_TYPE& ease, const int& base, const int& max) {

	return Easing(ease, (float)base, (float)max);
}

//========================================
// 補間処理(浮動小数)
//========================================
float _RNC_Ease::Easing(const EASE_TYPE& type, const float& base, const float& max) {

	float rate = 0.0f;
	float progress = base;

	// 基準値の制御
	if (progress > max)
		progress = max;
	else if (progress < 0.0f)
		progress = 0.0f;

	switch (type) {
		// [[[ 等速 ]]]
	case EASE_TYPE::LINEAR: {
		rate = progress / max;
	}break;
		// [[[ 徐々に加速 ]]]
	case EASE_TYPE::IN_SINE: {
		rate = progress / max;
		rate *= rate;
	}break;
		// [[[ 徐々に減速 ]]]
	case EASE_TYPE::OUT_SINE: {
		rate = progress / max;
		rate = -rate * (rate - 2.0f);
	}break;
		// [[[ 加速して減速 ]]]
	case EASE_TYPE::INOUT_SINE: {
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