//========================================
// 
// ステージオブジェクトマネージャーの処理
// Author:KEISUKE OTONO
// Arrange:RIKU NISHIMURA
// 
//========================================
#include "../main.h"
#include "../UI/partsUI.h"

//========================================
// 静的変数
//========================================
CStageObjectMgr::BlockType *CStageObjectMgr::m_pBlockType = NULL;
const char* CStageObjectMgr::BLOCK_INFO_FILE = "data\\GAMEDATA\\BLOCK\\BLOCK_DATA.txt";

//========================================
// コンストラクタ
//========================================
CStageObjectMgr::CStageObjectMgr(void):CObjectMgr("StageObjectMgr")
{
	
}

//========================================
// デストラクタ
//========================================
CStageObjectMgr::~CStageObjectMgr()
{
	delete m_pBlockType;
	m_pBlockType = NULL;
}

//========================================
// 更新
//========================================
void CStageObjectMgr::Update(void)
{
	
}

//========================================
// 読み込み
//========================================
void CStageObjectMgr::Load(void)
{
	int nCntModel = 0;
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(BLOCK_INFO_FILE, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END"))
		{// 読み込みを終了
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}

		if (!strcmp(aDataSearch, "NUM_MODEL"))
		{
			int nMaxType = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMaxType);		// 最大数
			m_pBlockType = new BlockType[nMaxType];
			assert(m_pBlockType != NULL);
		}
		else if (!strcmp(aDataSearch, "MODEL"))
		{
			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%s", &m_pBlockType[nCntModel].aFileName[0]);	// ファイル名

			m_pBlockType[nCntModel].nModelIdx = RNLib::Model().Load(m_pBlockType[nCntModel].aFileName);	// モデル番号
			nCntModel++;
		}
	}
}

//========================================
// 各生成
//========================================

//========================================
// ブロック
//========================================
CBlock *CStageObjectMgr::BlockCreate(D3DXVECTOR3 pos,CBlock::LOOKS_TYPE type, Color color, bool isCollision)
{
	CBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CBlock;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->SetColor(color);
	pObj->SetCollision(isCollision);
	pObj->Init(type);

	return pObj;
}

//========================================
// ブロック(判定　無)
//========================================
CFillBlock *CStageObjectMgr::FillBlockCreate(D3DXVECTOR3 pos,CFillBlock::BLOCKTYPE type, Color color)
{
	//プレイヤーインスタンスの生成
	CFillBlock *pObj = new CFillBlock;

	//初期化処理
	pObj->Init(type);

	pObj->SetPos(pos);
	pObj->SetColor(color);

	//プレイヤーを返す
	return pObj;
}

//========================================
// 棘
//========================================
CSpike *CStageObjectMgr::SpikeCreate(D3DXVECTOR3 pos, int nType)
{
	CSpike *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CSpike;

	// 初期化処理
	pObj->Init();

	switch (nType)
	{
	case 0:
		if (pos.y <= 0) {
			pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
			pos.y += CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		}
		else {
			pos.y -= CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		}
		break;
	case 1:
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, -D3DX_PI/2));
		pos.x -= CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		break;
	case 2:
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI / 2));
		pos.x += CStageObject::SIZE_OF_1_SQUARE * 0.25f;
		break;
	}

	pObj->SetPos(pos);
	
	return pObj;
}

//========================================
// マグマブロック
//========================================
CMagmaBlock *CStageObjectMgr::MagmaBlockCreate(D3DXVECTOR3 pos, Color color)
{
	CMagmaBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMagmaBlock;

	// 初期化処理
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetColor(color);

	return pObj;
}

