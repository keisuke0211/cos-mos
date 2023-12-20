//========================================
// 
// 描画状態の処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// 描画状態クラス
class _RNC_DrawState {
public:
	//========== [[[ 列挙型定義 ]]]]
	// カリングモード
	enum class CULLING_MODE {
		FRONT_SIDE,	// 表面
		BACK_SIDE,	// 裏面
		BOTH_SIDES,	// 両面
	};
	// アルファブレンドモード
	enum class ALPHA_BLEND_MODE {
		NORMAL,	// 通常
		ADD,	// 加算
	};
	// 補間モード
	enum class INTERPOLATION_MODE {
		NONE,
		LINEAR,
	};

	//========== [[[ 関数宣言 ]]]
	_RNC_DrawState           ();
	~_RNC_DrawState          ();
	void Init                (Device& device);
	void Uninit              (void);
	void Update              (void);
	void ResetVariableSetting(Device& device);
	void SetIsZTest          (Device& device, const bool& isZTest);
	void SetCullingMode      (Device& device, const CULLING_MODE& mode);
	void SetAlphaBlendMode   (Device& device, const ALPHA_BLEND_MODE& mode);
	void SetInterpolationMode(Device& device, const INTERPOLATION_MODE& mode);
	void SetIsTextureAlpha   (Device& device, const bool& isTextureAlpha);
	void SetIsLighting       (Device& device, const bool& isLighting);
	void SetIsFog            (Device& device, const bool& isFog);
	void SetFogParameter     (Device& device, const Color& col, const UShort& startDist, const UShort& endDist);
};