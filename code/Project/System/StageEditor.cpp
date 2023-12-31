//========================================
// 
// ステージエディットの読み込み
// Author:KEISUKE OTONO
// 
//========================================
#include "../manager.h"
#include "csv_file.h"
#include "../Object/stage-object.h"
#include "../Mode/mode_game.h"
#include "../Character/player.h"
#include "StageEditor.h"
#include "../Mode/mode_title.h"
#include "../UI/partsUI.h"
#include "../stage.h"

//========================================
// 静的変数
//========================================
CStageEditor::PlanetType *CStageEditor::m_PlanetType = NULL;
CStageEditor::StageColor CStageEditor::m_StageColor;
const char* CStageEditor::STAGE_INFO_FILE = "data\\GAMEDATA\\STAGE\\STAGE_FILE.txt";

//========================================
// コンストラクタ
//========================================
CStageEditor::CStageEditor(void)
{
	if (m_PlanetType != NULL){
		Uninit();
	}
	else{
		m_PlanetType = NULL;
	}

	{
		m_StageColor.Set = COLOR_WHITE;
		m_StageColor.Player1 = COLOR_WHITE;
		m_StageColor.Player2 = COLOR_WHITE;
		m_StageColor.Block = COLOR_WHITE;
		m_StageColor.FillBlock = COLOR_WHITE;
	}

	m_Info.nRow = 0;
	m_Info.nLine = 0;
	m_Info.nRowMax = 0;
	m_Info.nLineMax = 0;
	m_Info.CameraPos = INITD3DXVECTOR3;
	m_Info.nPlanetIdx = 0;
	m_Info.aBgFile = NULL;
	m_Info.aSoundFile = NULL;

	// 最大値
	m_Info.nPlanetMax = 0;
	m_Info.nLiftMax = 0;
	m_Info.nMateorMax = 0;
	m_Info.nLaserMax = 0;
	m_Info.nDogMax = 0;
	m_Info.nPileMax = 0;

	m_LiftInfo = NULL;
	m_MeteorInfo = NULL;
	m_LaserInfo = NULL;
	m_DogInfo = NULL;
	m_PileInfo = NULL;

	bRocket = false;
}

//========================================
// デストラクタ
//========================================
CStageEditor::~CStageEditor()
{
	//メモリ開放
	Release(true);
}

//========================================
// 終了処理
// Author:KEISUKE OTONO
//========================================
void CStageEditor::Uninit(void)
{
	//メモリ開放
	Release();
}

//========================================
// メモリ開放
// 惑星情報まで開放するかどうか（trueで開放
// Author:KEISUKE OTONO
// Arrange：HIRASAWA SHION
//========================================
void CStageEditor::Release(bool bPlanetRelease)
{
	//惑星情報の開放処理
	if (bPlanetRelease &&
		m_PlanetType != NULL)
	{
		for (int nPlanet = 0; nPlanet < m_Info.nPlanetMax; nPlanet++)
		{
			if (m_PlanetType[nPlanet].StageType != NULL)
			{
				delete[] m_PlanetType[nPlanet].StageType;
				m_PlanetType[nPlanet].StageType = NULL;
			}
		}

		delete[] m_PlanetType;
		m_PlanetType = NULL;
	}

	if (m_LiftInfo != NULL) {
		delete[] m_LiftInfo;
		m_LiftInfo = NULL;
	}

	if (m_MeteorInfo != NULL) {
		delete[] m_MeteorInfo;
		m_MeteorInfo = NULL;
	}

	if (m_LaserInfo != NULL) {
		delete[] m_LaserInfo;
		m_LaserInfo = NULL;
	}

	if (m_DogInfo != NULL) {
		delete[] m_DogInfo;
		m_DogInfo = NULL;
	}

	if (m_PileInfo != NULL) {
		delete[] m_PileInfo;
		m_PileInfo = NULL;
	}

	if (m_Info.aBgFile != NULL) {
		delete[] m_Info.aBgFile;
		m_Info.aBgFile = NULL;
	}

	if (m_Info.aSoundFile != NULL) {
		delete[] m_Info.aSoundFile;
		m_Info.aSoundFile = NULL;
	}
}

