//========================================
// 
// 遷移の処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "transition.h"

//****************************************
// マクロ定義
//****************************************
#define BLACK_TIME     (40)
#define HOLE_SCALE_MAX (6.0f)
//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]遷移クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CTransition::CTransition() {

	m_type = TYPE::NUI;
	m_state = STATE::NONE;
	m_fMove = 0.0f;
	m_stateCounter = 0;
	m_time = 0;
	m_col = COLOR_WHITE;
	m_texIdxes[0] = 0;
	m_texIdxes[1] = 0;
	m_startGearAngle = 0.0f;
}

//========================================
// デストラクタ
//========================================
CTransition::~CTransition() {

}

//========================================
// 初期化処理
//========================================
void CTransition::Init(void) {
	m_texIdxes[0] = RNLib::Texture().Load("data\\TEXTURE\\Load.png");
	m_texIdxes[1] = RNLib::Texture().Load("data\\TEXTURE\\LoadBody.png");
	m_texIdxes[2] = RNLib::Texture().Load("data\\TEXTURE\\LoadingGear.png");
}

//========================================
// 終了処理
//========================================
void CTransition::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CTransition::Update(void) {

	float rate = 0.0f;	// 何もない 0 ~ 1 完全に埋まる

	switch (m_state) {
		// [[[ 無し ]]]
	case STATE::NONE: {

	}break;
		// [[[ オープン ]]]
	case STATE::OPEN: {
		rate = 1.0f - ((float)m_stateCounter / m_time);

		// 状態を無しに
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::NONE;
		}
	}break;
		// [[[ オープン待ち ]]]
	case STATE::OPEN_WAIT: {
		rate = 1.0f;
	}break;
		// [[[ ブラック ]]]
	case STATE::BLACK: {
		rate = 1.0f;

		// 状態をオープン待ちに
		if (++m_stateCounter >= BLACK_TIME) {
			m_stateCounter = 0;
			m_state = STATE::OPEN_WAIT;
			break;
		}

		float size = ((float)(m_stateCounter > 10 ? 10 : m_stateCounter) / 10) * 160.0f;
		if (m_stateCounter >= BLACK_TIME - 5) {
			int counter = m_stateCounter + 1;
			size = 1.0f - ((counter - BLACK_TIME) / 5.0f) * 160.0f;
		}

		RNLib::Polygon2D().Put(2, Pos2D(RNLib::Window().GetWidth() - 100.0f, RNLib::Window().GetHeight() - 100.0f), (((RNLib::Count().GetCount() % 60) / 60.0f) * D3DX_PI_DOUBLE) + m_startGearAngle, true)
			->SetTex(m_texIdxes[2])
			->SetSize(size, size);

	}break;
		// [[[ クローズ ]]]
	case STATE::CLOSE: {
		rate = ((float)m_stateCounter / m_time);

		// 状態をオープン待ちに
		if (++m_stateCounter >= m_time) {
			m_stateCounter = 0;
			m_state = STATE::BLACK;
		}
	}break;
	}

	// 割合が0であれば終了
	if (rate == 0.0f)
		return;

	// 画面埋め処理
	FillScreen(rate);
}

//========================================
// オープン処理
//========================================
bool CTransition::Open(const TYPE& type, const UShort& time) {

	// 状態オープン待ちで無い時、
	if (m_state != STATE::OPEN_WAIT)
		return false;

	m_stateCounter = 0;
	m_state = STATE::OPEN;
	m_time = time;
	m_startGearAngle = D3DX_PI * RNLib::Number().GetRandomFloat(1.0f);

	return true;
}

//========================================
// クローズ処理
//========================================
bool CTransition::Close(const TYPE& type, const Color& col, const UShort& time) {

	// 状態が無しで無い時、
	if (m_state != STATE::NONE)
		return false;

	m_stateCounter = 0;
	m_state = STATE::CLOSE;
	m_time = time;
	m_col = col;
	m_type = type;

	return true;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [非公開]遷移クラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// 画面埋め処理
//========================================
void CTransition::FillScreen(const float& rate) {

	const float screenWidth = RNLib::Window().GetWidth();
	const float screenHeight = RNLib::Window().GetHeight();
	const float screenHeightHalf = screenHeight * 0.5f;

	if (m_type == TYPE::FADE) {
		RNLib::Polygon2D().Put(1, true)
			->SetCol(Color(0, 0, 0, 255 * rate))
			->SetVtxPos(Pos2D(0.0f, 0.0f), Pos2D(screenWidth, 0.0f), Pos2D(0.0f, screenHeight), Pos2D(screenWidth, screenHeight));
	}
	else if (m_type == TYPE::NUI) {

		{// [[[ 上 ]]]
			float right1 = (screenWidth + screenHeightHalf) * rate;
			float right2 = right1 - screenHeightHalf;

			// 胴体
			RNLib::Polygon2D().Put(1, true)
				->SetTex(m_texIdxes[1], RNLib::Count().GetBlinkF2(), 1, 2)
				->SetCol(m_col)
				->SetTexMirrorX(true)
				->SetVtxPos(Pos2D(0.0f, 0.0f), Pos2D(right2, 0.0f), Pos2D(0.0f, screenHeightHalf), Pos2D(right2, screenHeightHalf));

			// 頭
			RNLib::Polygon2D().Put(1, true)
				->SetCol(m_col)
				->SetTex(m_texIdxes[0], 0, 1, 2)
				->SetTexMirrorX(true)
				->SetVtxPos(Pos2D(right2 - 0.1f, 0.0f), Pos2D(right1, 0.0f), Pos2D(right2 - 0.1f, screenHeightHalf), Pos2D(right1, screenHeightHalf));
		}

		{// [[[ 下 ]]]
			float left1 = screenWidth - ((screenWidth + screenHeightHalf) * rate);
			float left2 = left1 + screenHeightHalf;

			// 胴体
			RNLib::Polygon2D().Put(1, true)
				->SetCol(m_col)
				->SetTex(m_texIdxes[1], !RNLib::Count().GetBlinkF2(), 1, 2)
				->SetVtxPos(Pos2D(left2, screenHeightHalf), Pos2D(screenWidth, screenHeightHalf), Pos2D(left2, screenHeight), Pos2D(screenWidth, screenHeight));

			// 頭
			RNLib::Polygon2D().Put(1, true)
				->SetCol(m_col)
				->SetTex(m_texIdxes[0], 1, 1, 2)
				->SetVtxPos(Pos2D(left1, screenHeightHalf), Pos2D(left2 + 0.1f, screenHeightHalf), Pos2D(left1, screenHeight), Pos2D(left2 + 0.1f, screenHeight));
		}
	}
}