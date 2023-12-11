//========================================
// 
// 伸びる犬の処理
// Author:KOMURO HIROMU
// Arrange:HIRASAWA SHION
// 
//========================================
#include "extenddog.h"
#include "../../main.h"
#include "../../Character/player.h"

#define RADIUS_WIDTH	(0.5f)	//横半径
#define RADIUS_HEIGHT	(0.5f)	//縦半径
#define CORRECT_HEIGHT	(15.0f)	//高さ補正

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CTrampolineクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CExtenddog::CExtenddog(void) {
	Manager::StageObjectMgr()->AddList(this);

	//初期状態
	m_type = TYPE::EXTEND_DOG;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;
	m_state = STATE::NONE;
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
	m_bElasticity = false;
	m_bInversion = false;
	m_modelIdx[0] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Hip.x");
	m_modelIdx[1] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Head.x");
	m_modelIdx[2] = RNLib::Model().Load("data\\MODEL\\WallDog\\WallDog_Body.x");
	m_nCntShrink = 0;
	m_fcurrenty = 0.0f;
	m_HeadPos = INITD3DXVECTOR3;
	m_BodyPosOld = INITD3DXVECTOR3;
	m_BodyPos = INITD3DXVECTOR3;
	m_HipPos = INITD3DXVECTOR3;
}

//========================================
// デストラクタ
//========================================
CExtenddog::~CExtenddog(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Init(void) {
	m_HeadPos.y = m_nHeight * SIZE_OF_1_SQUARE;
	m_BodyPos = m_HeadPos;

	if (m_bInversion)
			m_rot.z = D3DX_PI;
}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CExtenddog::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU / HIRASAWA SHION
//========================================
void CExtenddog::Update(void) {

	//前回位置更新
	m_BodyPosOld = m_BodyPos;
	
	//CObject *obj = NULL;
	//while (Manager::StageObjectMgr()->ListLoop(&obj)) {
	//	break;
	//	//取得したオブジェクトをキャスト
	//	CStageObject* stageObj = (CStageObject*)obj;
	//
	//	if (stageObj->GetType() == CStageObject::TYPE::BLOCK)
	//	{// 想定された種類の時
	//
	//		D3DXVECTOR3 stagepos = stageObj->GetPos();
	//		D3DXVECTOR2 stagesize = D3DXVECTOR2(stageObj->GetWidth(), stageObj->GetHeight());
	//
	//		if (stagepos.x + (stagesize.x * 0.5f) >= m_HeadPos.x &&
	//			stagepos.x - (stagesize.x * 0.5f) <= m_HeadPos.x)
	//		{// 範囲内にいるとき
	//			if (m_pos.y <= m_HeadPos.y && m_bInversion == false)
	//			{
	//				if (m_pos.y <= stagepos.y)
	//					m_pos = stagepos;
	//			}
	//			if (m_pos.y >= m_HeadPos.y && m_bInversion == true)
	//			{
	//				if (m_pos.y >= stagepos.y)
	//					m_pos = stagepos;
	//			}
	//		}
	//	}
	//}

	//状態別更新処理
	switch (m_state)
	{
		case STATE::NONE:   UpdateState_None(); break;
		case STATE::RETURN: UpdateState_Return(); break;
	}

	// 割合計算 
	CFloat fCountRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);

	//y座標の更新
	m_fcurrenty = !m_bInversion ?
		(SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1)))) : 
		-(SIZE_OF_1_SQUARE * m_nHeight - (fCountRate * (SIZE_OF_1_SQUARE * (m_nHeight - 1))));

	const Rot3D Rot = !m_bInversion ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI);

	// 尻
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[0], m_HipPos, Rot, false)->SetOutLineIdx(true);

	// 頭
	m_HeadPos.y = m_pos.y + m_fcurrenty;
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[1], m_HeadPos, Rot, false)->SetOutLineIdx(true);

	// 体
	m_BodyPos.y = !m_bInversion ?
		(m_HeadPos.y + m_pos.y - SIZE_OF_1_SQUARE * 1.5f) * 0.5f :
		(m_HeadPos.y + m_pos.y + SIZE_OF_1_SQUARE * 1.5f) * 0.5f;

	m_scale = !m_bInversion ?
		Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight + (SIZE_OF_1_SQUARE * m_nHeight * (1.0f - fCountRate))) * 8, 1.0f) :
		Scale3D(1.0f, (-m_HeadPos.y * 0.5) + (SIZE_OF_1_SQUARE * m_nHeight - (SIZE_OF_1_SQUARE * (m_nHeight - 1.5) * fCountRate)) * 12, 1.0f);

	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[2], m_BodyPos, Rot, m_scale, false)->SetOutLineIdx(true);



	const Pos3D Slide = Pos3D(SIZE_OF_1_SQUARE, 0.0f, 0.0f);
	const Pos3D slidebody = Pos3D(20.0f, 0.0f, 0.0f);

	m_height = fabsf(m_HeadPos.y - m_HipPos.y) - SIZE_OF_1_SQUARE;

	// 尻
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HipPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE)
		->SetCol(Color{ 255,0,0,255 })
		->SetZTest(false);

	// 頭
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HeadPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE)
		->SetCol(Color{ 0,255,0,255 })
		->SetZTest(false);

	// 体
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_BodyPos + slidebody, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, m_height)
		->SetCol(Color{ 0,0,255,255 })
		->SetZTest(false);

