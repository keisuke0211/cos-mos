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
#include "../System/BG-Editor.h"

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
CCamera* CMode_Game::m_cameraUp = NULL;
CCamera* CMode_Game::m_cameraDown = NULL;
int CMode_Game::m_nStageIdx = 0;
int CMode_Game::m_nPlanetIdx = 0;

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::CMode_Game(void) {

	m_cameraUp   = NULL;
	m_cameraDown = NULL;
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::~CMode_Game(void) {

	m_cameraUp->Delete();
	m_cameraDown->Delete();

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL){
			m_Menu[nCnt]->Uninit();
			m_Menu[nCnt] = NULL;
		}
	}
}

//========================================
// 初期化処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Init(void) {
	CMode::Init();

	// 遷移設定
	RNLib::Transition().Open(CTransition::TYPE::FADE, 30);

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);

	Manager::BlockMgr()->Load();
	Manager::BlockMgr()->ShiningWaveCreate();
	BackGroundPut(Color{ 100,100,100,255 }, Color{ 100,100,100,255 });

	m_rocketparts = CRocketPartsUI::Create();

	if (s_pPlayer == NULL)
		s_pPlayer = CPlayer::Create();

	// 読込
	/* ブロック	*/Manager::BlockMgr()->Load();

	// ステージ生成
	Manager::StgEd()->StageLoad(m_nPlanetIdx, m_nStageIdx);

	char *pBgFile = Manager::StgEd()->GetBgFile();

	if (pBgFile != NULL) {
		CBGEditor::Load(pBgFile);
	}

	SetBGColor(m_BgColorUp);

	// 上下カメラの生成
	m_cameraUp = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight() * 0.5f));
	m_cameraDown = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight() * 0.5f));
	m_cameraUp->SetClipping(true);
	m_cameraUp->SetPosVAndPosR(Pos3D(0.0f, 0.0f, -40.0f), Pos3D(0.0f, 0.0f, 0.0f));
	m_cameraDown->SetClipping(true);
	m_cameraDown->SetPosVAndPosR(Pos3D(0.0f, 0.0f, -40.0f), Pos3D(0.0f, 0.0f, 0.0f));

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_Menu[nCnt] = NULL;
	}

	// 壁モデル読み込み
	m_wallModelIdx[0] = RNLib::Model().Load("data\\MODEL\\Wall_Left.x");
	m_wallModelIdx[1] = RNLib::Model().Load("data\\MODEL\\Wall_Right.x");

	m_BGMIdx = RNLib::Sound().Load("data\\SOUND\\BGM\\STAGE1.wav");

	play = RNLib::Sound().Play(m_BGMIdx, CSound::CATEGORY::BGM, true);
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Uninit(void) {
	CMode::Uninit();

	play->Delete();

	if (s_pPlayer != NULL)	{
		s_pPlayer->Uninit();
		delete s_pPlayer;
		s_pPlayer = NULL;
	}

	Manager::BlockMgr()->ReleaseAll();
	Manager::BGMgr()->ReleaseAll();

	if (m_rocketparts != NULL) {
		m_rocketparts->Uninit();
		delete m_rocketparts;
		m_rocketparts = NULL;
	}
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Update(void) {
	CMode::Update();

	{// [[[ カメラ制御 ]]]
		Pos3D pos = (s_pPlayer->GetInfo(0)->pos + s_pPlayer->GetInfo(1)->pos) * 0.5f;
		pos.x *= 0.25f;
		pos.y = 0.0f;
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::GetMainCamera()->GetPosV(), pos);
	}

	{// [[[ 上下カメラ描画 ]]]
		const Pos2D windowCenterPos   = RNLib::Window().GetCenterPos();
		const float windowWidth       = RNLib::Window().GetWidth();
		const float windowHeight      = RNLib::Window().GetHeight();
		const float windowHeightHalf  = windowHeight * 0.5f;
		const float windowHeightHalf2 = windowHeightHalf * 0.5f;

		// 背景のカラーを設定
		m_cameraUp->SetBGCol(m_BgColorUp);
		m_cameraDown->SetBGCol(m_BgColorDown);

		// 上
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y - windowHeightHalf2, 0.0f), 0.0f)
			->SetTex_Camera(m_cameraUp)
			->SetSize(windowWidth, windowHeightHalf)
			->SetPriority(-1);

		// 下
		RNLib::Polygon2D().Put(Pos3D(windowCenterPos.x, windowCenterPos.y + windowHeightHalf2, 0.0f), 0.0f)
			->SetTex_Camera(m_cameraDown)
			->SetSize(windowWidth, windowHeightHalf)
			->SetPriority(-1);
	}

	// [[[ 壁モデル描画 ]]]
	RNLib::Model().Put(Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D, m_wallModelIdx[0]);
	RNLib::Model().Put(Pos3D(CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D, m_wallModelIdx[1]);

	m_rocketparts->Update();

	if (m_state != (int)STATE::PAUSE)
	{
		if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START))
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

			RNLib::Polygon2D().Put(D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, 0.0f), 0.0f, false)
				->SetSize(RNLib::Window().GetCenterX() * 2, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 0,0,0,120 })
				->SetPriority(0);

			RNLib::Polygon2D().Put(D3DXVECTOR3(m_Pause.LeftPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
				->SetSize(450.0f, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 150,150,150,150 })
				->SetPriority(0);

			if (m_Pause.bMenu && !m_Pause.bClose) {
				PauseMenu();
				PauseSelect();
			}
			PauseAnime();

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
	m_Pause.LeftPos = D3DXVECTOR3(-340.0f, 0.0f, 0.0f);
	m_Pause.RightPos = D3DXVECTOR3(2000.0f, 0.0f, 0.0f);
	m_Pause.LeftTargetPos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
	m_Pause.RightTargetPos = D3DXVECTOR3(850.0f, 0.0f, 0.0f);
	m_Pause.nCntAnime = 0;
	m_Pause.nSelect = 0;
	m_Pause.bMenu = false;
	m_Pause.bClose = false;

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	m_Menu[0] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Pause.LeftPos.x, 150.0f, 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont);

	m_Menu[1] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Pause.LeftPos.x, 250.0f, 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont);

	m_Menu[2] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Pause.LeftPos.x, 350.0f, 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont);	

	m_Menu[3] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Pause.LeftPos.x, 450.0f, 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont);

	m_Menu[4] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Pause.LeftPos.x, 550.0f, 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont);
}

