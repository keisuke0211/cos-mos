//================================================================================================
// 
// 杭処理[pile.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"
#include "../../main.h"

//杭をめり込ませる最低高さ
const float CPile::CAVEIN_DIFF_HEIGHT = 35.0f;

//=======================================
// コンストラクタ
//=======================================
CPile::CPile()
{
	Manager::BlockMgr()->AddList(this);

	//初期状態
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_BodyModelID = NONEDATA;
	m_TrunkModelID = NONEDATA;
	m_PilePos = INITVECTOR3D;	//本体座標
	m_NumTrunk = 0;			//幹の数（最低３個）
	m_TrunkHeight = 0.0f;	//幹座標
	m_TrunkHeightOld = 0.0f;//幹座標
}

//=======================================
// デストラクタ
//=======================================
CPile::~CPile()
{

}

//=======================================
// 初期化処理
//=======================================
void CPile::Init(void)
{
	m_BodyModelID = RNLib::Model().Load("data\\MODEL\\Block.x");
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");

	//前回情報を保存
	SetOld(0.0f);
}

//=======================================
// 終了処理
//=======================================
void CPile::Uninit(void)
{

}

//=======================================
// 更新処理
//=======================================
void CPile::Update(void)
{
	//めり込みリセット
	if (RNLib::Input().GetKeyTrigger(DIK_L)) m_TrunkHeight = 0.0f;

	//前回情報を保存
	SetOld(m_TrunkHeight);

	//モデル配置
	PutModel();
}

//===============================
//モデル配置
//===============================
void CPile::PutModel(void)
{
	//本体モデル
	RNLib::Model().Put(m_PilePos, INITD3DXVECTOR3, m_BodyModelID, false);

	//幹座標
	Pos3D PilePos = m_PilePos;

	//一番上の幹から配置するため、配置する高さを計算
	{
		//幹数の半分（切り捨て）
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//本体サイズ  ＋  サイズ × 上の幹数  ＋  幹のめり込み量
		PilePos.y = SIZE_OF_1_SQUARE * -0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;

		RNLib::Text2D().PutDebugLog(CreateText("杭位置  Y:%f  めり込み:%f", PilePos.y, m_TrunkHeight));
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//幹モデル
		RNLib::Model().Put(PilePos, INITD3DXVECTOR3, m_TrunkModelID);

		//配置座標を下げる
		PilePos.y -= SIZE_OF_1_SQUARE;
	}
}

//===============================
//設定処理
//引数に必要高さ（最低3）や配置座標を指定する
//-------------------------------
//引数１  pos：配置座標
//引数２  NumTrunk：幹の数（最低３個）
//引数３  TrunkHeight：幹のめり込み座標（めり込みなし = 0.0f）
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//幹数が最低数を下回っていたら修正
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//情報設定
	m_PilePos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;

	//当たり判定の高さを再設定
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//モデル配置
	PutModel();
}

//===============================
//めり込み量反映処理
//===============================
void CPile::CaveInTrunkHeight(float fCaveInHeight)
{
	//へこみ量反映
	fCaveInHeight += m_TrunkHeight;

	//杭の最大・最低高さを計算
	const int nNumHalf = m_NumTrunk / EVENPARITY;
	const float fMaxHeight = SIZE_OF_1_SQUARE * nNumHalf;
	const float fMinHeight = -SIZE_OF_1_SQUARE * nNumHalf;

	//杭が抜けないように調整
	if (fCaveInHeight < fMinHeight)		fCaveInHeight = fMinHeight;
	else if (fCaveInHeight > fMaxHeight)fCaveInHeight = fMaxHeight;

	//前回情報を保存
	SetOld(fCaveInHeight);

	//モデル配置
	PutModel();
}

//===============================
//前回情報を保存
//===============================
void CPile::SetOld(float fCaveInHeight)
{
	//めり込み量を反映させる
	m_TrunkHeightOld = m_TrunkHeight;
	m_TrunkHeight = fCaveInHeight;

	m_pos = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeight, 0.0f);
	m_posOld = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld, 0.0f);
}

//===============================
//位置取得（めり込み量も含める）
//===============================
D3DXVECTOR3 CPile::GetPosCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_PilePos;//現在座標を格納
	ReturnPos.y += m_TrunkHeight; //めり込み量を反映
	return ReturnPos;			  //座標を返す
}

//===============================
//前回位置取得（めり込み量も含める）
//===============================
D3DXVECTOR3 CPile::GetPosOldCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_PilePos;	//現在座標を格納
	ReturnPos.y += m_TrunkHeightOld;//前回のめり込み量を反映
	return ReturnPos;				//座標を返す
}