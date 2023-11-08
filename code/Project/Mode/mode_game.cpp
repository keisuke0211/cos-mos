//========================================
// 
// モード:ゲームの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../main.h"
#include "../Character/player.h"
#include "../System/StageEditor.h"
#include "../System/words/font-text.h"
#include "../Mode/mode_title.h"
#include "../UI/rocket-parts.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Gameクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
static const int s_PlanetMaxSummon = 8;		// 出現する位置の最大数
static const int s_StarMaxSummon = 10;		// 出現する位置の最大数
static const D3DXVECTOR3 s_PlanetSummonPos[s_PlanetMaxSummon] = {	// 惑星の出現する位置
	D3DXVECTOR3(-300.0f, 300.0f, 300.0f),
	D3DXVECTOR3(0.0f,	 200.0f, 300.0f),
	D3DXVECTOR3(200.0f,  300.0f, 300.0f),
	D3DXVECTOR3(-100.0f, 100.0f,   300.0f),
	D3DXVECTOR3(100.0f,	 100.0f,   300.0f),
	D3DXVECTOR3(-300.0f, -200.0f,300.0f),
	D3DXVECTOR3(0.0f,	 -200.0f,300.0f),
	D3DXVECTOR3(100.0f,  -300.0f,300.0f),
};
static const D3DXVECTOR3 s_StarSummonPos[s_StarMaxSummon] = {	// 星の出現する位置
	D3DXVECTOR3(-300.0f, 300.0f, 500.0f),
	D3DXVECTOR3(0.0f,	 200.0f, 500.0f),
	D3DXVECTOR3(200.0f,  300.0f, 500.0f),
	D3DXVECTOR3(-100.0f, 100.0f, 500.0f),
	D3DXVECTOR3(100.0f,	 100.0f, 500.0f),
	D3DXVECTOR3(-300.0f, 200.0f,500.0f),
	D3DXVECTOR3(0.0f,	 -200.0f,500.0f),
	D3DXVECTOR3(-400.0f, -300.0f,500.0f),
	D3DXVECTOR3(400.0f,  -100.0f ,500.0f),
	D3DXVECTOR3(500.0f,  0.0f ,500.0f),
};
Color CMode_Game::m_BgColorUp = INITCOLOR;
Color CMode_Game::m_BgColorDown = INITCOLOR;
CPlayer *CMode_Game::s_pPlayer = NULL;
CPlayer* CMode_Game::GetPlayer(void) { return s_pPlayer; }
int CMode_Game::m_nStageIdx = 0;
int CMode_Game::m_nPlanetIdx = 0;

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::CMode_Game(void) {

}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::~CMode_Game(void) {

}

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Init(void) {
	CMode::Init();

	// 遷移設定
	RNLib::Transition().Set(CTransition::STATE::OPEN, CTransition::TYPE::FADE);

	// カメラの視点/注視点を設定
	RNLib::Camera3D().SetGeometryInfo(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);

	Manager::BlockMgr()->Load();
	Manager::BlockMgr()->ShiningWaveCreate();
	Manager::BlockMgr()->MeteorGeneratorCreate(D3DXVECTOR3(-200.0f, 100.0f, 0.0f), D3DXVECTOR3(0.5f, 0.0f, 0.0f),120);

	BackGroundPut(Color{ 100,100,100,255 }, Color{ 100,100,100,255 });

	m_rocketparts = new CRocketPartsUI;

	m_rocketparts = CRocketPartsUI::Create();

	if (s_pPlayer == NULL)
		s_pPlayer = CPlayer::Create();

	// 読込
	/* ブロック	*/Manager::BlockMgr()->Load();

	// ステージ生成
	Manager::StgEd()->StageLoad(m_nPlanetIdx,m_nStageIdx);

	SetBGColor(m_BgColorUp);

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_Menu[nCnt] = NULL;
	}
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Uninit(void) {
	CMode::Uninit();

	if (s_pPlayer != NULL)	{
		s_pPlayer->Uninit();
		delete s_pPlayer;
		s_pPlayer = NULL;
	}

	Manager::BlockMgr()->ReleaseAll();

	m_rocketparts->Uninit();
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Update(void) {
	CMode::Update();

	m_rocketparts->Update();

	if (m_state != (int)STATE::PAUSE)
	{
		bool isTest = RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START);

		if (isTest)
		{
			SetState((int)STATE::PAUSE);
		}

		if (s_pPlayer != NULL)
			s_pPlayer->Update();

		if (RNLib::Input().GetKeyTrigger(DIK_0) && RNLib::Transition().GetState() == CTransition::STATE::NONE)
		{
			int planet = Manager::StgEd()->GetPlanetIdx();
			int stage = Manager::StgEd()->GetType()[planet].nStageIdx;
			Manager::StgEd()->SwapStage(stage + 1);
		}
	}

	// 背景(仮)
	{
		float width = RNLib::Window().GetWidth();
		float height = RNLib::Window().GetHeight();
		RNLib::Polygon3D().Put(D3DXVECTOR3(0.0f, -height*0.25f, 400.0f), INITD3DXVECTOR3)
			->SetLighting(false)
			->SetCol(m_BgColorDown)
			->SetSize(width * 2.0f, height * 0.5f)
			->SetPriority(-2)
			->SetZTest(false);
	}
}