//========================================
// ポーズメニューの処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseMenu(void)
{
	// 選択・非選択
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL) {
			if (nCnt == m_Pause.nSelect){ 
				m_Menu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
			}
			else{
				m_Menu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::UP))
	{
		m_Pause.nSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		m_Pause.nSelect++;
	}
	// ループ制御
	IntLoopControl(&m_Pause.nSelect, MENU_MAX, 0);
}

//========================================
// ポーズアニメーションの処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseAnime(void)
{
	// 左画面のアニメーション
	if (!m_Pause.bMenu)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Pause.LeftTargetPos.x - m_Pause.LeftPos.x) * 0.3f;

		m_Pause.LeftPos.x += move.x;
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{
			if (m_Menu[nCnt] != NULL)
			{
				m_Menu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
			}
		}

		if (++m_Pause.nCntAnime == PAUSE_LEFT_ANIME) {
			m_Pause.LeftPos = m_Pause.LeftTargetPos;
			m_Pause.nCntAnime = 0;
			m_Pause.bMenu = true;

			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
			m_Menu[0]->Regeneration("続ける", CFont::FONT_ROND_B, &pFont);
			m_Menu[1]->Regeneration("やり直す", CFont::FONT_ROND_B, &pFont);
			m_Menu[2]->Regeneration("選択画面", CFont::FONT_ROND_B, &pFont);
			m_Menu[3]->Regeneration("操作方法", CFont::FONT_ROND_B, &pFont);
			m_Menu[4]->Regeneration("設定", CFont::FONT_ROND_B, &pFont);
		}
	}

	// 閉じるアニメーション
	if (m_Pause.bClose)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Pause.LeftTargetPos.x - m_Pause.LeftPos.x) * 0.3f;

		m_Pause.LeftPos.x += move.x;
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{
			if (m_Menu[nCnt] != NULL)
			{
				m_Menu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
			}
		}

		if (++m_Pause.nCntAnime == PAUSE_LEFT_ANIME) {
			m_Pause.LeftPos = m_Pause.LeftTargetPos;

			if (RNLib::Transition().GetState() == CTransition::STATE::NONE)
			{
				SetState((int)STATE::NONE);
			}
		}
	}
}

//========================================
// ポーズ選択の処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseSelect(void)
{
	if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START) && !m_Pause.bClose)
	{
		m_Pause.bClose = true;
	}

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && !m_Pause.bClose)
	{
		switch (m_Pause.nSelect)
		{
		case MENU_RESUME:
			m_Pause.bClose = true;
			break;
		case MENU_RESET:
			Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);
			m_Pause.bClose = true;
			break;
		case MENU_SELECT:
			Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
			CMode_Title::SetSelect(true);
			break;
		}
	}

	if(m_Pause.bClose){ 
		m_Pause.LeftTargetPos *= -1;
		m_Pause.RightTargetPos *= -1;
		m_Pause.nCntAnime = 0;

		ProcessState(PROCESS::UNINIT);
	}
}