//========================================
// ファイル読み込み
// Author:KEISUKE OTONO
//========================================
void CStageEditor::FileLoad(void)
{

	int nCntPlanet = 0;
	int *pCntStage = NULL;
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(STAGE_INFO_FILE, "r");

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

			if (pCntStage != NULL)
			{
				delete[] pCntStage;
				pCntStage = NULL;
			}

			break;
		}
		if (aDataSearch[0] == '#')
		{// 折り返す
			continue;
		}

		if (!strcmp(aDataSearch, "NUM_PLANET"))
		{
			int nMax = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMax);		// 最大数

			if (nMax <= 0)
			{
				nMax = 0;
			}

			m_Info.nPlanetMax = nMax;
			m_PlanetType = new PlanetType[nMax];
			assert(m_PlanetType != NULL);

			pCntStage = new int[nMax];

			for (int nCnt = 0; nCnt < nMax; nCnt++)
			{
				pCntStage[nCnt] = 0;
			}

		}
		else if (!strcmp(aDataSearch, "SetPlanet"))
		{
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索

				if (!strcmp(aDataSearch, "EndPlanet"))
				{
					nCntPlanet++;
					break;
				}
				else if (!strcmp(aDataSearch, "NumStage")) {
					int StageMax = 0;
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &StageMax);	// ステージ数

					m_PlanetType[nCntPlanet].nStageMax = StageMax;
					m_PlanetType[nCntPlanet].StageType = new StageType[StageMax];
					assert(m_PlanetType[nCntPlanet].StageType != NULL);
				}
				else if (!strcmp(aDataSearch, "ModelPath")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_PlanetType[nCntPlanet].aTexFile[0]);	// テクスチャパス
				}
				else if (!strcmp(aDataSearch, "BackColor")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					int r = 0; int g = 0; int b = 0; int a = 0;
					fscanf(pFile, "%d", &r);
					fscanf(pFile, "%d", &g);
					fscanf(pFile, "%d", &b);
					fscanf(pFile, "%d", &a);

					m_PlanetType[nCntPlanet].color = Color{r,g,b,a};
				}
			}
		}
		else if (!strcmp(aDataSearch, "STAGE"))
		{
			int nPlanet = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nPlanet);	// 惑星番号

			if (pCntStage[nPlanet] < m_PlanetType[nPlanet].nStageMax)
			{
				fscanf(pFile, "%s", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].aFileName[0]);	// ファイル名
				fscanf(pFile, "%d", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].nNumCoin);		// コイン数
				fscanf(pFile, "%d", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].nLimitTime);		// 制限時間
				//fscanf(pFile, "%s", &m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].aName[0]);		// ステージ名

				if (nPlanet == 0 && pCntStage[nPlanet] == 0)
					m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].bRelease = false;
				else
					m_PlanetType[nPlanet].StageType[pCntStage[nPlanet]].bRelease = true;

				pCntStage[nPlanet]++;
			}
		}
	}
}

