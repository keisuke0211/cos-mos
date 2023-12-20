//========================================
// 
// 描画状態の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 基底用描画情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_DrawState::_RNC_DrawState() {

}

//========================================
// デストラクタ
//========================================
_RNC_DrawState::~_RNC_DrawState() {

}

//========================================
// 初期化処理
//========================================
void _RNC_DrawState::Init(Device& device) {

	// ~~~ 固定設定 ~~~

	//----------------------------------------
	// テクスチャステージステートの初期設定
	//----------------------------------------
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//----------------------------------------
	// サンプラーステートの初期設定
	//----------------------------------------
	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//----------------------------------------
	// フォグの初期設定
	//----------------------------------------
	SetIsFog(device, false);
	SetFogParameter(device, COLOR_WHITE, 0, 0);

	// ~~~ 可変設定 ~~~

	ResetVariableSetting(device);
}

//========================================
// 終了処理
//========================================
void _RNC_DrawState::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_DrawState::Update(void) {

}

//========================================
// 可変設定のリセット処理
//========================================
void _RNC_DrawState::ResetVariableSetting(Device& device) {

	//----------------------------------------
	// テクスチャ透過モードの設定
	//----------------------------------------
	SetIsTextureAlpha(device, true);

	//----------------------------------------
	// Zテストの初期設定
	//----------------------------------------
	SetIsZTest(device, true);

	//----------------------------------------
	// カリングモードの初期設定
	//----------------------------------------
	SetCullingMode(device, CULLING_MODE::FRONT_SIDE);

	//----------------------------------------
	// アルファブレンドモードの初期設定
	//----------------------------------------
	SetAlphaBlendMode(device, ALPHA_BLEND_MODE::NORMAL);

	//----------------------------------------
	// 補間モードの初期設定
	//----------------------------------------
	SetInterpolationMode(device, INTERPOLATION_MODE::NONE);

	//----------------------------------------
	// ライティングの初期設定
	//----------------------------------------
	device->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	SetIsLighting(device, true);
}

//========================================
// Zテストの設定
//========================================
void _RNC_DrawState::SetIsZTest(Device& device, const bool& isZTest) {

	if (isZTest) {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		device->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	else {
		device->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
}

//========================================
// カリングモードの設定
//========================================
void _RNC_DrawState::SetCullingMode(Device& device, const CULLING_MODE& mode) {

	switch (mode) {
	case CULLING_MODE::FRONT_SIDE:device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW ); break;
	case CULLING_MODE::BACK_SIDE :device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW  ); break;
	case CULLING_MODE::BOTH_SIDES:device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE); break;
	}
}

//========================================
// アルファブレンドモードの設定
//========================================
void _RNC_DrawState::SetAlphaBlendMode(Device& device, const ALPHA_BLEND_MODE& mode) {

	switch (mode) {
		// [[[ 通常 ]]]
	case ALPHA_BLEND_MODE::NORMAL: {
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
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
// 補間モードの設定
//========================================
void _RNC_DrawState::SetInterpolationMode(Device& device, const INTERPOLATION_MODE& mode) {

	switch (mode) {
		// [[[ 無し(ポイント) ]]]
	case INTERPOLATION_MODE::NONE: {
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	}break;
		// [[[ 線形 ]]]
	case INTERPOLATION_MODE::LINEAR: {
		device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	}break;
	}
}

//========================================
// テクスチャ透過モードの設定
//========================================
void _RNC_DrawState::SetIsTextureAlpha(Device& device, const bool& isTextureAlpha) {

	device->SetTextureStageState(0, D3DTSS_ALPHAOP, isTextureAlpha ? D3DTOP_MODULATE : D3DTOP_DISABLE);
}

//========================================
// ライティングモードの設定(フラグ指定)
//========================================
void _RNC_DrawState::SetIsLighting(Device& device, const bool& isLighting) {

	device->SetRenderState(D3DRS_LIGHTING, isLighting);
}

//========================================
// フォグモードの設定
//========================================
void _RNC_DrawState::SetIsFog(Device& device, const bool& isFog) {

	device->SetRenderState(D3DRS_FOGENABLE, isFog);
}

//========================================
// フォグモードの設定(フラグ指定)
//========================================
void _RNC_DrawState::SetFogParameter(Device& device, const Color& col, const UShort& startDist, const UShort& endDist) {

	const float start = startDist;
	const float end   = endDist;
	device->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_RGBA(col.r, col.g, col.b, col.a));
	device->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&start));
	device->SetRenderState(D3DRS_FOGEND  , *(DWORD*)(&end));
}