//========================================
// トランポリン
//========================================
CTrampoline *CStageObjectMgr::TrampolineCreate(D3DXVECTOR3 pos)
{
	CTrampoline *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CTrampoline;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

//========================================
// 隕石
//========================================
CMeteor *CStageObjectMgr::MeteorCreate(D3DXVECTOR3 pos,D3DXVECTOR3 move)
{
	CMeteor *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMeteor;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->SetMove(move);
	pObj->Init();


	return pObj;
}

//========================================
// 移動床
//========================================
CMoveBlock *CStageObjectMgr::MoveBlockCreate(D3DXVECTOR3 posV, D3DXVECTOR3 posL, D3DXVECTOR3 move, bool bxRetrurn)
{
	CMoveBlock *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMoveBlock;

	// 初期化処理
	pObj->SetPos(posV);
	pObj->SetMove(move);
	pObj->SetPosInfo(posV,posL);

	if (move.x <= 0)
		bxRetrurn = true;

	pObj->SetXReturn(bxRetrurn);

	pObj->Init();

	return pObj;
}

//========================================
// 杭
//========================================
CPile *CStageObjectMgr::PileCreate(D3DXVECTOR3 pos, int nNumPile, float fCaveIn)
{
	CPile *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CPile;

	// 初期化処理
	pObj->Init();
	pObj->Set(pos, nNumPile, fCaveIn);

	return pObj;
}

//========================================
// パーツ
//========================================
CParts *CStageObjectMgr::PartsCreate(D3DXVECTOR3 pos)
{
	CParts *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CParts;

	// 初期化処理
	pObj->Init();
	pObj->SetPos(pos);

	if (pos.y <= 0) {
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
	}

	return pObj;
}

//========================================
// コイン
//========================================
CCoin *CStageObjectMgr::CoinCreate(D3DXVECTOR3 pos)
{
	CCoin *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CCoin;

	// 初期化処理
	pObj->SetPos(pos);

	return pObj;
}

//========================================
// ロケット
//========================================
CRocket *CStageObjectMgr::RocketCreate(D3DXVECTOR3 pos)
{
	CRocket *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRocket;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

//========================================
// ゴールゲート
//========================================
CGoalGate *CStageObjectMgr::GoalGateCreate(D3DXVECTOR3 pos,bool bStartGate)
{
	CGoalGate *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CGoalGate;

	// 初期化処理
	pObj->Init();
	pObj->SetPos(pos);
	pObj->SetStartGate(bStartGate);

	return pObj;
}

//========================================
// 惑星
//========================================
CPlanet *CStageObjectMgr::PlanetCreate(D3DXVECTOR3 pos, D3DXVECTOR3 rot, CPlanet::STAR_TYPE type, Color col)
{
	CPlanet *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CPlanet;

	// 初期化処理
	pObj->SetType(type);
	pObj->SetPos(pos);
	pObj->SetCol(col);
	pObj->SetRot(rot);
	pObj->Init();
	

	return pObj;
}

//========================================
// 星
//========================================
CStar *CStageObjectMgr::StarCreate(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CStar *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CStar;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->SetRot(rot);
	pObj->Init();


	return pObj;
}

//========================================
// 往復レーザー
//========================================
CRoadTripLaser *CStageObjectMgr::RoadTripLaserCreate(D3DXVECTOR3 posV, D3DXVECTOR3 posL, D3DXVECTOR3 move,bool bXReturn)
{
	CRoadTripLaser *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CRoadTripLaser;

	// 初期化処理

	if (posV.y <= 0) {
		pObj->SetRot(D3DXVECTOR3(0.0f, 0.0f, D3DX_PI));
	}

	pObj->SetMove(move);
	pObj->SetPosInfo(posV,posL);

	if (move.x <= 0)
		bXReturn = true;

	pObj->SetXMoveReturn(bXReturn);

	pObj->Init();

	return pObj;
}

//========================================
//  動く犬
//========================================
CExtenddog	 *CStageObjectMgr::ExtenddogCreate(D3DXVECTOR3 fHeadpos, D3DXVECTOR3 fHippos,int fHeadheight, bool bShrink,bool bReturn)
{
	CExtenddog *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CExtenddog;

	// 初期化処理
	pObj->SetPos(fHeadpos);
	pObj->SetShrink(bShrink);
	pObj->SetHead(fHeadpos);
	pObj->SetHip(fHippos);
	pObj->SetHeadHeight(fHeadheight);
	pObj->SetReturn(bReturn);
	pObj->Init();


	return pObj;
}

//========================================
//  隕石ジェネレーター
//========================================
CMeteorGenerator *CStageObjectMgr::MeteorGeneratorCreate(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nSummonInterval)
{
	CMeteorGenerator *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CMeteorGenerator;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->SetMove(move);
	pObj->SetInterval(nSummonInterval);
	pObj->Init();

	return pObj;
}
//========================================
//  コウモリ
//========================================
CBat * CStageObjectMgr::BatCreate(D3DXVECTOR3 pos)
{
	CBat *pObj = NULL;

	if (pObj != NULL) { return pObj; }
	pObj = new CBat;

	// 初期化処理
	pObj->SetPos(pos);
	pObj->Init();

	return pObj;
}