//========================================
// 更新処理(状態)
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::ProcessState(const PROCESS process) {
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
	case STATE::PAUSE: {
		switch (process) {
			// [[[ 初期処理 ]]]
		case PROCESS::INIT: {
			PauseCreate();
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {
			for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
			{
				if (m_Menu[nCnt] != NULL)
				{
					m_Menu[nCnt]->Uninit();
					m_Menu[nCnt] = NULL;
				}
			}
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {
			PauseMenu();
			PauseSelect();
		}break;
		}
	}break;
	}
}

//========================================
// 背景の生成処理
// Author:KOMURO HIROMU
//========================================
void CMode_Game::BackGroundPut(Color mincol, Color addcol) {

	D3DXVECTOR3 Planpos,Starpos,rot;
	Color col;
	CPlanet::STAR_TYPE type;

	for (int nCntPut = 0; nCntPut < s_PlanetMaxSummon; nCntPut++)
	{
		// 惑星の設定
		Planpos = s_PlanetSummonPos[nCntPut];	// 出現する位置の設定
		Planpos += D3DXVECTOR3(rand() % 100 - 100, rand() % 100 - 100, rand() % 150 - 50);	// 位置の設定

		type = (CPlanet::STAR_TYPE)(rand() % (int)CPlanet::STAR_TYPE::MAX);	// 種類の設定
		col = Color{ mincol.r + rand() % addcol.r,mincol.g + rand() % addcol.g,mincol.b + rand() % addcol.b,255 };	// 色の設定
		rot = D3DXVECTOR3(rand() % 6 - 3, rand() % 6 - 3, rand() % 6 - 3);	// 向きの設定
		//Manager::BlockMgr()->PlanetCreate(Planpos, rot, type, col);	// 惑星の生成
	}

	for (int nCntPut = 0; nCntPut < s_StarMaxSummon; nCntPut++)
	{
		// 星の設定
		Starpos = s_StarSummonPos[nCntPut];	// 出現する位置の設定
		Starpos += D3DXVECTOR3(rand() % 50 - 50, rand() % 50 - 50, 0.0f);	// 位置の設定

		//Manager::BlockMgr()->StarCreate(Starpos, INITD3DXVECTOR3);	// 星の生成
	}
}

//========================================
// ポーズ生成の処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseCreate(void)
{
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),45.0f,1,1,-1, };

	m_Menu[0] = CFontText::Create(CFontText::BOX_NORMAL_RECT,
		D3DXVECTOR3(640.0f, 200.0f, 0.0f), D3DXVECTOR2(460.0f, 100.0f),
		"続ける", CFont::FONT_ROND_B, &pFont);

	m_Menu[1] = CFontText::Create(CFontText::BOX_NORMAL_RECT,
		D3DXVECTOR3(640.0f, 350.0f, 0.0f), D3DXVECTOR2(460.0f, 100.0f),
		"やり直す", CFont::FONT_ROND_B, &pFont);

	m_Menu[2] = CFontText::Create(CFontText::BOX_NORMAL_RECT,
		D3DXVECTOR3(640.0f, 500.0f, 0.0f), D3DXVECTOR2(460.0f, 100.0f),
		"選択画面に戻る", CFont::FONT_ROND_B, &pFont);
}

//========================================
// ポーズメニューの処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseMenu(void)
{
	// 色
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			if (nCnt == m_nSelect)
			{
				m_Menu[nCnt]->SetBoxColor(Color{ 0,255,0,255 });
			}
			else
			{
				m_Menu[nCnt]->SetBoxColor(INITCOLOR);
			}
		}
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::UP))
	{
		m_nSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		m_nSelect++;
	}

	// ループ制御
	IntLoopControl(&m_nSelect, MENU_MAX, 0);
}

//========================================
// ポーズ選択の処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseSelect(void)
{
	//if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START))
	//{
	//	SetState((int)STATE::NONE);
	//}

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && RNLib::Transition().GetState() == CTransition::STATE::NONE)
	{
		switch (m_nSelect)
		{
		case MENU_RESUME:
			SetState((int)STATE::NONE);
			break;
		case MENU_RESET:
			Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);
			break;
		case MENU_TITLE:
			Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
			CMode_Title::SetSelect(true);
			break;
		}

		ProcessState(PROCESS::UNINIT);
	}
}