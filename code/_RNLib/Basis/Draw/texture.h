//========================================
// 
// テクスチャの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../Mechanical/regist.h"
#include "../../RNmain.h"

//****************************************
// クラス定義
//****************************************
// テクスチャクラス
class _RNC_Texture : public CRegist {
public:
	//========== [[[ 関数宣言 ]]]
	static void CopyTextureData(LPDIRECT3DTEXTURE9& srcTexture, LPDIRECT3DTEXTURE9& destTexture);
	_RNC_Texture ();
	~_RNC_Texture();
	void  Init   (void);
	void  Uninit (void);
	void  Update (void);
	short Load   (const char *loadPath);
	void  Set    (Device pDevice, int nTex);

	// 設定取得
	Texture& GetTexture(int nTex) { return m_texs[nTex]; }
	float GetWidth(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Width;
	}
	float GetHeight(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Height;
	}
	int GetCrntPtn(int nPtnX, int nPtnY, int nSpan);

private:
	//========== [[[ 変数宣言 ]]]
	Texture* m_texs;
};