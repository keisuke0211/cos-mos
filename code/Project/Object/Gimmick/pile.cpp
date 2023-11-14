//================================================================================================
// 
// 杭処理[pile.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "pile.h"

//=======================================
// コンストラクタ
//=======================================
CPile::CPile()
{
	m_BodyModelID = NONEDATA;
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

	//モデル配置
	SetModel();
}

//===============================
//モデル配置
//===============================
void CPile::SetModel(void)
{
	//本体モデル
	RNLib::Model().Put(m_pos, INITD3DXVECTOR3, m_BodyModelID, false);

	//幹座標
	Pos3D PilePos = m_pos;

	//一番上の幹から配置するため、配置する高さを計算
	{
		//幹数の半分（切り捨て）
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//本体サイズの半分  ＋  サイズ × 上の幹数  ＋  幹のめり込み量
		PilePos.y = SIZE_OF_1_SQUARE * 0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//幹モデル
		RNLib::Model().Put(PilePos, INITD3DXVECTOR3, m_TrunkModelID);

		//配置座標を下げる
		PilePos.y = SIZE_OF_1_SQUARE;
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
	m_pos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;

	//モデル配置
	SetModel();
}