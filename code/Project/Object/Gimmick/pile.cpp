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

const int NUMTEX = 2;
const int RAND_TEX = rand() % NUMTEX;
//=======================================
// コンストラクタ
//=======================================
CPile::CPile()
{
	Manager::StageObjectMgr()->AddList(this);

	//初期状態
	m_type = TYPE::PILE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_TrunkModelID = NONEDATA;
	m_PilePos = INITVECTOR3D;	//本体座標
	m_NumTrunk = 0;			//幹の数（最低３個）
	m_fEvenTrunkCorrHeight = 0.0f;//幹の数が偶数だった場合の補正値（奇数なら0.0f
	m_TrunkHeight = 0.0f;	//幹座標
	m_TrunkHeightOld = 0.0f;//幹座標
	m_StartTrunkHeight = 0.0f;
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
	m_TrunkModelID = RNLib::Model().Load("data\\MODEL\\Trunk.x");

	m_nTex[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_000.png");
	m_nTex[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");

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
	if (RNLib::Input().GetKeyTrigger(DIK_L)) m_TrunkHeight = m_StartTrunkHeight;

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
	//幹座標
	Pos3D PilePos = m_PilePos;

	//一番上の幹から配置するため、配置する高さを計算
	{
		//幹数の半分（切り捨て）
		const int nNumHalf = m_NumTrunk / EVENPARITY;

		//本体サイズ  ＋  サイズ × 上の幹数  ＋  幹のめり込み量
		PilePos.y = SIZE_OF_1_SQUARE * -0.5f + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;

		RNLib::Text2D().PutDebugLog(CreateText(
			"杭Y位置:%.2f  めり込み:%.2f  高さ:%.2f  MaxY:%.2f  MinY:%.2f  補正値:%.2f",
			PilePos.y,	 m_TrunkHeight, m_height, m_pos.y + m_height, m_pos.y - m_height, m_fEvenTrunkCorrHeight));
	}

	for (int nCntPile = 0; nCntPile < m_NumTrunk; nCntPile++)
	{
		//幹モデル
		RNLib::Model().Put(PRIORITY_OBJECT, m_TrunkModelID, PilePos, INITD3DXVECTOR3);

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
	m_PilePos = pos ;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = TrunkHeight;
	m_StartTrunkHeight = TrunkHeight;
	m_fEvenTrunkCorrHeight = NumTrunk % EVENPARITY != 0 ? -SIZE_OF_1_SQUARE * 0.5f : 0.0f;

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
	//引数保存
	CFloat fCaveTemp = fCaveInHeight;

	//へこみ量反映
	fCaveInHeight += m_TrunkHeight;

	//杭の最大・最低高さを計算
	CInt nNumHalf = m_NumTrunk / EVENPARITY;
	CFloat fMaxHeight = SIZE_OF_1_SQUARE * nNumHalf - m_fEvenTrunkCorrHeight * 2.0f;
	CFloat fMinHeight = -SIZE_OF_1_SQUARE * nNumHalf;

	//杭が抜けないように調整
	if (fCaveInHeight < fMinHeight)		fCaveInHeight = fMinHeight;
	else if (fCaveInHeight > fMaxHeight)fCaveInHeight = fMaxHeight;

	//前回情報を保存
	SetOld(fCaveInHeight);

	//モデル配置
	PutModel();

	//パーティクル
	float world = 0.0f;
	float side = 0.785f;
	float PopPosY = SIZE_OF_1_SQUARE * 0.5f;

	if (fCaveTemp < 0.0f)
	{
		world = D3DX_PI;
		side *= -1.0f;
		PopPosY *= -1.0f;
	}

	CInt NumEffect = 12;
	for (int Cnt = 0; Cnt < NumEffect; Cnt++){

		const Pos3D TexPos = Pos3D(m_pos.x + (float)(rand() % (int)m_width - m_width * 0.5), PopPosY, m_pos.z);

		D3DXVECTOR3 rot = D3DXVECTOR3(0.0f, 0.0f, world + side);
		if (rand() % 2 != 0) rot = D3DXVECTOR3(0.0f, 0.0f, world - side);
		
		CFloat ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.6f) + INIT_EFFECT_SCALE.x * 0.6f);
		Manager::EffectMgr()->ParticleCreate(m_nTex[RAND_TEX], TexPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 300, rot,16,CDrawState::ALPHA_BLEND_MODE::NORMAL);
		Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(TexPos.x, TexPos.y + 1.0f * cosf(world), TexPos.z), rot, INITSCALE3D * 0.1f,INITCOLOR);
	}
}

//===============================
//前回情報を保存
//===============================
void CPile::SetOld(float fCaveInHeight)
{
	//めり込み量を反映させる
	m_TrunkHeightOld = m_TrunkHeight;
	m_TrunkHeight = fCaveInHeight;

	m_pos = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeight + m_fEvenTrunkCorrHeight, 0.0f);
	m_posOld = D3DXVECTOR3(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld + m_fEvenTrunkCorrHeight, 0.0f);
}

//===============================
//位置取得（めり込み量も含める）
//===============================
D3DXVECTOR3 CPile::GetPosCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;	//現在座標を格納
	ReturnPos.y += m_fEvenTrunkCorrHeight;//補正値を反映
	return m_pos;//座標を返す
}

//===============================
//前回位置取得（めり込み量も含める）
//===============================
D3DXVECTOR3 CPile::GetPosOldCaveIn(void)
{
	D3DXVECTOR3 ReturnPos = m_pos;	//現在座標を格納
	ReturnPos.y += m_TrunkHeightOld;//前回のめり込み量を反映
	ReturnPos.y += m_fEvenTrunkCorrHeight;//補正値を反映
	return ReturnPos;				//座標を返す
}

//===============================
//全体のめり込み量を初期化
//===============================
void CPile::ResetTrunkHeightAll(void)
{
	// オブジェクト1つ1つを見ていく
	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) {

		//ステージオブジェクトに変換して種類が合致
		CStageObject *stg = (CStageObject *)obj;
		if (stg->GetType() != CStageObject::TYPE::PILE) continue;

		//初期めり込み量に設定
		CPile *pPile = (CPile *)stg;
		pPile->m_TrunkHeight = pPile->m_TrunkHeightOld = pPile->m_StartTrunkHeight;
	}
}