//========================================
// 
// モード:タイトル処理
// Author:KEISUKE OTONO
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "../../_RNLib/RNsettings.h"
#include "../../_RNLib/Basis/Mechanical/sound.h"
#include "../main.h"
#include "mode_title.h"
#include "mode_game.h"
#include "../UI/MenuUi.h"
#include "../System/words/words.h"
#include "../System/words/font-text.h"
#include "../resource.h"
#include "../stage.h"
#include "../UI/coinUI.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Titleクラス
//----------|---------------------------------------------------------------------
//================================================================================
const D3DXVECTOR3 SELECTBOX = D3DXVECTOR3(7.5f, -15.0f, -125.0f);
const D3DXVECTOR3 UNSELECTBOX = D3DXVECTOR3(7.5f, -20.0f, -120.0f);
const D3DXVECTOR3 SELBOXRATE = SELECTBOX - UNSELECTBOX;
const D3DXVECTOR3 NUMPOSSELBOX = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
const D3DXVECTOR3 NUMPOSROCKET = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
const D3DXVECTOR3 FADEROCKET = D3DXVECTOR3(70.0f, UNSELECTBOX.y, UNSELECTBOX.z) + NUMPOSROCKET;
const D3DXVECTOR3 COINUIPOS = D3DXVECTOR3(25.0f, 16.7f, -136.0f);
const D3DXVECTOR3 IMAGE_STG_POS = D3DXVECTOR3(-40.0f, 10.0f, -110.0f);
const float NUM_ROT = 0.3925f;
const int MAX_COUNT = 24;
CInt ANIMCOUNT = MAX_COUNT;

CMenuUI *CMode_Title::m_MenuUI = NULL;
bool CMode_Title::m_bStageSelect = false;

int CMode_Title::m_nPlanetIdx = 0;
int CMode_Title::m_nStageSelect = 0;

//========================================
// コンストラクタ
//========================================
CMode_Title::CMode_Title(void) {

	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++) {
		m_BgPos[nCnt] = INITD3DXVECTOR3;
		m_TexIdx[nCnt] = 0;
	}

	Title              = TITLE_TITLE;
	NextTitle		   = TITLE_TITLE;
	m_nOldSelect       = 0;
	m_nOldnPlanet      = 0;
	m_PlanetAngle      = 0.0f;
	m_PlanetType       = NULL;
	m_CoinUI           = NULL;
	m_bBackMode        = false;
	m_RocketIdx        = RNLib::Model().Load("data\\MODEL\\Rocket_Body.x");
	m_SelIdx           = RNLib::Model().Load("data\\MODEL\\Select_Box.x");
	m_StgBoardIdx      = RNLib::Model().Load("data\\MODEL\\Stage_Board.x");
	m_CoinBoardIdx     = RNLib::Model().Load("data\\MODEL\\Coin_Board.x");
	m_ArrowIdx         = RNLib::Model().Load("data\\MODEL\\Arrow.x");
	m_AnimCnt = 0;
	m_RotCnt = 0;
	m_bStageChange = false;
	m_bRocketMove = false;
}

//========================================
// デストラクタ
//========================================
CMode_Title::~CMode_Title(void) {

	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++) {
		if (m_TITLE[nCnt] != NULL) {
			m_TITLE[nCnt]->Uninit();
			m_TITLE[nCnt] = NULL;
		}

		if (m_TitleShadow[nCnt] != NULL) {
			m_TitleShadow[nCnt]->Uninit();
			m_TitleShadow[nCnt] = NULL;
		}
	}

	if (m_PlanetType != NULL) {
		delete[] m_PlanetType;
		m_PlanetType = NULL;
	}

	if (m_MenuUI != NULL)
	{
		delete m_MenuUI;
		m_MenuUI = NULL;
	}

	// テキストの破棄
	TextRelease(TEXT_ALL);
}

