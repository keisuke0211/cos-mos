//========================================
// 
// テキストの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"
#include "../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]テキストクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CText::CText() {

	m_defaultFontData = {};
	m_fontDatas       = NULL;
	m_fontNum         = 0;
}

//========================================
// デストラクタ
//========================================
CText::~CText() {
	
}

//========================================
// 初期化処理
//========================================
void CText::Init(void) {

	// デフォルトフォントデータを作成
	m_defaultFontData.texIdx    = RNLib::Texture().Load("RNData\\Texture\\Font.png");
	m_defaultFontData.startCode = 0;
	m_defaultFontData.ptnX      = 128;
	m_defaultFontData.ptnY      = 512;

	// フォントリストを読み込む
	RNLib::Text().LoadFont(RNSettings::GetInfo().fontListPath);
}

//========================================
// 終了処理
//========================================
void CText::Uninit(void) {

	// フォント情報のポインタのメモリ解放
	CMemory::Release(&m_fontDatas);
}

//========================================
// 更新処理
//========================================
void CText::Update(void) {

}

//========================================
// フォントの読み込み処理
//========================================
void CText::LoadFont(const char* loadPath) {

	if (!RNLib::File().OpenLoadFile(loadPath))
		return;

	// 読み込みループ
	while (RNLib::File().SearchLoop("END")) {
		if (RNLib::File().CheckIdentifier("fontDatas{")) {

			// フォント数を読み込み
			RNLib::File().Scan(CFile::SCAN::USHORT, &m_fontNum);
			CMemory::Alloc(&m_fontDatas, m_fontNum);

			UShort fontCount = 0;
			while (RNLib::File().SearchLoop("}")) {
				if (RNLib::File().CheckIdentifier("fontData{")) {

					// エラーメッセージ
					if (fontCount >= m_fontNum) {
						RNLib::Window().Message_ERROR(CreateText("フォントデータの数が指定数をオーバーしています。\n%s", loadPath));
						break;
					}

					while (RNLib::File().SearchLoop("}")) {
						RNLib::File().Scan(CFile::SCAN::TEXIDX,&m_fontDatas[fontCount].texIdx   , "texIdx");
						RNLib::File().Scan(CFile::SCAN::USHORT,&m_fontDatas[fontCount].startCode, "startCode");
						RNLib::File().Scan(CFile::SCAN::USHORT,&m_fontDatas[fontCount].ptnX     , "ptnX");
						RNLib::File().Scan(CFile::SCAN::USHORT,&m_fontDatas[fontCount].ptnY     , "ptnY");
					}

					if (++fontCount > m_fontNum)
						RNLib::Window().Message_ERROR("");
				}
			}

			// エラーメッセージ
			if (fontCount < m_fontNum) {
				RNLib::Window().Message_ERROR(CreateText("フォントデータの数が指定数に対して不足しています。\n%s", loadPath));
				break;
			}
		}
	}

	// ファイルを閉じる
	RNLib::File().CloseFile();
}

//========================================
// フォントの幅/高さ取得
//========================================
float CText::GetFontWidth(const short& fontIdx) {
	
	return RNLib::Texture().GetWidth(m_fontDatas[fontIdx].texIdx) / m_fontDatas[fontIdx].ptnX;
}
float CText::GetFontHeight(const short& fontIdx) {

	return RNLib::Texture().GetHeight(m_fontDatas[fontIdx].texIdx) / m_fontDatas[fontIdx].ptnY;
}