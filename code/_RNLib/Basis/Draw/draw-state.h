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
class CDrawState {
public:
	//========== [[[ 列挙型定義 ]]]]
	// Zテストモード
	enum class ZTEST_MODE {
		ENABLED,	// 有効
		DISABLED,	// 無効
	};
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
	// テクスチャ透過モード
	enum class TEXTURE_ALPHA_MODE {
		ENABLED,	// 有効
		DISABLED,	// 無効
	};
	// ライティングモード
	enum class LIGHTING_MODE {
		ENABLED,	// 有効
		DISABLED,	// 無効
	};
	// フォグモード
	enum class FOG_MODE {
		ENABLED,	// 有効
		DISABLED,	// 無効
	};

	//========== [[[ 関数宣言 ]]]
	CDrawState();
	~CDrawState();
	void Init                 (LPDIRECT3DDEVICE9& device);
	void Uninit               (void);
	void StartTemporarySetMode(void);
	void EndTemporarySetMode  (LPDIRECT3DDEVICE9& device);
	void SetZTestMode         (const ZTEST_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetZTestMode         (const bool& isZTest, LPDIRECT3DDEVICE9& device);
	void SetCullingMode       (const CULLING_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetAlphaBlendMode    (const ALPHA_BLEND_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetTextureAlphaMode  (const TEXTURE_ALPHA_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetTextureAlphaMode  (const bool& isTextureAlpha, LPDIRECT3DDEVICE9& device);
	void SetLightingMode      (const LIGHTING_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetLightingMode      (const bool& isLighting, LPDIRECT3DDEVICE9& device);
	void SetFogMode           (const FOG_MODE& mode, LPDIRECT3DDEVICE9& device);
	void SetFogMode           (const bool& isFog, LPDIRECT3DDEVICE9& device);
	void SetFogParameter      (const Color& col, const float& startDist, const float& endDist, LPDIRECT3DDEVICE9& device);

private:
	//========== [[[ 構造体定義 ]]]]
	struct FormerMode {
		ZTEST_MODE         ZTest        = ZTEST_MODE::ENABLED;
		CULLING_MODE       culling      = CULLING_MODE::FRONT_SIDE;
		ALPHA_BLEND_MODE   alphaBlend   = ALPHA_BLEND_MODE::NORMAL;
		TEXTURE_ALPHA_MODE textureAlpha = TEXTURE_ALPHA_MODE::ENABLED;
		LIGHTING_MODE      lighting     = LIGHTING_MODE::ENABLED;
		FOG_MODE           fog          = FOG_MODE::ENABLED;
	};

	//========== [[[ 変数宣言 ]]]]
	bool m_isAssignFormerMode;
	FormerMode m_formerMode;
};