//========================================
// 
// テクスチャの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNLib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| テクスチャクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CTexture::CTexture() {
	m_texs = NULL;	// テクスチャへのポインタ
}

//========================================
// デストラクタ
//========================================
CTexture::~CTexture() {

	// テクスチャ解放
	if (m_texs != NULL) {
		for (int nCntTex = 0; nCntTex < m_num; nCntTex++) {
			if (m_texs[nCntTex] != NULL) {
				m_texs[nCntTex]->Release();
				m_texs[nCntTex] = NULL;
			}
		}
		delete m_texs;
		m_texs = NULL;
	}
}

//========================================
// 初期化処理
//========================================
void CTexture::Init(void) {
	
}

//========================================
// 読み込み処理
//========================================
int CTexture::Load(const char* loadPath) {

	unsigned short numOld = m_num;
	short idx = NONEDATA;

	if (CRegist::Load(loadPath, idx))
	{// 読み込み成功
		// テクスチャのメモリ再確保
		RNLib::Memory().ReAlloc<LPDIRECT3DTEXTURE9>(&m_texs, numOld, m_num);
		m_texs[idx] = NULL;

		// テクスチャの読み込み
		LPDIRECT3DDEVICE9 device = RNLib::Window().GetD3DDevice();
		if (FAILED(D3DXCreateTextureFromFile(device, loadPath, &m_texs[idx])))
		{// 読み込み失敗
			assert(false);

			// テクスチャのメモリリセット
			RNLib::Memory().ReAlloc<LPDIRECT3DTEXTURE9>(&m_texs, m_num, numOld);

			// 読み込み済パスのメモリリセット
			ReAllocLoadPath(numOld);

			// 数をリセット
			m_num = numOld;

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// 設定処理
// Author:RIKU NISHIMURA
//========================================
void CTexture::Set(LPDIRECT3DDEVICE9 pDevice, int nTex) {
	if (nTex < 0 || nTex >= m_num)
	{// テクスチャ番号が範囲外の時、
		// テクスチャの設定
		pDevice->SetTexture(0, NULL);
		return;	// 処理を終了する
	}

	// テクスチャの設定
	pDevice->SetTexture(0, m_texs[nTex]);
}

//========================================
// 今のパターン番号を取得
// Author:RIKU NISHIMURA
//========================================
int CTexture::GetCrntPtn(int nPtnX, int nPtnY, int nSpan) {
	return (RNLib::GetCount() % ((nPtnX * nPtnY) * nSpan)) / nSpan;
}