//========================================
// 初期化処理
//========================================
void CMode_Title::Init(void) {
	CMode::Init();

	// 遷移設定
	Manager::Transition().Open(CTransition::TYPE::FADE, 60);

	// テキストの初期化
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++) {
		m_bMove[nCnt] = false; 
		m_TITLE[nCnt] = m_TitleShadow[nCnt] = NULL;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++) {
		m_pMenu[nCnt] = NULL;
	}

	// メニュー生成
	m_MenuUI = CMenuUI::Create(CMode::TYPE::TITLE );

	// ステージ読込
	CreateStageSelectInfo();

	if (m_bStageSelect) 
	{// ステージ選択時、
		SwapMode(TITLE_SELECT);
		m_bStageSelect = false;
	}
	else
	{// ステージ非選択時、
		// モード設定
		SwapMode(TITLE_TITLE);
	}

	// テクスチャ
	m_BgPos[0] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, -100.0f);
	m_BgPos[1] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1060, -50.0f);

	for (int nCnt = 1; nCnt < TEX_MAX; nCnt++) {
		m_TexIdx[nCnt] = 0;
	}

	// テクスチャの読み込み
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Space.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Planet.png");
	m_TexIdx[2] = RNLib::Texture().Load("data\\TEXTURE\\StageSelect\\Number.png");

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_CoinUI == NULL) {
		m_CoinUI = CCoinUI::Create(COINUIPOS);
	}

	// 状態設定
	SetState((int)STATE::NONE);
}

//========================================
// 終了処理
//========================================
void CMode_Title::Uninit(void) {
	CMode::Uninit();

	CMemory::Release(&m_AnimCnt);
}

//========================================
// 更新処理
//========================================
void CMode_Title::Update(void) {
	CMode::Update();

	// 背景の描画
	RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, m_BgPos[TEX_BG], 0.0f, false)
		->SetSize(1280.0f, 720.0f)
		->SetCol(Color{ 255,255,255,255 })
		->SetTex(m_TexIdx[TEX_BG]);

	if (Title <= TITLE_MENU || Title == TITLE_SELECT)
	{
		if (Title <= TITLE_MENU)
			m_PlanetAngle += -0.002f;
		else if (Title == TITLE_SELECT)
			m_PlanetAngle += -0.002f;

		RNLib::Number().LoopClamp(&m_PlanetAngle, D3DX_PI, -D3DX_PI);
	}

	if (Title <= TITLE_MENU)
	{
		RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, m_BgPos[TEX_PLANET], m_PlanetAngle, false)
			->SetSize(1400.0f, 1400.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(m_TexIdx[TEX_PLANET]);

		// ロケット
		RNLib::Model().Put(PRIORITY_OBJECT, m_RocketIdx, D3DXVECTOR3(60.0f, -40.0f, -20.0f), D3DXVECTOR3(0.0f, D3DX_PI, 1.9f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), false)
			->SetOutLineIdx(true);
	}

	// ゲーム終了
	bool isEnd = false;
	if (m_MenuUI != NULL)
		isEnd = m_MenuUI->GetInfo().bGameEnd;

	if (isEnd)
		PostQuitMessage(0);
	else
	{

		// 各モードの処理
		if (Title == TITLE_TITLE)
			TextAnime();
		else if (Title == TITLE_MENU) {
			m_MenuUI->Update();
			MenuAnime();
		}
		else if (Title == TITLE_SELECT)
			StageSelect();
		else if (Title == TITLE_NEXT)
			return;

		if (m_bStageChange == false) {
			if (m_bRocketMove == false) {
				if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && Manager::Transition().GetState() == CTransition::STATE::NONE)
				{
					//RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::SELECT], CSound::CATEGORY::SE, false);

			switch (Title)
			{
			case TITLE_OUTSET:
			{
				SwapMode(TITLE_MENU_ANIME);
			}
			break;
			case TITLE_MENU:
				break;
			case TITLE_SELECT:
			{
				SwapMode(TITLE_NEXT);
				Stage::SetStageNumber(m_nPlanetIdx, m_nStageSelect);
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);

						if (m_PlanetType != NULL)
						{
							delete[] m_PlanetType;
							m_PlanetType = NULL;
						}
					}
					break;
					}
				}
			}
		}

		if (Title != NextTitle)
			SwapMode(NextTitle);
	}
}

