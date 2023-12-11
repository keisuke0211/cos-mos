//========================================
// 
// テキストの処理
// Author:RIKU NISHIMURA
// 
//========================================
// [[[ text.cpp ]]]
//========================================
#include "../../RNlib.h"

//****************************************
// マクロ定義
//****************************************
#define FONTDATA_PATH "RNData\\FontList.txt"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTextクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CText::CText() {
	m_pFont    = NULL;	// フォント情報のポインタ
	m_nFontNum = 0;		// フォント数
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CText::~CText() {
	
}

//========================================
// 初期化処理
//========================================
void CText::Init(void) {

	// フォントを読み込む
	LoadFont();
}

//========================================
// 終了処理
//========================================
void CText::Uninit(void) {

	// フォント情報のポインタのメモリ解放
	CMemory::Release(&m_pFont);
}

//========================================
// 更新処理
//========================================
void CText::Update(void) {

}

//========================================
// フォントの読み込み処理
// Author:RIKU NISHIMURA
//========================================
void CText::LoadFont(void) {
	if (!RNLib::File().OpenLoadFile(FONTDATA_PATH))
		return;

	// 読み込みループ
	while (RNLib::File().SearchLoop("END")) {
		if (false) {}
		else if (RNLib::File().CheckIdentifier(/* フォント設定情報 */"FONTSET{")) {
			while (RNLib::File().SearchLoop("}")) {
				if (false) {}
				else if (RNLib::File().CheckIdentifier(/* フォント情報 */"FONT{")) {
					int nNumOld_and_idx = m_nFontNum;
					m_nFontNum++;	// フォント数を加算

					{// フォント情報のポインタのメモリ再確保
						FontData* pNew = new FontData[m_nFontNum];
						int nCopySize = m_nFontNum < nNumOld_and_idx ? m_nFontNum : nNumOld_and_idx;
						if (nCopySize > 0) {
							memcpy(pNew, m_pFont, sizeof(FontData) * nCopySize);
							delete m_pFont;
						}
						m_pFont = pNew;
					}

					while (RNLib::File().SearchLoop("}")) {
						if (false) {}
						else if (RNLib::File().CheckIdentifier(/* テクスチャパス */"TEXTURE_PATH:")) { RNLib::File().Scan(CFile::SCAN::TEXIDX,&m_pFont[nNumOld_and_idx].nTexIdx); }
						else if (RNLib::File().CheckIdentifier(/* 開始文字       */"START_CODE:"))   { RNLib::File().Scan(CFile::SCAN::INT, &m_pFont[nNumOld_and_idx].nStartCode); }
						else if (RNLib::File().CheckIdentifier(/* パターン幅     */"PTN_WIDTH:"))    { RNLib::File().Scan(CFile::SCAN::INT, &m_pFont[nNumOld_and_idx].nPtnWidth); }
						else if (RNLib::File().CheckIdentifier(/* パターン高さ   */"PTN_HEIGHT:"))   { RNLib::File().Scan(CFile::SCAN::INT, &m_pFont[nNumOld_and_idx].nPtnHeight); }
						else if (RNLib::File().CheckIdentifier(/* 間隔比率       */"SPACE_RATE:"))   { RNLib::File().Scan(CFile::SCAN::FLOAT, &m_pFont[nNumOld_and_idx].fSpaceRate); }
					}
				}
			}
		}
	}

	// ファイルを閉じる
	RNLib::File().CloseFile();
}

//========================================
// フォントの幅取得
// Author:RIKU NISHIMURA
//========================================
float CText::GetFontWidth(int nFont) {
	return RNLib::Texture().GetWidth(m_pFont[nFont].nTexIdx) / m_pFont[nFont].nPtnWidth;
}

//========================================
// フォントの高さ取得
// Author:RIKU NISHIMURA
//========================================
float CText::GetFontHeight(int nFont) {
	return RNLib::Texture().GetHeight(m_pFont[nFont].nTexIdx) / m_pFont[nFont].nPtnHeight;
}