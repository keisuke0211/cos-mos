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
//==========| テキスト2Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Text2D::_RNC_Text2D() {

	m_isShowDebugLog        = true;
	m_isShowDebugLogOptions = false;
	m_debugLogLine          = 0;
	m_debugCount            = 0;
}

//========================================
// デストラクタ
//========================================
_RNC_Text2D::~_RNC_Text2D() {

}

//========================================
// 初期化処理
//========================================
void _RNC_Text2D::Init(void) {

	// 実行モードであればデバッグ表示を行わない
	m_isShowDebugLog = (RNSystem::GetMode() != RNSystem::MODE::EXECUTION);
}

//========================================
// 終了処理
//========================================
void _RNC_Text2D::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void _RNC_Text2D::Update(void) {

	m_debugLogLine = 0;

	// 実行モードでない時、
	if (RNSystem::GetMode() != RNSystem::MODE::EXECUTION){

		// デバッグ表示のON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_F1))
			m_isShowDebugLog = !m_isShowDebugLog;

		// 各デバッグ表示オプションのON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_F2))
			m_isShowDebugLogOptions = !m_isShowDebugLogOptions;

		// 各デバッグ表示オプション
		RNLib::Text2D().PutDebugLog("----------StanderdInformation");
		RNLib::Text2D().PutDebugLog("HideDebugLog      [F1]:TRUE");
		RNLib::Text2D().PutDebugLog(String("DefaultDebugOption[F2]:%s", m_isShowDebugLogOptions ? "TRUE" : "FALSE"));
		if (m_isShowDebugLogOptions) {
			RNLib::Text2D().PutDebugLog("----------Performance");
			RNLib::Text2D().PutDebugLog(String("FPS    :%d", RNSystem::GetFPS()));
			RNLib::Text2D().PutDebugLog(String("RunTime:%02d:%02d.%02d", RNLib::Count().GetCount() / 3600, (RNLib::Count().GetCount() / 60) % 60, RNLib::Count().GetCount() % 60));
			
			RNLib::Text2D().PutDebugLog("----------DrawState");
			RNLib::Text2D().PutDebugLog(String("Polygon2DNum:%d", RNLib::DrawMgr().GetPolygon2DNum()));
			RNLib::Text2D().PutDebugLog(String("Polygon3DNum:%d", RNLib::DrawMgr().GetPolygon3DNum()));
			RNLib::Text2D().PutDebugLog(String("ModelNum    :%d", RNLib::DrawMgr().GetModelNum()));
			
			RNLib::Text2D().PutDebugLog("----------ObjectNumInObjectMgr");
			CObjectMgr**& objectMgrs   = CObjectMgr::GetObjectMgrs  ();
			UShort&       objectMgrNum = CObjectMgr::GetObjectMgrNum();
			for (int cntObjectMgr = 0; cntObjectMgr < objectMgrNum; cntObjectMgr++)
				PutDebugLog(String("%d < %s", objectMgrs[cntObjectMgr]->GetNum(), objectMgrs[cntObjectMgr]->GetName()));

			RNLib::Text2D().PutDebugLog("----------Other");
			RNLib::Text2D().PutDebugLog(String("DebugCount:%d", m_debugCount));
		}
		RNLib::Text2D().PutDebugLog("----------");
		RNLib::Text2D().PutDebugLog("");
	}
}

//========================================
// 設置処理
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::Put(const UShort& priority, const char* string, const _RNC_Text::ALIGNMENT alignment, const short& fontIdx, const Pos2D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText2D(priority, pos, angle, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// デバッグログ設置処理
//========================================
Pos2D _RNC_Text2D::PutDebugLog(const char* string) {

	if (!m_isShowDebugLog)
		return INITPOS2D;

	setlocale(LC_ALL, "");
	size_t    length = strlen(string);
	wchar_t* wstr = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, string, length + 1);
	const int strLen = (int)wcslen(wstr);

	const float top = m_debugLogLine * 16.0f;
	const float bottom = (m_debugLogLine + 1) * 16.0f;
	const float right = strLen * 16.0f;
	free(wstr);

	RNLib::Polygon2D().Put(_RNC_DrawMgr::SCREEN_PRIORITY_DEBUG_LOG, true)
		->SetVtxPos(Pos2D(0.0f, top), Pos2D(right, top), Pos2D(0.0f, bottom), Pos2D(right, bottom))
		->SetVtxCol(Color(0, 0, 0, 100), Color(0, 0, 0, 100), Color(0, 0, 0, 100), Color(0, 0, 0, 100));

	// 設置位置
	Pos2D putPos = Pos2D(0.0f, 8.0f + m_debugLogLine * 16.0f);

	// 左上から下にかけてテキスト2Dを設置する
	Put(_RNC_DrawMgr::SCREEN_PRIORITY_DEBUG_LOG, string, _RNC_Text::ALIGNMENT::LEFT, NONEDATA, putPos, 0.0f, true)
		->SetSize(Size2D(16.0f, 16.0f));

	// デバッグログの行数加算
	m_debugLogLine++;

	return putPos;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| 登録情報クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
_RNC_Text2D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// デストラクタ
//========================================
_RNC_Text2D::CRegistInfo::~CRegistInfo() {

	// 文字列のメモリ解放
	RNLib::Memory().Release(&m_string);
}

//========================================
// パラメーターのクリア処理
//========================================
void _RNC_Text2D::CRegistInfo::ClearParameter(void) {

	RNLib::Memory().Release(&m_string);
	m_alignment         = _RNC_Text::ALIGNMENT::CENTER;
	m_fontIdx           = NONEDATA;
	m_pos               = INITPOS2D;
	m_angle             = 0.0f;
	m_scaleOrSize       = INITVECTOR2D;
	m_isScale           = false;
	m_col               = COLOR_WHITE;
	m_interpolationMode = _RNC_DrawState::INTERPOLATION_MODE::NONE;
}

//========================================
// 設置処理(ポリゴン2D)
//========================================
void _RNC_Text2D::CRegistInfo::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

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
			->SetTex(fontData.texIdx, (int)wstr[cntChar] - (int)fontData.startCode, fontData.ptnX, fontData.ptnY)
			->SetInterpolationMode(m_interpolationMode);
	}

	// wchar_t型文字列の解放
	free(wstr);
}

//========================================
// 位置を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetPos(const Pos2D& pos) {

	if (this == NULL)
		return NULL;

	m_pos = pos;

	return this;
}

//========================================
// 角度を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetAngle(const float& angle) {

	if (this == NULL)
		return NULL;

	m_angle = angle;

	return this;
}

//========================================
// 文字列を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	RNLib::Memory().AllocString(&m_string, string);

	return this;
}

//========================================
// 配置を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetAlignment(const _RNC_Text::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// フォント番号を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// 色を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// 拡大倍率を設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// 大きさを設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}

//========================================
// 補間モードを設定
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}