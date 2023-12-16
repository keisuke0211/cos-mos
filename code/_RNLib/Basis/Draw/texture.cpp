//========================================
// 
// テクスチャの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| テクスチャクラス
//----------|---------------------------------------------------------------------
//================================================================================

// 仮想関数を使用してLPDIRECT3DTEXTURE9を複製する関数
LPDIRECT3DTEXTURE9 CloneTexture(LPDIRECT3DTEXTURE9 sourceTexture, LPDIRECT3DDEVICE9 device)
{
	LPDIRECT3DTEXTURE9 clonedTexture = nullptr;

	// ソーステクスチャの情報を取得
	D3DSURFACE_DESC desc;
	sourceTexture->GetLevelDesc(0, &desc);

	// クローン用のテクスチャを作成
	HRESULT result = device->CreateTexture(desc.Width, desc.Height, 1, 0, desc.Format, D3DPOOL_MANAGED, &clonedTexture, nullptr);

	if (SUCCEEDED(result))
	{
		// ソーステクスチャのサーフェスを取得
		LPDIRECT3DSURFACE9 sourceSurface;
		sourceTexture->GetSurfaceLevel(0, &sourceSurface);

		// クローンテクスチャのサーフェスを取得
		LPDIRECT3DSURFACE9 destSurface;
		clonedTexture->GetSurfaceLevel(0, &destSurface);

		// ソースサーフェスからデータを取得
		RECT rect = { 0, 0, (LONG)desc.Width, (LONG)desc.Height };
		result = D3DXLoadSurfaceFromSurface(destSurface, nullptr, nullptr, sourceSurface, nullptr, &rect, D3DX_FILTER_NONE, 0);

		// リソースの解放
		sourceSurface->Release();
		destSurface->Release();
	}

	return clonedTexture;
}

//========================================
// [静的]テクスチャのデータをコピー
//========================================
void _RNC_Texture::CopyTextureData(LPDIRECT3DTEXTURE9& srcTexture, LPDIRECT3DTEXTURE9& destTexture) {

	// コピー先テクスチャを解放
	if (srcTexture != NULL) {
		srcTexture->Release();
		srcTexture = NULL;
	}

	// コピー元テクスチャが無ければ終了
	if (destTexture == NULL)
		return;

	srcTexture = CloneTexture(destTexture, RNLib::Window().GetD3DDevice());
}

//========================================
// コンストラクタ
//========================================
_RNC_Texture::_RNC_Texture() {

	m_texs = NULL;	// テクスチャへのポインタ
}

//========================================
// デストラクタ
//========================================
_RNC_Texture::~_RNC_Texture() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Texture::Init(void) {
	
}

//========================================
// 終了処理
//========================================
void _RNC_Texture::Uninit(void) {

	// テクスチャ解放
	if (m_texs != NULL) {
		for (int cntTex = 0; cntTex < m_num; cntTex++) {
			if (m_texs[cntTex] != NULL) {
				m_texs[cntTex]->Release();
				m_texs[cntTex] = NULL;
			}
		}
		delete m_texs;
		m_texs = NULL;
	}
}

//========================================
// 更新処理
//========================================
void _RNC_Texture::Update(void) {

}

//========================================
// 読み込み処理
//========================================
short _RNC_Texture::Load(const char* loadPath) {

	UShort numOld = m_num;
	short idx = NONEDATA;

	if (CRegist::Load(loadPath, idx))
	{// 読み込み成功
		// テクスチャのメモリ再確保
		RNLib::Memory().ReAlloc(&m_texs, numOld, m_num);
		m_texs[idx] = NULL;

		// テクスチャの読み込み
		Device device = RNLib::Window().GetD3DDevice();
		if (FAILED(D3DXCreateTextureFromFile(device, loadPath, &m_texs[idx])))
		{// 読み込み失敗
			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("テクスチャの読み込みに失敗しました。\n%s", loadPath));

			// テクスチャのメモリリセット
			RNLib::Memory().ReAlloc(&m_texs, m_num, numOld);

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
void _RNC_Texture::Set(Device pDevice, int nTex) {
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
int _RNC_Texture::GetCrntPtn(int nPtnX, int nPtnY, int nSpan) {
	return (RNLib::Count().GetCount() % ((nPtnX * nPtnY) * nSpan)) / nSpan;
}