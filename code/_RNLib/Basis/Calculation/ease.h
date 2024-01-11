//========================================
// 
// 補間処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// 列挙型定義
//****************************************
// 補間の種類
enum class EASE_TYPE { 
	LINEAR,
	IN_SINE,
	OUT_SINE,
	INOUT_SINE,
	MAX,
};

//****************************************
// クラス定義
//****************************************
// 補間クラス
class _RNC_Ease {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_Ease ();
	~_RNC_Ease();
	float Easing(const EASE_TYPE& ease, const int& base, const int& max);
	float Easing(const EASE_TYPE& ease, const float& base, const float& max);

private:
	//========== [[[ 友達宣言 ]]]
	friend class _RNC_Calculation;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};