//========================================
// 更新処理(状態)
//========================================
void CMode_Title::ProcessState(const PROCESS process) {
	switch ((STATE)m_state) {
		//----------------------------------------
		// 無し
		//----------------------------------------
	case STATE::NONE: {
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {

		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {

		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {

		}break;
		}
	}break;
	}
}

//========================================
// タイトルテキストアニメーション
//========================================
void CMode_Title::TextAnime(void)
{
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		if (m_TITLE[nCnt] != NULL)
		{
			D3DXVECTOR3 pos = m_TITLE[nCnt]->GetPos();

			if (pos.y <= 210.0f && m_bMove[nCnt]) {
				D3DXVECTOR3 move;

				move.y = 15.0f;

				m_TITLE[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
				m_TitleShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

				if (pos.y >= 200.0f) {
					move.y = 0.0f;
					pos.y = 200;

					m_TITLE[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
					m_TitleShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

					if (nCnt == WORDS_MAX - 1)
						SwapMode(TITLE_OUTSET);

				}
				else if (pos.y >= 20 && nCnt != WORDS_MAX - 1 && !m_bMove[nCnt + 1])
					m_bMove[nCnt + 1] = true;
			}
		}
	}
}

//========================================
// メニュー演出
//========================================
void CMode_Title::MenuAnime(void)
{
	bool MenuEnd = m_MenuUI->m_MenuEnd;
	bool backMode = m_MenuUI->GetInfo().bBackMode;

	if (MenuEnd && backMode)
		NextTitle = TITLE_TITLE;
	else if (MenuEnd && !m_bBackMode) {
		TextRelease(TEXT_ALL);
		NextTitle = TITLE_SELECT;
			return;
	}

	// タイトル
	if (Title == TITLE_MENU) {
		for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
		{
			if (m_TITLE[nCnt] != NULL) {
				D3DXVECTOR3 pos = m_TITLE[nCnt]->GetPos();

				if (pos.y >= -60.0f && m_bMove[nCnt]) {
					D3DXVECTOR3 move;

					move.y = -20.0f;

					m_TITLE[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
					m_TitleShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

					if (pos.y <= -60.0f) {
						delete[nCnt] m_TITLE[nCnt];
						m_TITLE[nCnt] = NULL;
					}
				}
			}
		}
	}
}

//========================================
// ステージ選択情報の生成
//========================================
void CMode_Title::CreateStageSelectInfo(void) {

	Manager::StgEd()->Uninit();
	Manager::StgEd()->FileLoad();
	
	// 惑星最大数取得
	int nPlanetMax = Manager::StgEd()->GetPlanetMax();

	m_PlanetType = new PlanetType[nPlanetMax];
	m_PlanetAngle = 0.0f;

	for (int nCnt = 0; nCnt < nPlanetMax; nCnt++) {
		char *aTexFile = Manager::StgEd()->GetType()[nCnt].aTexFile;
		char *aStgName = Manager::StgEd()->GetType()[nCnt].aName;

		m_PlanetType[nCnt].nModel = RNLib::Model().Load(aTexFile);
	}

	int nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
	CMemory::Alloc(&m_AnimCnt,nStageMax);

	const Pos3D PosCor = Pos3D(nStageMax * (NUMPOSSELBOX.x * 0.5f), 0.0f, 0.0f);
	m_RocketPos = m_RocketPosOld = UNSELECTBOX - PosCor + NUMPOSROCKET;
	m_RocketposRate = INITD3DXVECTOR3;
	m_RocketRot = m_RocketRotOld = D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * 0.5f);
	m_RocketRotRate = INITD3DXVECTOR3;
	m_RotCnt = ANIMCOUNT;
	m_nDrawPlanet = m_nPlanetIdx;
	m_RocketAnimCnt = 0;
	m_ImageStgCnt = 0;
	m_NumAnimCnt = ANIMCOUNT * 0.5f;
	for (int AnimInit = 0; AnimInit < Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax; AnimInit++)
		m_AnimCnt[AnimInit] = 0;

	m_nCnt = 0;
}

//========================================
// ステージ選択
//========================================
void CMode_Title::StageSelect(void) {

	int nPlanetMax = Manager::StgEd()->GetPlanetMax();
	int nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
	const Pos3D PosCor = Pos3D(nStageMax * (NUMPOSSELBOX.x * 0.5f), 0.0f, 0.0f);
	float RocketAnimRate;

	//描画処理
	StageDraw(nPlanetMax, nStageMax, PosCor,RocketAnimRate);

	//----------------------------------------
	// ステージ選択処理
	//----------------------------------------
	if(m_bStageChange == false)
	m_nOldSelect = m_nStageSelect;
	bool bInput = false;

	if (m_bStageChange == false) {
		if (m_bRocketMove == false) {

			if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK)) {
				TextRelease(TEXT_MENU);
				SwapMode(TITLE_MENU_ANIME);
				return;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT)) {
				m_nStageSelect--;
				bInput = true;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT)) {
				m_nStageSelect++;
				bInput = true;
			}

			if (bInput) {
				m_nOldnPlanet = m_nPlanetIdx;
				m_nDrawPlanet = m_nOldnPlanet;

				if (m_nStageSelect > -1 && m_nStageSelect < nStageMax) {
					m_RocketPosOld = m_RocketPosOld + (m_RocketposRate * RocketAnimRate);
					m_RotCnt = 0;
					m_RocketAnimCnt = 0;
				}

				if (m_nStageSelect < 0) {
					m_nSelectTemp = m_nStageSelect;
					m_nStageSelect = 0;

					if (m_nPlanetIdx != 0) {
						m_RotCnt = 0;
						m_RocketAnimCnt = 0;
						m_nOldSelect = -1;
						m_bStageChange = true;
						m_StgFlag = STAGE::DESPAWN;
					}
				}
				else if (m_nStageSelect >= nStageMax) {
					m_nSelectTemp = m_nStageSelect;
					m_nStageSelect = nStageMax - 1;

					if (m_nPlanetIdx < nPlanetMax - 1) {
						m_RotCnt = 0;
						m_RocketAnimCnt = 0;
						m_nOldSelect = 5;
						m_bStageChange = true;
						m_StgFlag = STAGE::DESPAWN;
					}
				}
			}
		}
	}

	//出現処理
	if (m_StgFlag == STAGE::POP)
		StagePop(nPlanetMax,nStageMax,PosCor);
}
//========================================
// ステージ描画処理
//========================================
void CMode_Title::StageDraw(int nPlanet, int nStage, D3DXVECTOR3 poscor, float &RktAnimRt) {

	float CountRate = CEase::Easing(CEase::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
	float ImageCntRate = CEase::Easing(CEase::TYPE::IN_SINE, m_ImageStgCnt, ANIMCOUNT * 0.5);
	float AnimRate;
	D3DXVECTOR3 numpos;

	{// 惑星の描画
		RNLib::Model().Put(PRIORITY_OBJECT, m_PlanetType[m_nDrawPlanet].nModel, D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f) * CountRate, false)
			->SetOutLineIdx(5);

		RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(IMAGE_STG_POS), D3DXVECTOR3(0.0f, -0.58875f, 0.0f))
			->SetSize(32.0f * ImageCntRate, 18.0f * ImageCntRate)
			->SetCol(INITCOLOR)
			/*->SetTex(m_TexIdx[0])*/
			->SetZTest(true);

		if (m_bStageChange == false) {
			if (m_bRocketMove == false) {
				if (m_RocketAnimCnt < ANIMCOUNT * 0.5) {
					if (m_ImageStgCnt > 0)
						m_ImageStgCnt--;
				}
				else if (m_RocketAnimCnt >= ANIMCOUNT * 0.5) {
					if (m_ImageStgCnt < ANIMCOUNT * 0.5)
						m_ImageStgCnt++;
				}
			}
		}

		if (m_bStageChange == true || m_bRocketMove == true) {

			if (m_RocketAnimCnt < ANIMCOUNT) {
				if (m_ImageStgCnt > 0)
					m_ImageStgCnt--;
			}
			else if (m_RocketAnimCnt >= ANIMCOUNT) {
				if (m_ImageStgCnt < ANIMCOUNT)
					m_ImageStgCnt++;
			}
		}

		if (m_nCnt < MAX_COUNT && m_bStageChange == false)
			m_nCnt++;
		else if (m_nCnt > 0 && m_bStageChange == true)
			m_nCnt--;

		if (m_nCnt == 0 && m_bStageChange == true) {
			if (m_nOldSelect == nStage)
				m_nOldSelect = 0;
			else if (m_nOldSelect == -1)
				m_nOldSelect = nStage;
			m_StgFlag = STAGE::POP;
		}
	}

	{//看板
		//ステージ看板
		RNLib::Model().Put(PRIORITY_OBJECT, m_StgBoardIdx, D3DXVECTOR3(0.0f, 16.5f + 12.0f * (1.0f - CountRate), -145.0f), INITD3DXVECTOR3, INITSCALE3D)
			->SetOutLineIdx(5);

		//コイン看板
		RNLib::Model().Put(PRIORITY_OBJECT, m_CoinBoardIdx, D3DXVECTOR3(30.0f, 18.0f, -135.0f), D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f), INITSCALE3D)
			->SetOutLineIdx(5);

		if (m_CoinUI != NULL) {
			m_CoinUI->Update();
		}
	}

	{// 矢印の描画
		if (m_nPlanetIdx > 0) {
			// 矢印の描画(左)
			RNLib::Model().Put(PRIORITY_OBJECT, m_ArrowIdx, D3DXVECTOR3(SELECTBOX.x - poscor.x - NUMPOSSELBOX.x * 0.7f, UNSELECTBOX.y, UNSELECTBOX.z - 5.0f), D3DXVECTOR3(0.0f, 0.0f, 1.57f), INITSCALE3D)
				->SetCol(Color{ 0,168,112,255 });
		}

		if (m_nPlanetIdx < nPlanet - 1) {
			// 矢印の描画(右)
			RNLib::Model().Put(PRIORITY_OBJECT, m_ArrowIdx, D3DXVECTOR3(SELECTBOX.x + poscor.x - NUMPOSSELBOX.x * 0.3f, UNSELECTBOX.y, UNSELECTBOX.z - 5.0f), D3DXVECTOR3(0.0f, 0.0f, -1.57f), INITSCALE3D)
				->SetCol(Color{ 0,168,112,255 });
		}
	}

	// 選択アイコンの処理
	for (int nCnt = 0; nCnt < nStage; nCnt++) {

		if (nCnt == m_nStageSelect) {
			//アニメーション割合
			AnimRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_AnimCnt[nCnt], ANIMCOUNT);
			if (m_AnimCnt[nCnt] < ANIMCOUNT) m_AnimCnt[nCnt]++;

			float RotRate = CEase::Easing(CEase::TYPE::OUT_SINE, m_RotCnt, ANIMCOUNT);
			if (m_RotCnt < ANIMCOUNT) m_RotCnt++;

			//選択時	//ロケット
			RktAnimRt = CEase::Easing(CEase::TYPE::OUT_SINE, m_RocketAnimCnt, ANIMCOUNT);

			if (m_RocketAnimCnt < ANIMCOUNT) m_RocketAnimCnt++;
			if (m_RocketAnimCnt == ANIMCOUNT) {
				if (m_bStageChange == true) {
					m_RocketAnimCnt = 0;
					m_bStageChange = false;
					m_bRocketMove = true;
					if (m_nStageSelect == 0)
						m_RocketPosOld = FADEROCKET;
					else if (m_nStageSelect == nStage - 1)
						m_RocketPosOld = D3DXVECTOR3(-FADEROCKET.x, FADEROCKET.y, FADEROCKET.z);
				}
				else {
					m_bRocketMove = false;
					m_RocketPosOld = m_RocketPos; m_RocketposRate = INITD3DXVECTOR3;
				}
			}

			if (m_bRocketMove == false) {
				if (m_bStageChange == false)
					m_RocketPos = UNSELECTBOX - poscor + nCnt * NUMPOSSELBOX + NUMPOSROCKET;
				if (m_bStageChange == true) {
					if (m_nStageSelect == 0)
						m_RocketPos = D3DXVECTOR3(-FADEROCKET.x, FADEROCKET.y, FADEROCKET.z);
					else if (m_nStageSelect == nStage - 1)
						m_RocketPos = FADEROCKET;
				}
			}
			else {
				if (m_RocketAnimCnt > 0) {
					if (m_nStageSelect == 0)
						m_RocketPos = UNSELECTBOX - poscor + 0.0f * NUMPOSSELBOX + NUMPOSROCKET;
					else if (m_nStageSelect == nStage - 1)
						m_RocketPos = UNSELECTBOX - poscor + (nStage - 1) * NUMPOSSELBOX + NUMPOSROCKET;
				}
			}

			if (m_nStageSelect != m_nOldSelect){
				m_RocketposRate = m_RocketPos - m_RocketPosOld;
				m_RocketRotOld = m_RocketRot;

				if (m_RocketposRate.x > 0)
					m_RocketRot = D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * 0.5f);
				else
					m_RocketRot = D3DXVECTOR3(D3DX_PI, 0.0f, D3DX_PI * 0.5f);

				m_RocketRotRate = m_RocketRot - m_RocketRotOld;
			}

			{//ロケット
				RNLib::Model().Put(PRIORITY_OBJECT, m_RocketIdx, m_RocketPosOld + (m_RocketposRate * RktAnimRt), m_RocketRotOld + (RotRate * m_RocketRotRate), Scale3D(0.15f, 0.15f, 0.15f), false);
			}

			float NumRate = CEase::Easing(CEase::TYPE::LINEAR, m_NumAnimCnt, ANIMCOUNT * 2);
			if (m_bRotDir) {
				if (m_NumAnimCnt < ANIMCOUNT * 2)
					m_NumAnimCnt++;
				else
					m_bRotDir = false;
			}
			else{
				if (m_NumAnimCnt > 0)
					m_NumAnimCnt--;
				else
					m_bRotDir = true;
			}

			//相対位置を求める
			const D3DXVECTOR3 rot = D3DXVECTOR3(INITROT3D.x, INITROT3D.y + (NUM_ROT - ((NUM_ROT * 2.0f) * NumRate)), INITROT3D.z);
			numpos = D3DXVECTOR3(0.0f,0.0f,-5.0f);
			Matrix mtxBlock = CMatrix::ConvPosRotScaleToMtx(UNSELECTBOX - poscor + nCnt * NUMPOSSELBOX + (SELBOXRATE * AnimRate), rot, INITSCALE3D * CountRate);
			Matrix mtxNum = CMatrix::MultiplyMtx(
				CMatrix::ConvPosToMtx(numpos), 
				mtxBlock);

			//選択時	//ブロック
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, mtxBlock, false)
				->SetCol(Color{ 243,191,63,255 });

			//数字テクスチャ
			if (m_bStageChange == false && m_nCnt == MAX_COUNT) {
				RNLib::Polygon3D().Put(PRIORITY_UI, mtxNum)
					->SetSize(5.0f, 5.0f)
					->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
			}
		}
		else {
			//アニメーション割合
			AnimRate = 1.0f - CEase::Easing(CEase::TYPE::OUT_SINE, m_AnimCnt[nCnt], ANIMCOUNT);
			if (m_AnimCnt[nCnt] > 0) m_AnimCnt[nCnt]--;

			// 非選択時	//ブロック
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, SELECTBOX - poscor + nCnt * NUMPOSSELBOX - (SELBOXRATE * AnimRate), INITD3DXVECTOR3, INITSCALE3D * CountRate, false)
				->SetCol(Color{ 81,63,21,255 });

			//数字
			if (m_bStageChange == false && m_nCnt == MAX_COUNT) {
				numpos = D3DXVECTOR3(SELECTBOX.x - poscor.x + (nCnt * NUMPOSSELBOX.x), SELECTBOX.y, SELECTBOX.z - 5.0f);
				RNLib::Polygon3D().Put(PRIORITY_UI, numpos - (SELBOXRATE * AnimRate), INITROT3D)
					->SetSize(5.0f, 5.0f)
					->SetCol(Color{ 85,85,85,255 })
					->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
			}
		}
	}
}
//========================================
// ステージ切り替え処理
//========================================
void CMode_Title::StagePop(int nPlanet,int &nStage,D3DXVECTOR3 poscor) {

	int nStageMaxOld = nStage;

	if (m_nSelectTemp < 0 && m_nPlanetIdx != 0) {

			m_nPlanetIdx--;
			nStage = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
			m_nStageSelect = nStage - 1;
			m_nOldSelect = nStage;
		}
		else if (m_nSelectTemp >= nStage && m_nPlanetIdx != nStage - 1) {
			
			m_nPlanetIdx++;
			m_nStageSelect = 0;
			m_nOldSelect = -1;
			nStage = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
		}

	m_nDrawPlanet = m_nPlanetIdx;

	RNLib::Number().Clamp(&m_nStageSelect, nStage - 1, 0);

	if (nStage != nStageMaxOld) {
		CMemory::Alloc(&m_AnimCnt, nStage);
		m_RocketPos = UNSELECTBOX - poscor + NUMPOSROCKET;
		m_RocketposRate = INITD3DXVECTOR3;
		for (int AnimInit = 0; AnimInit < nStage; AnimInit++)
			m_AnimCnt[AnimInit] = 0;
	}
	m_StgFlag = STAGE::NONE;
}

