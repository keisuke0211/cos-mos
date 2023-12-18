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

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CExtenddogクラス
//----------|---------------------------------------------------------------------
//================================================================================

//頭・お尻用サイズ
const float CExtenddog::HEAD_HIP_SIZE = CStageObject::SIZE_OF_1_SQUARE * 0.25f;

//モデルパス
const char *CExtenddog::MODEL_PATH[(int)Parts::Max] = 
{
	"data\\MODEL\\WallDog\\WallDog_Hip.x",  // お尻
	"data\\MODEL\\WallDog\\WallDog_Body.x", // 体
	"data\\MODEL\\WallDog\\WallDog_Head.x", // 頭
};
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
	m_nCntShrink = 0;
	m_nHeightMin = m_nHeightMax = 1;
	m_HeadPos = INITD3DXVECTOR3;
	m_BodyPos = INITD3DXVECTOR3;
	m_BodyPosOld = INITD3DXVECTOR3;
	m_HipPos = INITD3DXVECTOR3;

	m_StartBodyPos = INITD3DXVECTOR3;
	m_StartHeight = 0.0f;

	for (int nCntModel = 0; nCntModel < (int)Parts::Max; nCntModel++)
	{
		m_modelIdx[nCntModel] = RNLib::Model().Load(MODEL_PATH[nCntModel]);
	}
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

	CFloat Height = SIZE_OF_1_SQUARE * m_bElasticity ? 
		m_nHeightMin : m_nHeightMax;

	CFloat PosDiffY = Height + m_HipPos.y;
	
	//体の初期位置設定
	m_StartBodyPos = m_BodyPos = Pos3D(m_HeadPos.x, PosDiffY, m_HeadPos.z);// * 0.5f

	//高さ算出
	m_StartHeight = m_height = fabsf(Height - m_BodyPos.y) - HEAD_HIP_SIZE;

	if (!m_bInversion) return;

	m_rot.z = D3DX_PI;
	m_StartBodyPos.y *= -1.0f;
	m_BodyPos.y *= -1.0f;
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

	//状態別更新処理
	switch (m_state)
	{
		case STATE::NONE:   UpdateState_None(); break;
		case STATE::RETURN: UpdateState_Return(); break;
	}

	// 割合計算 
	CFloat fCountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_nCntShrink, MAX_COUNT);

	const Rot3D Rot = !m_bInversion ? INITROT3D : Rot3D(0.0f, 0.0f, D3DX_PI);

	//サイズ割合
	CFloat HeightRate = (m_nHeightMax - m_nHeightMin) * fCountRate;

	//高さ
	m_height = SIZE_OF_1_SQUARE *((float)m_nHeightMin + HeightRate);
	
	//半分の高さ
	CFloat HalfHeight = m_height * 0.5f;

	// 体位置
	m_BodyPos.y = !m_bInversion ?
		((m_HipPos.y + HEAD_HIP_SIZE) + HalfHeight) :
		((m_HipPos.y - HEAD_HIP_SIZE) - HalfHeight);

	//体スケール
	m_scale.y = m_height * 10.0f;

	//頭用　体のサイズと自身のサイズの合計
	CFloat HeadSize = HalfHeight + HEAD_HIP_SIZE;

	// 頭位置
	m_HeadPos.y = !m_bInversion ?
		m_BodyPos.y + HeadSize :
		m_BodyPos.y - HeadSize;

	// 体
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Body], m_BodyPos, Rot, m_scale, false)->SetOutLineIdx(true);

	// 頭
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Head], m_HeadPos, Rot, false)->SetOutLineIdx(true);

	// 尻
	RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdx[(int)Parts::Hip], m_HipPos, Rot, false)->SetOutLineIdx(true);

#if 0
	const Pos3D Slide = Pos3D(SIZE_OF_1_SQUARE, 0.0f, 0.0f);
	const Pos3D slidebody = Pos3D(20.0f, 0.0f, 0.0f);

	// 尻
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HipPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 0.5f)
		->SetCol(Color{ 255,0,0,255 })
		->SetZTest(false);

	// 頭
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_HeadPos + Slide, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, SIZE_OF_1_SQUARE * 0.5f)
		->SetCol(Color{ 0,255,0,255 })
		->SetZTest(false);

	// 体
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_BodyPos + slidebody, INITROT3D)
		->SetSize(SIZE_OF_1_SQUARE, m_height)
		->SetCol(Color{ 0,0,255,255 })
		->SetZTest(false);

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
		RNLib::Text2D().PutDebugLog(String("理想高Y:%.1f, 理想低Y:%.1f  理想高さ:%.1f  実際高さ%.1f",
									IDealMax, IDealMin, IDealMax - IDealMin, m_height));
	}
	else
	{
		CFloat IDealMax = m_HipPos.y + SIZE_OF_1_SQUARE * 0.5f;
		CFloat IDealMin = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.5f;
		RNLib::Text2D().PutDebugLog(String("理想高Y:%.1f, 理想低Y:%.1f  理想高さ:%.1f  実際高さ%.1f",
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
		MaxY = m_HeadPos.y + SIZE_OF_1_SQUARE * 0.25f;
		MinY = m_HipPos.y - SIZE_OF_1_SQUARE * 0.25f;
	}
	else
	{
		MaxY = m_HipPos.y + SIZE_OF_1_SQUARE * 0.25f;
		MinY = m_HeadPos.y - SIZE_OF_1_SQUARE * 0.25f;
	}

	//最高と最低の半分を返す
	return (MaxY - MinY) * 0.5f;
}

//========================================
//お尻の方向を取得
//========================================
int CExtenddog::GetHipRot(void)
{
	//反転していないなら下側、反転してるなら上側
	if (!m_bInversion) return (int)CCollision::ROT::UNDER;
	else return (int)CCollision::ROT::OVER;
}