//========================================
// ステージ読み込み
// Author:KEISUKE OTONO
//========================================
void CStageEditor::StageLoad(int planet, int stage)
{
	// エフェクト解放
	Manager::EffectMgr()->DeleteAll();

	//メモリ開放
	Release();

	CSVFILE *pFile = new CSVFILE;

	m_Info.nPlanetIdx = planet;
	m_PlanetType[planet].nStageIdx = stage;
	bool bSet = true;
	bool bEnd = false;

	RNLib::Number().Clamp(&m_PlanetType[planet].nStageIdx, 0, m_PlanetType[planet].nStageIdx);

	// 読み込み
	pFile->FileLood(m_PlanetType[planet].StageType[stage].aFileName, false, false, ',');

	// 行数の取得
	int nRowMax = pFile->GetRowSize();

	// 各データに代入
	for (int nRow = 0; nRow < nRowMax; nRow++)
	{
		// 配置情報の生成
		int nType = -1;				// 種類

		// 列数の取得
		int nLineMax = pFile->GetLineSize(nRow);

		for (int nLine = 0; nLine < nLineMax; nLine++)
		{
			if (bEnd) { break; }
			char *aDataSearch = NULL;	// データ検索用
			string sData = pFile->GetData(nRow, nLine);
			char* cstr = new char[sData.size() + 1]; // メモリ確保
			std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
			aDataSearch = cstr;
			

			if (!strcmp(aDataSearch, "End")) { bEnd = true;}
			else if (!strcmp(aDataSearch, "SetBg")) {
				nLine += 4;

				if (m_Info.aBgFile == NULL){
					string sData = pFile->GetData(nRow, nLine);
					m_Info.aBgFile = new char[sData.size() + 1]; // メモリ確保
					std::char_traits<char>::copy(m_Info.aBgFile, sData.c_str(), sData.size() + 1);
				}

			}
			else if (!strcmp(aDataSearch, "SetSound")) {
				nLine += 4;

				if (m_Info.aSoundFile == NULL) {
					string sData = pFile->GetData(nRow, nLine);
					m_Info.aSoundFile = new char[sData.size() + 1]; // メモリ確保
					std::char_traits<char>::copy(m_Info.aSoundFile, sData.c_str(), sData.size() + 1);
				}
				
			}
			else if (!strcmp(aDataSearch, "SetCamera")) {
				nLine += 4;

				ToData(m_Info.CameraPos.x, pFile, nRow, nLine); nLine++;
				ToData(m_Info.CameraPos.y, pFile, nRow, nLine); nLine++;
				ToData(m_Info.CameraPos.z, pFile, nRow, nLine); nLine++;
			}
			else if (!strcmp(aDataSearch, "StageWidth")){
				nLine += 4;
				int nWidth;
				
				ToData(nWidth, pFile, nRow, nLine);
				m_Info.nLine = 0;
				m_Info.nLineMax = nWidth;
			}
			else if (!strcmp(aDataSearch, "StageHeight")){
				nLine += 4;
				int nHeight;

				ToData(nHeight, pFile, nRow, nLine);
				m_Info.nRow = 0;
				m_Info.nRowMax = nHeight;
			}
			else if (!strcmp(aDataSearch, "SetColor")){
				StgColor(pFile,nRow,nLine);
			}
			else if (!strcmp(aDataSearch, "SetStage")){
				nRow++;
				// ステージ生成
				while (1)
				{
					char *aDataSearch = NULL;	// データ検索用
					string sData = pFile->GetData(nRow, nLine);
					char* cstr = new char[sData.size() + 1]; // メモリ確保
					std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
					aDataSearch = cstr;

					if (!strcmp(aDataSearch, "EndStage")) {
						if (cstr != NULL) {
							delete[] cstr;
							cstr = NULL;
						}
						break;
					}
					else
					{
						for (m_Info.nLine = 0; m_Info.nLine < m_Info.nLineMax; m_Info.nLine++)
						{
						int nType = -1;
							ToData(nType, pFile, nRow, m_Info.nLine);
							SetStage(nType);
						}
						nRow++;
						m_Info.nRow++;
					}

					if (cstr != NULL)
					{
						delete[] cstr;
						cstr = NULL;
					}
				}

				if (bRocket == true)
				{
					Stage::SetRocketPartsUI(CRocketPartsUI::Create());
				}
				
			}
			else if (!strcmp(aDataSearch, "SetLiftInfo")) {
				SetLiftInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetMeteorInfo")) {
				SetMeteorInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetLaserInfo")) {
				SetLaserInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetDogInfo")){
				SetDogInfo(pFile, nRow, nLine);
			}
			else if (!strcmp(aDataSearch, "SetPileInfo")) {
				SetPileInfo(pFile, nRow, nLine);
			}

			if (cstr != NULL){
				delete[] cstr;
				cstr = NULL;
			}
		}

		// 最大数に達したら返す
		if (nRow == nRowMax || bEnd)
		{
			break;
		}
	}

	// メモリ開放
	delete pFile;
	pFile = NULL;
}

