//========================================
// 
// テキスト2Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include <locale.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| テキスト2Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CText2D::CText2D() {

	m_debugLogLine = 0;
}

//========================================
// デストラクタ
//========================================
CText2D::~CText2D() {

}

//========================================
// 初期化処理
//========================================
void CText2D::Init(void) {

}

//========================================
// 終了処理
//========================================
void CText2D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CText2D::Update(void) {

	m_debugLogLine = 0;
}

//========================================
// 設置処理
//========================================
CText2D::CRegistInfo* CText2D::Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Pos2D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText2D(0, pos, angle, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// デバッグログ設置処理
//========================================
Pos2D CText2D::PutDebugLog(const char* string) {

	// 設置位置
	Pos2D putPos = Pos2D(0.0f, 8.0f + m_debugLogLine * 16.0f);

	// 左上から下にかけてテキスト2Dを設置する
	Put(0, string, CText::ALIGNMENT::LEFT, 0, putPos, 0.0f, true)
		->SetSize(Size2D(16.0f, 16.0f));

	// デバッグログの行数加算
	m_debugLogLine++;

	return putPos;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 登録情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CText2D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CText2D::CRegistInfo::~CRegistInfo() {

	// 文字列のメモリ解放
	CMemory::Release(&m_string);
}

//========================================
// パラメーターのクリア処理
//========================================
void CText2D::CRegistInfo::ClearParameter(void) {

	CMemory::Release(&m_string);
	m_alignment   = CText::ALIGNMENT::CENTER;
	m_fontIdx     = NONEDATA;
	m_pos         = INITPOS2D;
	m_angle       = 0.0f;
	m_scaleOrSize = INITVECTOR2D;
	m_isScale     = false;
	m_col         = INITCOLOR;
	m_isZtest     = true;
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
void CText2D::CRegistInfo::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// フォントデータを取得
	CText::FontData fontData = RNLib::Text().GetFont(m_fontIdx);
	
	//----------------------------------------
	// 幅/高さ/間隔を算出
	//----------------------------------------
	float charWidth      = 0.0f;
	float charHeightHalf = 0.0f;
	if (m_isScale) {
		charWidth      = (RNLib::Texture().GetWidth (fontData.nTexIdx) / fontData.nPtnWidth ) * m_scaleOrSize.x;
		charHeightHalf = (RNLib::Texture().GetHeight(fontData.nTexIdx) / fontData.nPtnHeight) * m_scaleOrSize.y * 0.5f;
	}
	else {
		charWidth      = m_scaleOrSize.x;
		charHeightHalf = m_scaleOrSize.y * 0.5f;
	}

	//----------------------------------------
	// ロケールを設定してマルチバイト文字に対応
	//----------------------------------------
	setlocale(LC_ALL, "");

	//----------------------------------------
	// char型の文字列をwchar_t型の文字列に変換
	//----------------------------------------
	size_t   length = strlen(m_string);
	wchar_t* wstr   = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, m_string, length + 1);

	//----------------------------------------
	// 一文字ずつ設置していく
	//----------------------------------------
	const int   strLen    = (int)wcslen(wstr);
	const float leftShift = strLen * -0.5f;
	const float topY      = m_pos.y - charHeightHalf;
	const float bottomY   = m_pos.y + charHeightHalf;
	const float rotX      = m_angle + D3DX_PI_HALF;
	const float rateXinX  = sinf(rotX);
	const float rateYinX  = cosf(rotX);
	const float rateXinY  = sinf(m_angle);
	const float rateYinY  = cosf(m_angle);
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// カウントの文字が空白の時、折り返す
		if (wstr[cntChar] == ' ') {
			continue;
		}

		// [[[ 表示形式に応じた設定位置の設定 ]]]
		Pos2D setPos = INITPOS2D;
		switch (m_alignment) {
		case CText::ALIGNMENT::CENTER: {
			setPos.x += (leftShift + cntChar) * charWidth;
		}break;
		case CText::ALIGNMENT::LEFT: {
			setPos.x += cntChar * charWidth;
		}break;
		case CText::ALIGNMENT::RIGHT: {
			setPos.x += (-strLen + cntChar) * charWidth;
		}break;
		}
		
		// ポリゴン2Dを設置
		const float leftX   = m_pos.x + setPos.x;
		const float rightX  = m_pos.x + setPos.x + charWidth;
		const float XLeft   = rateXinX * leftX;
		const float XRight  = rateXinX * rightX;
		const float XTop    = rateXinY * topY;
		const float XBottom = rateXinY * bottomY;
		const float YLeft   = rateYinX * leftX;
		const float YRight  = rateYinX * rightX;
		const float YTop    = rateYinY * topY;
		const float YBottom = rateYinY * bottomY;
		RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen)
			->SetVtxPos(
				Pos2D(XLeft  + XTop   , YTop    + YLeft ),
				Pos2D(XRight + XTop   , YTop    + YRight),
				Pos2D(XLeft  + XBottom, YBottom + YLeft ),
				Pos2D(XRight + XBottom, YBottom + YRight))
			->SetCol(m_col)
			->SetTex(fontData.nTexIdx, (int)wstr[cntChar] - (int)fontData.nStartCode, fontData.nPtnWidth, fontData.nPtnHeight)
			->SetZTest(m_isZtest);
	}

	// wchar_t型文字列の解放
	free(wstr);
}

//========================================
// 位置を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetPos(const Pos2D& pos) {

	if (this == NULL)
		return NULL;

	m_pos = pos;

	return this;
}

//========================================
// 角度を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetAngle(const float& angle) {

	if (this == NULL)
		return NULL;

	m_angle = angle;

	return this;
}

//========================================
// 文字列を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	StrCpyDynamicMemory(&m_string, string);

	return this;
}

//========================================
// 配置を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetAlignment(const CText::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// フォント番号を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// 色を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 拡大倍率を設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// 大きさを設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}