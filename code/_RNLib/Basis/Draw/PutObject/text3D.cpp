//========================================
// 
// テキスト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include <locale.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| テキスト3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Text3D::_RNC_Text3D() {

}

//========================================
// デストラクタ
//========================================
_RNC_Text3D::~_RNC_Text3D() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Text3D::Init(void) {

}

//========================================
// 終了処理
//========================================
void _RNC_Text3D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Text3D::Update(void) {

}

//========================================
// 設置処理
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::Put(const UShort& priority, const char* string, const _RNC_Text::ALIGNMENT alignment, const short& fontIdx, const Matrix& mtx, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText3D(priority, mtx, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// 設置処理(位置と向き指定)
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::Put(const UShort& priority, const char* string, const _RNC_Text::ALIGNMENT alignment, const short& fontIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const bool& isOnScreen) {

	return Put(priority, string, alignment, fontIdx, RNLib::Matrix().ConvPosRotToMtx(pos, rot), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 登録情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Text3D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
_RNC_Text3D::CRegistInfo::~CRegistInfo() {

	// 文字列のメモリ解放
	RNLib::Memory().Release(&m_string);
}

//========================================
// パラメーターのクリア処理
//========================================
void _RNC_Text3D::CRegistInfo::ClearParameter(void) {

	RNLib::Memory().Release(&m_string);
	m_alignment         = _RNC_Text::ALIGNMENT::CENTER;
	m_fontIdx           = NONEDATA;
	m_mtx               = INITMATRIX;
	m_scaleOrSize       = INITVECTOR2D;
	m_isScale           = false;
	m_col               = COLOR_WHITE;
	m_isZtest           = true;
	m_isLighting        = false;
	m_isBillboard       = false;
	m_interpolationMode = _RNC_DrawState::INTERPOLATION_MODE::NONE;
}

//========================================
// 設置処理(ポリゴン3D)
//========================================
void _RNC_Text3D::CRegistInfo::PutPolygon3D(const UShort& priority, const bool& isOnScreen) {

	// フォントデータを取得
	_RNC_Text::FontData fontData = RNLib::Text().GetFontData(m_fontIdx);
	
	//----------------------------------------
	// 幅/高さ/間隔を算出
	//----------------------------------------
	float charWidth      = 0.0f;
	float charHeightHalf = 0.0f;
	if (m_isScale) {
		charWidth      = (RNLib::Texture().GetWidth (fontData.texIdx) / fontData.ptnX ) * m_scaleOrSize.x;
		charHeightHalf = (RNLib::Texture().GetHeight(fontData.texIdx) / fontData.ptnY) * m_scaleOrSize.y * 0.5f;
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
	const float topY      = charHeightHalf;
	const float bottomY   = -charHeightHalf;
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// カウントの文字が空白の時、折り返す
		if (wstr[cntChar] == ' ')
			continue;

		// [[[ 表示形式に応じた設定位置の設定 ]]]
		Pos2D setPos = INITPOS2D;
		switch (m_alignment) {
		case _RNC_Text::ALIGNMENT::CENTER: {
			setPos.x += (leftShift + cntChar) * charWidth;
		}break;
		case _RNC_Text::ALIGNMENT::LEFT: {
			setPos.x += cntChar * charWidth;
		}break;
		case _RNC_Text::ALIGNMENT::RIGHT: {
			setPos.x += (-strLen + cntChar) * charWidth;
		}break;
		}

		// ポリゴン3Dを設置
		const float rightX = setPos.x + charWidth;
		RNLib::DrawMgr().PutPolygon3D(priority, m_mtx, isOnScreen)
			->SetVtxPos(
				Pos3D(setPos.x, topY   , 0.0f),
				Pos3D(rightX  , topY   , 0.0f),
				Pos3D(setPos.x, bottomY, 0.0f),
				Pos3D(rightX  , bottomY, 0.0f))
			->SetCol(m_col)
			->SetTex(fontData.texIdx, (int)wstr[cntChar] - (int)fontData.startCode, fontData.ptnX, fontData.ptnY)
			->SetZTest(m_isZtest)
			->SetLighting(m_isLighting)
			->SetBillboard(m_isBillboard)
			->SetInterpolationMode(m_interpolationMode);
	}

	// wchar_t型文字列の解放
	free(wstr);
}

//========================================
// マトリックスを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// 文字列を設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	RNLib::Memory().AllocString(&m_string, string);

	return this;
}

//========================================
// 配置を設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetAlignment(const _RNC_Text::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// フォント番号を設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// 色を設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 拡大倍率を設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// 大きさを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}

//========================================
// Zテストを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;

	return this;
}

//========================================
// ライティングを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// ビルボードを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetBillboard(const bool& isBillboard) {

	if (this == NULL)
		return NULL;

	m_isBillboard = isBillboard;

	return this;
}

//========================================
// 補間モードを設定
//========================================
_RNC_Text3D::CRegistInfo* _RNC_Text3D::CRegistInfo::SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}