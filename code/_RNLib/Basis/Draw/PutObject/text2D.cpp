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
CText2D::CRegistInfo* CText2D::Put(const Pos3D& pos, const Angle& angle, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMgr::GetProcessState() != CDrawMgr::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMgr().PutText2D(pos, angle, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// デバッグログ設置処理
//========================================
void CText2D::PutDebugLog(const char* string) {

	// フォントデータを取得
	const CText::FontData fontData = RNLib::Text().GetFont(0);
	const float charWidth  = RNLib::Texture().GetWidth2D (fontData.nTexIdx) / fontData.nPtnWidth;
	const float charHeight = RNLib::Texture().GetHeight2D(fontData.nTexIdx) / fontData.nPtnHeight;

	// 左上から下にかけてテキスト2Dを設置する
	Put(Pos3D(charWidth * 0.5f, (charHeight * (0.5f + (float)m_debugLogLine)), 0.0f), 0.0f, string, CText::ALIGNMENT::LEFT, 0, true);

	// デバッグログの行数加算
	m_debugLogLine++;
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
	m_scaleX      = 1.0f;
	m_scaleY      = 1.0f;
	m_isFactScale = false;
	m_pos         = INITD3DXVECTOR3;
	m_angle       = 0.0f;
	m_col         = INITCOLOR;
	m_isZtest     = true;
	m_isLighting  = true;
	m_isBillboard = false;
	m_priority    = 0;
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
void CText2D::CRegistInfo::PutPolygon2D(const bool& isOnScreen) {

	// フォントデータを取得
	CText::FontData fontData = RNLib::Text().GetFont(m_fontIdx);
	
	//----------------------------------------
	// 幅/高さ/間隔を算出
	//----------------------------------------
	float charWidth  = 0.0f;
	float charHeight = 0.0f;
	float charSpace  = 0.0f;
	if (m_isFactScale) {
		charWidth  = m_scaleX;
		charHeight = m_scaleY;
	}
	else {
		charWidth  = (RNLib::Texture().GetWidth(fontData.nTexIdx) * PIXEL2D_SIZE) / fontData.nPtnWidth;
		charHeight = (RNLib::Texture().GetHeight(fontData.nTexIdx) * PIXEL2D_SIZE) / fontData.nPtnHeight;
	}
	charSpace = charWidth * m_scaleX * fontData.fSpaceRate;

	//----------------------------------------
	// ロケールを設定してマルチバイト文字に対応
	//----------------------------------------
	setlocale(LC_ALL, "");

	//----------------------------------------
	// char型の文字列をwchar_t型の文字列に変換
	//----------------------------------------
	size_t length(strlen(m_string));
	wchar_t* wstr((wchar_t*)malloc((length + 1) * sizeof(wchar_t)));
	mbstowcs(wstr, m_string, length + 1);

	//----------------------------------------
	// 一文字ずつ設置していく
	//----------------------------------------
	int strLen = (int)wcslen(wstr);
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// カウントの文字が空白の時、折り返す
		if (wstr[cntChar] == ' ')
			continue;	

		// [[[ 表示形式に応じた設定位置の設定 ]]]
		D3DXVECTOR2 setPos = INITD3DXVECTOR2;
		switch (m_alignment) {
		case CText::ALIGNMENT::CENTER: {
			setPos.x += ((strLen * -0.5f) + cntChar + 0.5f) * charSpace;
		}break;
		case CText::ALIGNMENT::LEFT: {
			setPos.x += cntChar * charSpace;
		}break;
		case CText::ALIGNMENT::RIGHT: {
			setPos.x += (-strLen + cntChar + 1) * charSpace;
		}break;
		}

		// [[[ 結果マトリックスの算出 ]]]
		Matrix resultMtx; {

			// 基準マトリックスとテキストマトリックスを求める
			Matrix baseMtx = INITMatrix;
			Matrix textMtx = INITMatrix;

			{// 通常の時、
				D3DXVECTOR3 charPos = D3DXVECTOR3(setPos.x, setPos.y, 0.0f);
				baseMtx = CMatrix::ConvPosRotToMtx(D3DXVECTOR3(m_pos.x,m_pos.y, 0.0f), D3DXVECTOR3(0.0f, 0.0f, m_angle));
				textMtx = CMatrix::ConvPosRotToMtx(charPos, INITD3DXVECTOR3);
			}

			// 基準マトリックスとテキストマトリックスを掛け合わせる
			resultMtx = CMatrix::MultiplyMtx(baseMtx, textMtx);
		}

		// ポリゴン2Dを設置
		RNLib::DrawMgr().PutPolygon2D(CMatrix::ConvMtxToPos(resultMtx), CMatrix::ConvMtxToRot(resultMtx).z, isOnScreen)
			->SetSize(charWidth, charHeight)
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
CText2D::CRegistInfo* CText2D::CRegistInfo::SetPos(const Pos3D& pos) {

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
// 大きさを設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetSize(const float& width, const float& height) {

	if (this == NULL)
		return NULL;

	m_scaleX = width;
	m_scaleY = height;
	m_isFactScale = true;

	return this;
}

//========================================
// 大きさを設定(テクスチャ基準で拡大)
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetSize_TexBaseScale(const float& scaleX, const float& scaleY) {

	if (this == NULL)
		return NULL;

	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_isFactScale = false;

	return this;
}

//========================================
// 優先度設定
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetPriority(const short& priority) {

	if (this == NULL)
		return NULL;

	m_priority = priority;

	return this;
}