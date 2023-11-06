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
//==========| テキスト3Dクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 設置処理
//========================================
CText3D::CRegistInfo* CText3D::Put(const D3DXMATRIX& mtx, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen) {

	// 登録受付中でない時、終了
	if (CDrawMng::GetProcessState() != CDrawMng::PROCESS_STATE::REGIST_ACCEPT)
		return NULL;

	return RNLib::DrawMng().PutText3D(mtx, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// 設置処理(位置と向き指定)
//========================================
CText3D::CRegistInfo* CText3D::Put(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen) {

	return Put(CMatrix::ConvPosRotToMtx(pos, rot), string, alignment, fontIdx, isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 登録情報クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CText3D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
CText3D::CRegistInfo::~CRegistInfo() {

	// 文字列のメモリ解放
	RNLib::Memory().Release(&m_string);
}

//========================================
// パラメーターのクリア処理
//========================================
void CText3D::CRegistInfo::ClearParameter(void) {

	RNLib::Memory().Release(&m_string);
	m_alignment   = CText::ALIGNMENT::CENTER;
	m_fontIdx     = NONEDATA;
	m_scaleX      = 1.0f;
	m_scaleY      = 1.0f;
	m_isFactScale = false;
	m_mtx         = INITD3DXMATRIX;
	m_col         = INITCOLOR;
	m_isZtest     = true;
	m_isLighting  = true;
	m_isBillboard = false;
	m_priority    = 0;
}

//========================================
// 設置処理(ポリゴン3D)
//========================================
void CText3D::CRegistInfo::PutPolygon3D(const bool& isOnScreen) {

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
		charWidth  = (RNLib::Texture().GetWidth(fontData.nTexIdx) * PIXEL3D_SIZE) / fontData.nPtnWidth;
		charHeight = (RNLib::Texture().GetHeight(fontData.nTexIdx) * PIXEL3D_SIZE) / fontData.nPtnHeight;
	}
	charSpace = charWidth * m_scaleX * fontData.fSpaceRate;

	//----------------------------------------
	// ロケールを設定してマルチバイト文字に対応
	//----------------------------------------
	setlocale(LC_ALL, "");

	//----------------------------------------
	// char型の文字列をwchar_t型の文字列に変換
	//----------------------------------------
	size_t   length = strlen(m_string);
	wchar_t* wstr = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, m_string, length + 1);

	//----------------------------------------
	// 一文字ずつ設置していく
	//----------------------------------------
	D3DXVECTOR3 cameraNorRToV = RNLib::Camera3D().GetNor();
	int         strLen        = (int)strlen(m_string);
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// カウント文字が空白の時、折り返す
		if (m_string[cntChar] == ' ') {
			continue;	
		}

		// [[[ 表示形式に応じた設定位置の設定 ]]]
		D3DXVECTOR3 setPos = INITD3DXVECTOR3;
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
		D3DXMATRIX resultMtx; {

			// 基準マトリックスとテキストマトリックスを求める
			D3DXMATRIX baseMtx = INITD3DXMATRIX;
			D3DXMATRIX textMtx = INITD3DXMATRIX;

			if (m_isBillboard) 
			{// ビルボードの時、
				D3DXVECTOR3 charPos = INITD3DXVECTOR3;
				D3DXVec3Cross(&charPos, &setPos, &cameraNorRToV);
				baseMtx = CMatrix::ConvPosRotToMtx(CMatrix::ConvMtxToPos(m_mtx), INITD3DXVECTOR3);
				textMtx = CMatrix::ConvPosRotToMtx(charPos, INITD3DXVECTOR3);
			}
			else 
			{// 通常の時、
				D3DXVECTOR3 charPos = setPos;
				baseMtx = m_mtx;
				textMtx = CMatrix::ConvPosRotToMtx(charPos, INITD3DXVECTOR3);
			}

			// 基準マトリックスとテキストマトリックスを掛け合わせる
			resultMtx = CMatrix::MultiplyMtx(baseMtx, textMtx);
		}

		// ポリゴン3Dを設置
		RNLib::DrawMng().PutPolygon3D(resultMtx, isOnScreen)
			->SetSize(charWidth, charHeight)
			->SetCol(m_col)
			->SetTex(fontData.nTexIdx, (int)wstr[cntChar] - (int)fontData.nStartCode, fontData.nPtnWidth, fontData.nPtnHeight)
			->SetZTest(m_isZtest)
			->SetLighting(m_isLighting)
			->SetBillboard(m_isBillboard)
			->SetPriority(m_priority);
	}

	// wchar_t型文字列の解放
	free(wstr);
}

//========================================
// マトリックスを設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetMtx(const D3DXMATRIX& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// 文字列を設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	StrCpyDynamicMemory(&m_string, string);

	return this;
}

//========================================
// 配置を設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetAlignment(const CText::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// フォント番号を設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// 色を設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 大きさを設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetSize(const float& width, const float& height) {

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
CText3D::CRegistInfo* CText3D::CRegistInfo::SetSize_TexBaseScale(const float& scaleX, const float& scaleY) {

	if (this == NULL)
		return NULL;

	m_scaleX = scaleX;
	m_scaleY = scaleY;
	m_isFactScale = false;

	return this;
}

//========================================
// Zテストを設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;

	return this;
}

//========================================
// ライティングを設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// ビルボードを設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetBillboard(const bool& isBillboard) {

	if (this == NULL)
		return NULL;

	m_isBillboard = isBillboard;

	return this;
}

//========================================
// 優先度設定
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetPriority(const short& priority) {

	if (this == NULL)
		return NULL;

	m_priority = priority;

	return this;
}