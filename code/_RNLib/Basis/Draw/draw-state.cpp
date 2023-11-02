//========================================
// 
// 描画状態の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 基底用描画情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CDrawState::CDrawState() {

	m_isAssignFormerMode = true;
	m_formerMode         = {};
}

//========================================
// デストラクタ
//========================================
CDrawState::~CDrawState() {

}

//========================================
// 初期化処理
//========================================
void CDrawState::Init(LPDIRECT3DDEVICE9& device) {

	//----------------------------------------
	// テクスチャステージステートの初期設定
	//----------------------------------------
	SetTextureAlphaMode(TEXTURE_ALPHA_MODE::ENABLED, device);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//----------------------------------------
	// Zテストの初期設定
	//----------------------------------------
	SetZTestMode(ZTEST_MODE::ENABLED, device);

	//----------------------------------------
	// カリングの初期設定
	//----------------------------------------
	SetCullingMode(CULLING_MODE::FRONT_SIDE, device);

	//----------------------------------------
	// アルファブレンドの初期設定
	//----------------------------------------
	SetAlphaBlendMode(ALPHA_BLEND_MODE::NORMAL, device);

	//----------------------------------------
	// ライティングの初期設定
	//----------------------------------------
	SetLightingMode(LIGHTING_MODE::ENABLED, device);

	//----------------------------------------
	// フォグの初期設定
	//----------------------------------------
	SetFogMode(FOG_MODE::DISABLED, device);
	SetFogParameter(INITCOLOR, 0.0f, 0.0f, device);

	//----------------------------------------
	// サンプラーステートの初期設定
	//----------------------------------------
	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
}

//========================================
// 終了処理
//========================================
void CDrawState::Uninit(void) {

}

//========================================
// 一時的なモード設定を開始
//========================================
void CDrawState::StartTemporarySetMode(void) {

	// 元のモード代入フラグを偽にする
	m_isAssignFormerMode = false;
}

//========================================
// 一時的なモード設定を終了
//========================================
void CDrawState::EndTemporarySetMode(LPDIRECT3DDEVICE9& device) {

	// 元のモードで設定し直す
	SetZTestMode(m_formerMode.ZTest, device);
	SetCullingMode(m_formerMode.culling, device);
	SetAlphaBlendMode(m_formerMode.alphaBlend, device);
	SetTextureAlphaMode(m_formerMode.textureAlpha, device);
	SetLightingMode(m_formerMode.lighting, device);

	// 元のモード代入フラグを真にする
	m_isAssignFormerMode = true;
}

//========================================
// Zテストモードの設定
//========================================
void CDrawState::SetZTestMode(const ZTEST_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.ZTest = mode;

	switch (mode) {
		// [[[ 有効 ]]]
	case ZTEST_MODE::ENABLED: {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}break;
		// [[[ 無効 ]]]
	case ZTEST_MODE::DISABLED: {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}break;
	}
}

//========================================
// Zテストモードの設定(フラグ指定)
//========================================
void CDrawState::SetZTestMode(const bool& isZTest, LPDIRECT3DDEVICE9& device) {

	SetZTestMode(isZTest ? ZTEST_MODE::ENABLED : ZTEST_MODE::DISABLED, device);
}

//========================================
// カリングモードの設定
//========================================
void CDrawState::SetCullingMode(const CULLING_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.culling = mode;

	switch (mode) {
		// [[[ 表面 ]]]
	case CULLING_MODE::FRONT_SIDE: {
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}break;
		// [[[ 裏面 ]]]
	case CULLING_MODE::BACK_SIDE: {
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	}break;
		// [[[ 両面 ]]]
	case CULLING_MODE::BOTH_SIDES: {
		device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}break;
	}
}

//========================================
// アルファブレンドモードの設定
//========================================
void CDrawState::SetAlphaBlendMode(const ALPHA_BLEND_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.alphaBlend = mode;

	switch (mode) {
		// [[[ 通常 ]]]
	case ALPHA_BLEND_MODE::NORMAL: {
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ALPHAREF, 255);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}break;
		// [[[ 加算 ]]]
	case ALPHA_BLEND_MODE::ADD: {
		device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		device->SetRenderState(D3DRS_ALPHAREF, 0);
		device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}break;
	}
}

//========================================
// テクスチャ透過モードの設定
//========================================
void CDrawState::SetTextureAlphaMode(const TEXTURE_ALPHA_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.textureAlpha = mode;

	switch (mode) {
		// [[[ 有効 ]]]
	case TEXTURE_ALPHA_MODE::ENABLED: {
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// テクスチャブレンドを乗算に設定 (※テクスチャの透明度がポリゴンの色をすかして背後の色を見せてしまう)
	}break;
		// [[[ 無効 ]]]
	case TEXTURE_ALPHA_MODE::DISABLED: {
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	}break;
	}
}

//========================================
// テクスチャ透過モードの設定(フラグ指定)
//========================================
void CDrawState::SetTextureAlphaMode(const bool& isTextureAlpha, LPDIRECT3DDEVICE9& device) {

	SetTextureAlphaMode(isTextureAlpha ? TEXTURE_ALPHA_MODE::ENABLED : TEXTURE_ALPHA_MODE::DISABLED, device);
}

//========================================
// ライティングモードの設定
//========================================
void CDrawState::SetLightingMode(const LIGHTING_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.lighting = mode;

	switch (mode) {
		// [[[ 有効 ]]]
	case LIGHTING_MODE::ENABLED: {
		device->SetRenderState(D3DRS_LIGHTING, TRUE);
	}break;
		// [[[ 無効 ]]]
	case LIGHTING_MODE::DISABLED: {
		device->SetRenderState(D3DRS_LIGHTING, FALSE);
	}break;
	}
}

//========================================
// ライティングモードの設定(フラグ指定)
//========================================
void CDrawState::SetLightingMode(const bool& isLighting, LPDIRECT3DDEVICE9& device) {

	SetLightingMode(isLighting ? LIGHTING_MODE::ENABLED : LIGHTING_MODE::DISABLED, device);
}

//========================================
// フォグモードの設定
//========================================
void CDrawState::SetFogMode(const FOG_MODE& mode, LPDIRECT3DDEVICE9& device) {

	// 元のモードとして代入
	if (m_isAssignFormerMode)
		m_formerMode.fog = mode;

	switch (mode) {
		// [[[ 有効 ]]]
	case FOG_MODE::ENABLED: {
		device->SetRenderState(D3DRS_FOGENABLE, TRUE);
	}break;
		// [[[ 無効 ]]]
	case FOG_MODE::DISABLED: {
		device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	}break;
	}
}

//========================================
// フォグモードの設定(フラグ指定)
//========================================
void CDrawState::SetFogMode(const bool& isFog, LPDIRECT3DDEVICE9& device) {

	SetFogMode(isFog ? FOG_MODE::ENABLED : FOG_MODE::DISABLED, device);
}

//========================================
// フォグモードの設定(フラグ指定)
//========================================
void CDrawState::SetFogParameter(const Color& col, const float& startDist, const float& endDist, LPDIRECT3DDEVICE9& device) {

	device->SetRenderState(D3DRS_FOGCOLOR, ColorToD3DCOLOR(col));
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&startDist));
	device->SetRenderState(D3DRS_FOGEND  , *(DWORD*)(&endDist));
}