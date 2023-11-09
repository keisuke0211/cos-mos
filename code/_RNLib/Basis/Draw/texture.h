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
class CTexture : public CRegist {
public:
	//========== [[[ 関数宣言 ]]]
	CTexture();
	~CTexture();
	void Init(void);
	void Uninit(void);
	void Update(void);
	int Load(const char *loadPath);
	void Set(Device pDevice, int nTex);

	// 設定取得
	float GetWidth(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Width;
	}
	float GetWidth2D(int nTex) { return GetWidth(nTex) * PIXEL2D_SIZE; }
	float GetWidth3D(int nTex) { return GetWidth(nTex) * PIXEL3D_SIZE; }
	float GetHeight(int nTex) {
		if (nTex < 0)
			return 0.0f;
		D3DSURFACE_DESC desc;
		m_texs[nTex]->GetLevelDesc(0, &desc);
		return (float)desc.Height;
	}
	float GetHeight2D(int nTex) { return GetHeight(nTex) * PIXEL2D_SIZE; }
	float GetHeight3D(int nTex) { return GetHeight(nTex) * PIXEL3D_SIZE; }
	int GetCrntPtn(int nPtnX, int nPtnY, int nSpan);

private:
	//========== [[[ 変数宣言 ]]]
	Texture* m_texs;
};