//========================================
// 色情報
// Author:KEISUKE OTONO
//========================================
void CStageEditor::StgColor(CSVFILE *pFile, int nRow, int nLine)
{
	while (1)
	{
		nLine = 0;
		nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndColor")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "1P"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Player1 = m_StageColor.Set;
			//Stage::GetPlayer()->SetColor(0, m_StageColor.Player1);
		}
		else if (!strcmp(aDataSearch, "2P"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Player2 = m_StageColor.Set;
			//Stage::GetPlayer()->SetColor(1, m_StageColor.Player2);
		}
		else if (!strcmp(aDataSearch, "BgUp"))
		{
			SetColor(pFile, nRow, nLine);
		}
		else if (!strcmp(aDataSearch, "BgDown"))
		{
			SetColor(pFile, nRow, nLine);
		}
		else if (!strcmp(aDataSearch, "Block"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.Block = m_StageColor.Set;
		}
		else if (!strcmp(aDataSearch, "FillBlock"))
		{
			SetColor(pFile, nRow, nLine);
			m_StageColor.FillBlock = m_StageColor.Set;
		}

		if (cstr != NULL)
		{
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 色設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetColor(CSVFILE *pFile, int nRow, int nLine)
{
	nLine += 4;
	ToData(*(int*)&m_StageColor.Set.r, pFile, nRow, nLine); nLine++;
	ToData(*(int*)&m_StageColor.Set.g, pFile, nRow, nLine); nLine++;
	ToData(*(int*)&m_StageColor.Set.b, pFile, nRow, nLine); nLine++;
	ToData(*(int*)&m_StageColor.Set.a, pFile, nRow, nLine); nLine++;
}

//========================================
// ステージ生成
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetStage(int nType)
{
	if (nType >= 0)
	{
		float fSizeX = CStageObject::SIZE_OF_1_SQUARE;
		float fSizeY = CStageObject::SIZE_OF_1_SQUARE;
		D3DXVECTOR3 pos = INITPOS3D;

		pos.x += ((m_Info.nLineMax * -0.5f) + m_Info.nLine + 0.5f) * fSizeX;
		pos.y -= ((m_Info.nRowMax * -0.5f) + m_Info.nRow + 0.5f) * fSizeY;

		pos.z = 0.0f/* + RNLib::Number().GetRandomFloat(1.0f) * 4.0f*/;

		// 配置
		ObjPlace(fSizeX,fSizeY,pos,nType);
	}
}

//========================================
// ステージ切り替え
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SwapStage(int nStageIdx)
{
	int planet = m_Info.nPlanetIdx;
	int stage = m_PlanetType[planet].nStageIdx;
	int NecstStage = nStageIdx;

	if (stage != NecstStage)
	{
		if (NecstStage >= m_PlanetType[planet].nStageMax)
		{
			planet++;
			NecstStage = 0;
		}

		if (planet < m_Info.nPlanetMax)
		{
			if (Manager::Transition().GetState() == CTransition::STATE::NONE)
			{
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::NUI);
				Stage::SetStageNumber(planet, NecstStage);
			}
		}
		else
		{
			Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::NUI);
			CMode_Title::SetSelect(true);
		}
	}
}

//========================================
// オブジェクト配置
// Author:KEISUKE OTONO
//========================================
void CStageEditor::ObjPlace(float fSizeX, float fSizeY, D3DXVECTOR3 pos, int nType)
{
	// ブロック
	if (nType >= 100 && nType < 200) {
		Manager::StageObjectMgr()->BlockCreate(pos, (CBlock::LOOKS_TYPE)(nType - 100), m_StageColor.Block, false);
	}
	else if (nType >= 200 && nType < 300) {
		Manager::StageObjectMgr()->BlockCreate(pos, (CBlock::LOOKS_TYPE)(nType - 200), m_StageColor.Block, true);
	}

	switch (nType)
	{
	case TYPE_BLOCK:
		Manager::StageObjectMgr()->BlockCreate(pos, CBlock::LOOKS_TYPE::SOIL_BLOCK, m_StageColor.Block, true);
		break;
	case TYPE_TRAMPOLINE:
		pos.x += fSizeX / 2;
		Manager::StageObjectMgr()->TrampolineCreate(pos);
		break;
	case TYPE_SPIKE:
		Manager::StageObjectMgr()->SpikeCreate(pos,0);
		break;
	case TYPE_SPIKE_L:
		Manager::StageObjectMgr()->SpikeCreate(pos, 1);
		break;
	case TYPE_SPIKE_R:
		Manager::StageObjectMgr()->SpikeCreate(pos, 2);
		break;
	case TYPE_LIFT:
		Manager::StageObjectMgr()->MoveBlockCreate(pos, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Meteor:
		pos.x += fSizeX;
		pos.y -= fSizeY;
		Manager::StageObjectMgr()->MeteorCreate(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Laser:
		Manager::StageObjectMgr()->RoadTripLaserCreate(pos, pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		break;
	case TYPE_Extenddog:
		Manager::StageObjectMgr()->BlockCreate(pos,CBlock::LOOKS_TYPE::SOIL_BLOCK_GRASSY);
		break;
	case TYPE_COIN:
		Manager::StageObjectMgr()->CoinCreate(pos);
		break;
	case TYPE_FILL_BLOCK_11:
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_1x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_12:
		pos.y -= fSizeX * 0.5f;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_1x2, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_13:
		pos.y -= fSizeX;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_1x3, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_21:
		pos.x += fSizeY * 0.5f;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_2x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_22:
		pos.x += fSizeX * 0.5f;
		pos.y -= fSizeY * 0.5f;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_2x2, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_31:
		pos.x += fSizeY;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_3x1, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_33:
		pos.x += fSizeX;
		pos.y -= fSizeY;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_3x3, m_StageColor.FillBlock);
		break;
	case TYPE_FILL_BLOCK_44:
		pos.x += fSizeX * 1.5f;
		pos.y -= fSizeY * 1.5f;
		Manager::StageObjectMgr()->FillBlockCreate(pos, CFillBlock::BLOCKTYPE::TYPE_4x4, m_StageColor.FillBlock);
		break;
	case TYPE_PLAYER_0:
		pos.y += fSizeY * 0.5f;
		Stage::GetPlayer()->SetPos(0, pos);
		Manager::StageObjectMgr()->GoalGateCreate(pos,true);
		break;
	case TYPE_PLAYER_1:
		pos.y += -fSizeY * 0.5f;
		Stage::GetPlayer()->SetPos(1, pos);
		Manager::StageObjectMgr()->GoalGateCreate(pos,true);
		break;
	case TYPE_GOALGATE:
		Manager::StageObjectMgr()->GoalGateCreate(pos,false);
		break;
	case TYPE_PARTS:
		Manager::StageObjectMgr()->PartsCreate(pos);
		break;
	case TYPE_GOAL:
		pos.x += CStageObject::SIZE_OF_1_SQUARE * ((4 * 0.5f) - 0.5f);
		pos.y -= CStageObject::SIZE_OF_1_SQUARE * ((7 * 0.5f) - 0.5f);
		pos.z += CStageObject::SIZE_OF_1_SQUARE;
		Manager::StageObjectMgr()->RocketCreate(pos);
		bRocket = true;
		break;
	}
}

//========================================
// リフトの情報設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetLiftInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nLift = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndLiftInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "LiftMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nLiftMax = Max;
			m_LiftInfo = new Liftinfo[Max];
			assert(m_LiftInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetLift")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// データ検索用
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // メモリ確保
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndLift")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nLift < m_Info.nLiftMax){
						Manager::StageObjectMgr()->MoveBlockCreate(m_LiftInfo[nLift].posV, m_LiftInfo[nLift].posR, m_LiftInfo[nLift].move);
					}

					nLift++;
					break;
				}
				else if (!strcmp(aDataSearch, "PosV")) {
					int Row = 0; int Line = 4; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LiftInfo[nLift].posV = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "PosR")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LiftInfo[nLift].posR = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_LiftInfo[nLift].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_LiftInfo[nLift].move.y, pFile, nRow, nLine); nLine++;
					m_LiftInfo[nLift].move.z = 0.0f;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL){
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 隕石の情報設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetMeteorInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nMeteor = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndMeteorInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "MeteorMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nMateorMax = Max;
			m_MeteorInfo = new MeteorInfo[Max];
			assert(m_MeteorInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetMeteor")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// データ検索用
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // メモリ確保
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndMeteor")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nMeteor < m_Info.nMateorMax) {
						Manager::StageObjectMgr()->MeteorGeneratorCreate(m_MeteorInfo[nMeteor].pos, m_MeteorInfo[nMeteor].move, m_MeteorInfo[nMeteor].inteval);
					}

					nMeteor++;
					break;
				}
				else if (!strcmp(aDataSearch, "Pos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_MeteorInfo[nMeteor].pos = GetPos(Row, Line);
					m_MeteorInfo[nMeteor].pos.x += CStageObject::SIZE_OF_1_SQUARE;
					m_MeteorInfo[nMeteor].pos.y -= CStageObject::SIZE_OF_1_SQUARE;
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_MeteorInfo[nMeteor].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_MeteorInfo[nMeteor].move.y, pFile, nRow, nLine); nLine++;
					m_MeteorInfo[nMeteor].move.z = 0.0f;
				}
				else if (!strcmp(aDataSearch, "Inteval")) {
					int inteval = 0; nLine += 4;
					ToData(inteval, pFile, nRow, nLine);

					m_MeteorInfo[nMeteor].inteval = inteval;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// レーザーの情報設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetLaserInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nLaser = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndLaserInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "LaserMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nLaserMax = Max;
			m_LaserInfo = new LaserInfo[Max];
			assert(m_LaserInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetLaser")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// データ検索用
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // メモリ確保
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndLaser")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nLaser < m_Info.nLaserMax) {
						Manager::StageObjectMgr()->RoadTripLaserCreate(m_LaserInfo[nLaser].posV, m_LaserInfo[nLaser].posR, m_LaserInfo[nLaser].move);
					}

					nLaser++;
					break;
				}
				else if (!strcmp(aDataSearch, "PosV")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LaserInfo[nLaser].posV = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "PosR")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_LaserInfo[nLaser].posR = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "Move")) {
					nLine += 4;
					ToData(m_LaserInfo[nLaser].move.x, pFile, nRow, nLine); nLine++;
					ToData(m_LaserInfo[nLaser].move.y, pFile, nRow, nLine); nLine++;
					m_LaserInfo[nLaser].move.z = 0.0f;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 犬の情報設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetDogInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nDog = 0;
	int row = m_Info.nRowMax * 0.5f;
	float fSizeY = CStageObject::SIZE_OF_1_SQUARE;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndDogInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "DogMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nDogMax = Max;
			m_DogInfo = new DogInfo[Max];
			assert(m_DogInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetDog")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// データ検索用
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // メモリ確保
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndDog")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nDog < m_Info.nDogMax) {
						bool bReturn = false;

						float AddPosY = CStageObject::SIZE_OF_1_SQUARE * 0.5f;
						if (m_DogInfo[nDog].HipPos.y >= 0) {
							m_DogInfo[nDog].HipPos.y = m_DogInfo[nDog].HipPos.y - AddPosY;
							bReturn = true;
						}
						else if (m_DogInfo[nDog].HipPos.y <= 0) {
							m_DogInfo[nDog].HipPos.y = m_DogInfo[nDog].HipPos.y + AddPosY;
						}

						Manager::StageObjectMgr()->ExtenddogCreate(m_DogInfo[nDog].HeadPos, m_DogInfo[nDog].HipPos, m_DogInfo[nDog].HeightMin, m_DogInfo[nDog].HeightMax, m_DogInfo[nDog].bElasticity, bReturn);
					}

					nDog++;
					break;
				}
				else if (!strcmp(aDataSearch, "HeadPos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_DogInfo[nDog].HeadPos = GetPos(Row, Line);

					if (Row <= row)
						m_DogInfo[nDog].HeadPos.y += fSizeY * 0.25f;
					else if (Row >= row)
						m_DogInfo[nDog].HeadPos.y += -fSizeY * 0.25f;
				}
				else if (!strcmp(aDataSearch, "HipPos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_DogInfo[nDog].HipPos = GetPos(Row, Line);

					if (Row <= row)
						m_DogInfo[nDog].HipPos.y += fSizeY * 0.25f;
					else if (Row >= row)
						m_DogInfo[nDog].HipPos.y += -fSizeY * 0.25f;
				}
				else if (!strcmp(aDataSearch, "Height")) {
					nLine += 4;
					ToData(m_DogInfo[nDog].HeightMin, pFile, nRow, nLine); nLine++;
					ToData(m_DogInfo[nDog].HeightMax, pFile, nRow, nLine); nLine++;
				}
				else if (!strcmp(aDataSearch, "Elasticity")) {
					nLine += 4; int nShrink = 0;
					ToData(nShrink, pFile, nRow, nLine); nLine++;

					if (nShrink <= 0)
						m_DogInfo[nDog].bElasticity = false;
					else
						m_DogInfo[nDog].bElasticity = true;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 杭の情報設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetPileInfo(CSVFILE *pFile, int nRow, int nLine)
{
	int nPile = 0;

	while (1)
	{
		nLine = 0;	nRow++;
		char *aDataSearch = NULL;	// データ検索用
		string sData = pFile->GetData(nRow, nLine);
		char* cstr = new char[sData.size() + 1]; // メモリ確保
		std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
		aDataSearch = cstr;

		if (!strcmp(aDataSearch, "EndPileInfo")) {
			if (cstr != NULL) {
				delete[] cstr;
				cstr = NULL;
			}
			break;
		}
		else if (!strcmp(aDataSearch, "PileMax")) {
			int Max = 0;
			nLine += 4;
			ToData(Max, pFile, nRow, nLine);

			m_Info.nPileMax = Max;
			m_PileInfo = new PileInfo[Max];
			assert(m_PileInfo != NULL);
		}
		else if (!strcmp(aDataSearch, "SetPile")) {
			while (1)
			{
				nLine = 0;	nRow++;				char *aDataSearch = NULL;	// データ検索用
				string sData = pFile->GetData(nRow, nLine);
				char* cstr = new char[sData.size() + 1]; // メモリ確保
				std::char_traits<char>::copy(cstr, sData.c_str(), sData.size() + 1);
				aDataSearch = cstr;

				if (!strcmp(aDataSearch, "EndPile")) {
					if (cstr != NULL) {
						delete[] cstr;
						cstr = NULL;
					}

					if (nPile < m_Info.nPileMax) {

						Manager::StageObjectMgr()->PileCreate(m_PileInfo[nPile].pos, m_PileInfo[nPile].nNumPile, m_PileInfo[nPile].fCaveIn);
					}

					nPile++;
					break;
				}
				else if (!strcmp(aDataSearch, "Pos")) {
					int Row = 0; int Line = 0; nLine += 4;
					ToData(Line, pFile, nRow, nLine); nLine++;
					ToData(Row, pFile, nRow, nLine); nLine++;

					m_PileInfo[nPile].pos = GetPos(Row, Line);
				}
				else if (!strcmp(aDataSearch, "NumPile")) {
					nLine += 4;
					int nNumPile = 0;
					ToData(nNumPile, pFile, nRow, nLine); nLine++;

					if (nNumPile <= 3){
						nNumPile = 3;
					}

					m_PileInfo[nPile].nNumPile = nNumPile;
				}
				else if (!strcmp(aDataSearch, "CaveIn")) {
					nLine += 4;
					ToData(m_PileInfo[nPile].fCaveIn, pFile, nRow, nLine); nLine++;
				}

				if (cstr != NULL) {
					delete[] cstr;
					cstr = NULL;
				}
			}
		}

		if (cstr != NULL) {
			delete[] cstr;
			cstr = NULL;
		}
	}
}

//========================================
// 座標の取得
// Author:KEISUKE OTONO
//========================================
D3DXVECTOR3 CStageEditor::GetPos(int nRow, int nLine)
{
	float fSizeX = CStageObject::SIZE_OF_1_SQUARE;
	float fSizeY = CStageObject::SIZE_OF_1_SQUARE;
	D3DXVECTOR3 pos = INITPOS3D;

	pos.x += ((m_Info.nLineMax * -0.5f) + nLine + 0.5f) * fSizeX;
	pos.y -= ((m_Info.nRowMax * -0.5f) + nRow + 0.5f) * fSizeY;
	pos.z = 0.0f;

	return pos;
}

//========================================
// ステージ解放に必要なコインの取得
// Author:KEISUKE OTONO
//========================================
int CStageEditor::GetStageCoin(int planet, int stage)
{
	int Coin = m_PlanetType[planet].StageType[stage].nNumCoin;
	
	if (Coin <= 0)
		Coin = 0;

	return Coin;
}

//========================================
// 制限時間の取得
// Author:KEISUKE OTONO
//========================================
int CStageEditor::GetStageLimittime(int planet, int stage) {

	return m_PlanetType[planet].StageType[stage].nLimitTime;
}

//========================================
// ステージ解放の設定
// Author:KEISUKE OTONO
//========================================
void CStageEditor::SetStageRel(int planet, int stage, bool bRel)
{
	if (planet <= 0)
		planet = 0;
	else if (planet >= m_Info.nPlanetMax)
		planet = m_Info.nPlanetMax - 1;

	if (stage <= 0)
		stage = 0;
	else if (stage >= m_PlanetType[planet].nStageMax)
		stage = m_PlanetType[planet].nStageMax - 1;

	m_PlanetType[planet].StageType[stage].bRelease = bRel;
}

//========================================
// ステージ解放の取得
// Author:KEISUKE OTONO
//========================================
bool CStageEditor::GetStageRel(int planet, int stage)
{
	if (planet <= 0)
		planet = 0;
	else if (planet >= m_Info.nPlanetMax)
		planet = m_Info.nPlanetMax - 1;

	if (stage <= 0)
		stage = 0;
	else if (stage >= m_PlanetType[planet].nStageMax)
		stage = m_PlanetType[planet].nStageMax - 1;

	return m_PlanetType[planet].StageType[stage].bRelease;
}

//========================================
// 変換
// Author:KEISUKE OTONO
//========================================

// int
bool CStageEditor::ToData(int &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stoi(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// UShort
bool CStageEditor::ToData(UShort& val, CSVFILE* pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stoi(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// float
bool CStageEditor::ToData(float &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stof(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

// double
bool CStageEditor::ToData(double &val, CSVFILE *pFile, int nRow, int nLine)
{
	try
	{
		string sData = pFile->GetData(nRow, nLine);
		val = stod(sData);
		return true;
	}
	catch (...)
	{
		return false;
	}
}

//========================================
// ステージ総数取得
// Author:HIRASAAWA SHION
//========================================
void CStageEditor::GetPlanetAndStageMax(CInt planet, int& NumPlanet, int& NumStage)
{
	NumPlanet = m_Info.nPlanetMax;
	NumStage = m_PlanetType[planet].nStageMax;
}