#if 1
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(3.0f, 3.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HipPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HeadPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_BodyPos + slidebody, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, 1.0f)
		->SetCol(INITCOLOR)
		->SetZTest(false);

	if (!m_bInversion)
	{
		CFloat IDealMax = m_HeadPos.y + SIZE_OF_1_SQUARE * 0.5f;
		CFloat IDealMin = m_HipPos.y - SIZE_OF_1_SQUARE * 0.5f;
		RNLib::Text2D().PutDebugLog(CreateText("理想高Y:%.1f, 理想低Y:%.1f  理想高さ:%.1f  実際高さ%.1f",
									IDealMax, IDealMin, IDealMax - IDealMin, m_height));
	}
	else
	{
		CFloat IDealMax = m_HipPos.y + SIZE_OF_1_SQUARE * 0.5f;
		CFloat IDealMin = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.5f;
		RNLib::Text2D().PutDebugLog(CreateText("理想高Y:%.1f, 理想低Y:%.1f  理想高さ:%.1f  実際高さ%.1f",
									IDealMax, IDealMin, IDealMax - IDealMin, m_height));
	}
#endif
}

//========================================
// 状態【何もなし】更新処理
// Author:HIRASAWA SHION
//========================================
void CExtenddog::UpdateState_None(void)
{
	if (m_bElasticity == false)
	{
		// 縮むカウント
		m_nCntShrink--;
		if (m_nCntShrink <= 0)
			m_nCntShrink = 0;
	}
	else
	{
		// 縮むカウント
		m_nCntShrink++;
		if (m_nCntShrink >= MAX_COUNT)
			m_nCntShrink = MAX_COUNT;
	}
}

//========================================
// 状態【戻る】更新処理
// Author:HIRASAWA SHION
//========================================
void CExtenddog::UpdateState_Return(void)
{
	if (m_bElasticity == false)
	{
		// 縮むカウント
		m_nCntShrink++;
		if (m_nCntShrink >= MAX_COUNT){
			m_nCntShrink = MAX_COUNT;
			m_state = STATE::NONE;
		}
	}
	else
	{
		// 縮むカウント
		m_nCntShrink--;
		if (m_nCntShrink <= 0) {
			m_nCntShrink = 0;
			m_state = STATE::NONE;
		}
	}
}

//========================================
//当たり判定用の高さ取得
// Author:HIRASAWA SHION
//========================================
float CExtenddog::GetColliHeight(void)
{
	//最高・最低座標
	float MaxY = 0.0f;
	float MinY = 0.0f;

	//頭・お尻から最高・最低座標を算出（反転してたら頭・お尻が逆
	if (!m_bInversion)
	{
		MaxY = m_HeadPos.y + SIZE_OF_1_SQUARE * 0.5f;
		MinY = m_HipPos.y - SIZE_OF_1_SQUARE * 0.5f;
	}
	else
	{
		MaxY = m_HipPos.y + SIZE_OF_1_SQUARE * 0.5f;
		MinY = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.5f;
	}

	//最高と最低の半分を返す
	return (MaxY - MinY) * 0.5f;
}