//========================================
// モード切替
//========================================
void CMode_Title::SwapMode(TITLE aTitle) {
	Title = aTitle;
	NextTitle = aTitle;
	switch (aTitle)
	{
	case CMode_Title::TITLE_TITLE:
	{
		TextRelease(TEXT_TITLE);
		{
			m_TitleShadow[0] = CWords::Create("Ｃ", D3DXVECTOR3(786.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_TitleShadow[1] = CWords::Create("Ｏ", D3DXVECTOR3(946.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_TitleShadow[2] = CWords::Create("Ｓ", D3DXVECTOR3(1096.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_TitleShadow[3] = CWords::Create("／", D3DXVECTOR3(1246.0f, -54.0f, 0.0f), D3DXVECTOR3(100.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_TitleShadow[4] = CWords::Create("Ｍ", D3DXVECTOR3(1406.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_TitleShadow[5] = CWords::Create("Ｏ", D3DXVECTOR3(1566.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_TitleShadow[6] = CWords::Create("Ｓ", D3DXVECTOR3(1706.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		}
		{
			m_TITLE[0] = CWords::Create("Ｃ", D3DXVECTOR3(780.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_TITLE[1] = CWords::Create("Ｏ", D3DXVECTOR3(940.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_TITLE[2] = CWords::Create("Ｓ", D3DXVECTOR3(1090.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_TITLE[3] = CWords::Create("／", D3DXVECTOR3(1234.0f, -66.0f, 0.0f), D3DXVECTOR3(100.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_TITLE[4] = CWords::Create("Ｍ", D3DXVECTOR3(1400.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_TITLE[5] = CWords::Create("Ｏ", D3DXVECTOR3(1560.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_TITLE[6] = CWords::Create("Ｓ", D3DXVECTOR3(1700.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
		}
		m_bMove[0] = true;
		m_bBackMode = false;
	}
		break;
	case CMode_Title::TITLE_OUTSET:
	{
		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),60.0f,5,10,-1, };// 45
		FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),true, D3DXVECTOR3(6.0f,6.0f,0.0f) ,D3DXVECTOR2(4.0f,4.0f) };

		m_pMenu[0] = CFontText::Create(CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(330.0f, 600.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
			"ボタンを押してねД", CFont::FONT_ROND_B, &pFont, false, false, &pShadow);
	}
		break;
	case CMode_Title::TITLE_MENU_ANIME:
	{
		TextRelease(TEXT_MENU);

		// メニュー生成
		m_MenuUI->MenuCreate();

		NextTitle = TITLE_MENU;
	}
		break;
	case CMode_Title::TITLE_SELECT:
	{
		m_MenuUI->TextRelease(CMenuUI::TEXT_ALL);

		if (m_MenuUI != NULL)
		{
			delete m_MenuUI;
			m_MenuUI = NULL;
		}

		m_bStageSelect = false;
		m_nOldSelect = 0;
		m_nPlanetIdx = 0;
		m_nOldnPlanet = 0;

		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };// 45
	}
		break;
	case CMode_Title::TITLE_NEXT:
		TextRelease(TEXT_ALL);
		break;
	}
}

//========================================
// テキスト削除
//========================================
void CMode_Title::TextRelease(TEXT type) {
	// タイトル
	if (type == TEXT_TITLE || type == TEXT_ALL) {
		for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++){
			if (m_TITLE[nCnt] != NULL){
				m_TITLE[nCnt]->Uninit();
				m_TITLE[nCnt] = NULL;
			}

			if (m_TitleShadow[nCnt] != NULL){
				m_TitleShadow[nCnt]->Uninit();
				m_TitleShadow[nCnt] = NULL;
			}
		}
	}

	// メニュー
	if (type == TEXT_MENU || type == TEXT_ALL) {
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++) {
			if (m_pMenu[nCnt] != NULL) {
				m_pMenu[nCnt]->Uninit();
				m_pMenu[nCnt] = NULL;
			}
		}
	}
}