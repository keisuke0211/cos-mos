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
		CInt nNumHalf = m_NumTrunk / EVENPARITY;
		CInt Under = (m_NumTrunk + 1) % EVENPARITY;

		//本体サイズ  ＋  サイズ × 上の幹数  ＋  幹のめり込み量
		PilePos.y = SIZE_OF_1_SQUARE * 0.5f * Under + SIZE_OF_1_SQUARE * nNumHalf + m_TrunkHeight;
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
//引数３  TrunkHeight：幹のめり込み係数（めり込みなし = 0.0f）
//===============================
void CPile::Set(Pos3D pos, int NumTrunk, float TrunkHeight)
{
	//幹数が最低数を下回っていたら修正
	if (NumTrunk < MIN_TRUNK) NumTrunk = MIN_TRUNK;

	//情報設定
	m_PilePos = pos;
	m_NumTrunk = NumTrunk;
	m_TrunkHeight = SIZE_OF_1_SQUARE * TrunkHeight;
	m_fEvenTrunkCorrHeight = NumTrunk % EVENPARITY == 0 ? SIZE_OF_1_SQUARE * 0.5f : 0.0f;

	//当たり判定の高さを再設定
	m_height = SIZE_OF_1_SQUARE * NumTrunk;

	//めり込み量調整
	CaveInTrunkHeight(0.0f);

	//初期めり込み量設定
	m_StartTrunkHeight = m_TrunkHeight;

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
	CFloat HalfSize = SIZE_OF_1_SQUARE * nNumHalf;
	CFloat CorrHeight = m_fEvenTrunkCorrHeight * 2.0f;

	//杭が抜けないように調整
	RNLib::Number().Clamp(&fCaveInHeight, -HalfSize - CorrHeight, HalfSize - CorrHeight);

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
		Manager::EffectMgr()->ParticleCreate(m_nTex[RAND_TEX], TexPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,255,155,30 }, CParticle::TYPE::TYPE_FLOATUP, 300,rot, D3DXVECTOR3(80.0f, 80.0f, 0.0f),false,false,_RNC_DrawState::ALPHA_BLEND_MODE::NORMAL);
		Manager::EffectMgr()->ModelEffectCreate(0, D3DXVECTOR3(TexPos.x, TexPos.y + 1.0f * cosf(world), TexPos.z), rot, INITSCALE3D * 0.1f,COLOR_WHITE);
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

	m_pos =    Pos3D(m_PilePos.x, m_PilePos.y + m_TrunkHeight + m_fEvenTrunkCorrHeight, 0.0f);
	m_posOld = Pos3D(m_PilePos.x, m_PilePos.y + m_TrunkHeightOld + m_fEvenTrunkCorrHeight, 0.0f);
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

//===============================
//当たり判定情報取得
//引数１　中心位置
//引数２　中心位置（前回
//引数３　高さ
//===============================
void CPile::GetCollisionInfo(Pos3D& CenterPos, Pos3D& CenterPosOld, float& height)
{
	height = m_height * 0.5f;

	const Pos2D Upper = Pos2D(m_pos.x, m_pos.y + height + m_fEvenTrunkCorrHeight);
	const Pos2D Lower = Pos2D(m_pos.x, m_pos.y - height + m_fEvenTrunkCorrHeight);

	CFloat center = m_pos.y + m_fEvenTrunkCorrHeight;

	CenterPos    = Pos3D(m_pos.x, m_pos.y + m_fEvenTrunkCorrHeight, 0.0f);
	CenterPosOld = Pos3D(m_posOld.x, m_posOld.y + m_fEvenTrunkCorrHeight, 0.0f);
}