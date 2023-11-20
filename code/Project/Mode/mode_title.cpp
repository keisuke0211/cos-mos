//========================================
// 
// モード:タイトル処理
// Author:KEISUKE OTONO
// 
//========================================
#include "../main.h"
#include "mode_title.h"
#include "mode_game.h"
#include "../System/words/words.h"
#include "../System/words/font-text.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Titleクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
const char* CMode_Title::TEXT_FILE = "data\\GAMEDATA\\TITLE\\TitleFile.txt";
bool CMode_Title::s_bStageSelect = true;

//========================================
// コンストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Title::CMode_Title(void) {

	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++)
	{
		m_BgPos[nCnt] = INITD3DXVECTOR3;
		m_TexIdx[nCnt] = 0;
	}

	Title = TITLE_TITLE;
	m_nSelect = 0;
	m_nOldSelect = 0;
	m_nPlanetIdx = 0;
	m_nOldnPlanet = 0;
	m_PlanetAngle = 0.0f;
	m_nSelect = 0;
	m_nOldSelect = 0;
	m_PlanetType = NULL;
	m_bBackMode = false;

	m_Anime.pOperation = NULL;
	m_Anime.pSetting = NULL;
}

//========================================
// デストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Title::~CMode_Title(void) {

	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		if (m_TITLE[nCnt] != NULL)
		{
			m_TITLE[nCnt]->Uninit();
			m_TITLE[nCnt] = NULL;
		}
		if (m_TitleShadow[nCnt] != NULL)
		{
			m_TitleShadow[nCnt]->Uninit();
			m_TitleShadow[nCnt] = NULL;
		}
	}

	if (m_PlanetType != NULL)
	{
		delete[] m_PlanetType;
		m_PlanetType = NULL;
	}
}

//========================================
// 初期化処理
// Author:KEISUKE OTONO
//========================================
void CMode_Title::Init(void) {
	CMode::Init();

	// テキストの初期化
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)m_pMenu[nCnt] = { NULL };
	for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++)m_pSubMenu[nCnt] = { NULL };
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++) {
		m_bMove[nCnt] = false; m_TITLE[nCnt] = NULL; m_TitleShadow[nCnt] = NULL;
	}

	// テキスト読込
	TextLoad();

	// ステージ読込
	StageLoad();

	if (s_bStageSelect)
	{
		SwapMode(TITLE_SELECT);
		s_bStageSelect = false;
	}
	else if(!s_bStageSelect)
		// モード設定
		SwapMode(TITLE_TITLE);

	

	// テクスチャ
	m_BgPos[0] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, -100.0f);
	m_BgPos[1] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1460.0f, 0.0f);
	m_BgPos[2] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1460.0f, 0.0f);
	m_BgPos[3] = D3DXVECTOR3(50, 140.0f, 0.0f);
	m_nSelect = 0;

	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Space.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Planet\\blue.png");
	m_TexIdx[2] = RNLib::Texture().Load("data\\TEXTURE\\Planet\\Orange.png");
	m_TexIdx[3] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_00.png");

	// 遷移設定
	RNLib::Transition().Open(CTransition::TYPE::FADE, 30);

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);

	// 背景色変更
	SetBGColor(Color{ 200,0,0,255 });
}

//========================================
// 終了処理
// Author:KEISUKE OTONO
//========================================
void CMode_Title::Uninit(void) {
	CMode::Uninit();

	TextRelease(TEXT_ALL);
}

//========================================
// 更新処理
// Author:KEISUKE OTONO
//========================================
void CMode_Title::Update(void) {
	CMode::Update();

	// 各モードの処理
	if (Title == TITLE_TITLE)
		TitleAnime();
	else if (Title == TITLE_MENU) {
		if (!m_Anime.bClose)MenuSelect();
		MenuAnime();
	}
	else if (Title == TITLE_SELECT)
		StageSelect();
	else if (Title == TITLE_NEXT)
		return;

	RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, m_BgPos[0], 0.0f, false)
		->SetSize(1280.0f, 720.0f)
		->SetCol(Color{ 255,255,255,255 })
		->SetTex(m_TexIdx[0]);

	// メニューの背景
	if (Title == TITLE_MENU){
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Anime.LeftPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
			->SetSize(450.0f, RNLib::Window().GetCenterY() * 2)
			->SetCol(Color{ 150,150,150,150 });

		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Anime.RightPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
			->SetSize(630.0f, RNLib::Window().GetCenterY() * 2)
			->SetCol(Color{ 150,150,150,150 });

		if (m_Anime.nSelect == MENU_CONTROLLER || m_Anime.nSelect == MENU_SERRING)
		{
			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Anime.RightPos.x, 400.0f, 100.0f), 0.0f, false)
				->SetSize(500.0f, 600.0f)
				->SetTex(m_Anime.BoxTex);
		}
	}

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && RNLib::Transition().GetState() == CTransition::STATE::NONE)
	{
		switch (Title)
		{
		case TITLE_OUTSET:
		{
			SwapMode(TITLE_MENU_ANIME);
			TextRelease(TEXT_TITLE);
		}
		break;
		case TITLE_MENU:
		{
			switch (m_nSelect)
			{
			case MENU_GAME:
				m_Anime.bClose = true;
				m_Anime.LeftTargetPos *= -1;
				m_Anime.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
				m_Anime.nCntLeftAnime = 0;
				break;
			case MENU_SERRING:
				break;
			case MENU_END:
				//ゲームの終了
				PostQuitMessage(0);
				break;
			}
		}
		break;
		case TITLE_SELECT:
		{
			SwapMode(TITLE_NEXT);
			CMode_Game::SetStage(m_nPlanetIdx,m_nSelect);
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

//========================================
// 更新処理(状態)
// Author:KEISUKE OTONO
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
// タイトルアニメーション
// Author:KEISUKE OTONO
//========================================
void CMode_Title::TitleAnime(void)
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
// メニュー生成
// Author:KEISUKE OTONO
//========================================
void CMode_Title::MenuCreate(void)
{
	m_Anime.LeftPos = D3DXVECTOR3(-340.0f, 0.0f, 0.0f);
	m_Anime.RightPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
	m_Anime.LeftTargetPos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	m_Anime.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Anime.nCntLeftAnime = 0;
	m_Anime.nCntRightAnime = 0;
	m_Anime.nSelect = 0;
	m_Anime.bRightMove = false;
	m_Anime.bRightDisp = false;
	m_Anime.nRightTextType = 0;
	m_Anime.bMenu = false;
	m_Anime.bClose = false;

	m_Anime.BoxTex = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox10.png");

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	for (int nText = MENU_GAME; nText < MENU_MAX; nText++) {
		m_pMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
			D3DXVECTOR3(m_Anime.LeftPos.x - 20, 150.0f + (100.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
			"", CFont::FONT_ROND_B, &pFont, false, true, &pShadow);
	}
}

//========================================
// サブテキストの生成
// Author:KEISUKE OTONO
//========================================
void CMode_Title::SubTextCreate(void)
{
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	if (m_pSubMenu != NULL)
		TextRelease(TEXT_RIGHT);

	if (m_Anime.nSelect == MENU_CONTROLLER) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_BLUE, D3DXVECTOR3(m_Anime.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
			"", CFont::FONT_ROND_B, &pFont);

		for (int nText = 1; nText < m_Anime.OperationMax; nText++) {
			m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Anime.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont, false, false);
		}
	}
	else if (m_Anime.nSelect == MENU_SERRING) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
		CFontText::BOX_NORMAL_BLUE, D3DXVECTOR3(m_Anime.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
		"", CFont::FONT_ROND_B, &pFont);

		for (int nText = 1; nText < m_Anime.SettingMax; nText++) {
		m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
		D3DXVECTOR3(m_Anime.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
		"", CFont::FONT_ROND_B, &pFont, false, false);
		}
	}

	m_Anime.nRightTextType = m_Anime.nSelect;
}

//========================================
// メニュー演出
// Author:KEISUKE OTONO
//========================================
void CMode_Title::MenuAnime(void)
{
	// 左画面のアニメーション
	if (!m_Anime.bMenu || m_Anime.bClose)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Anime.LeftTargetPos.x - m_Anime.LeftPos.x) * 0.3f;

		m_Anime.LeftPos.x += move.x;
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)		{
			if (m_pMenu[nCnt] != NULL)
				m_pMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
		}

		if (++m_Anime.nCntLeftAnime == PAUSE_LEFT_ANIME) {
			m_Anime.LeftPos = m_Anime.LeftTargetPos;
			m_Anime.nCntLeftAnime = 0;

			if (!m_Anime.bClose) {
				m_Anime.bMenu = true;

				FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
				FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

				m_pMenu[0]->Regeneration("ゲーム", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[1]->Regeneration("操作方法", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[2]->Regeneration("設定", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[3]->Regeneration("やめる", CFont::FONT_ROND_B, &pFont, &pShadow);
			}
			else if (m_Anime.bClose) {

				if (m_bBackMode)
					SwapMode(TITLE_OUTSET);
				else if (!m_bBackMode){
					TextRelease(TEXT_ALL);
					SwapMode(TITLE_SELECT);
				}
			}
		}
	}

	// 右画面のアニメーション
	if (m_Anime.bRightMove || (m_Anime.bClose && m_Anime.bRightDisp))
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Anime.RightTargetPos.x - m_Anime.RightPos.x) * 0.3f;

		m_Anime.RightPos.x += move.x;
		for (int nCnt = 0; nCnt < INPUT_MAX; nCnt++) {
			if (m_pSubMenu[nCnt] != NULL) {
				m_pSubMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
			}
		}

		if (++m_Anime.nCntRightAnime == PAUSE_RIGHT_ANIME) {
			m_Anime.RightPos = m_Anime.RightTargetPos;
			m_Anime.bRightMove = false;
			m_Anime.nCntRightAnime = 0;

			if (!m_Anime.bRightDisp)
			{
				m_Anime.bRightDisp = true;
				m_Anime.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
				FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
				FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

				// テキストの再生成
				if (m_Anime.nSelect == MENU_CONTROLLER) {
					for (int nText = 0; nText < m_Anime.OperationMax; nText++) {
						if (m_pSubMenu[nText] != NULL)
							m_pSubMenu[nText]->Regeneration(m_Anime.pOperation[nText].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
					}
				}
				if (m_Anime.nSelect == MENU_SERRING) {
					for (int nText = 0; nText < m_Anime.SettingMax; nText++) {
						if (m_pSubMenu[nText] != NULL)
							m_pSubMenu[nText]->Regeneration(m_Anime.pSetting[nText].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
					}
				}
			}
			else if (m_Anime.bRightDisp && !m_Anime.bClose) {
				m_Anime.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
				m_Anime.bRightDisp = false;
				TextRelease(TEXT_RIGHT);
			}
		}
	}
}

//========================================
// メニュー
// Author:KEISUKE OTONO
//========================================
void CMode_Title::MenuSelect(void)
{
	// 選択・非選択
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_pMenu[nCnt] != NULL) {
			if (nCnt == m_Anime.nSelect) {
				m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
			}
			else {
				m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK))
	{
		m_bBackMode = true;
		m_Anime.bClose = true;
		m_Anime.LeftTargetPos *= -1;
		m_Anime.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
		m_Anime.nCntLeftAnime = 0;
		return;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::UP))
	{
		m_Anime.nSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		m_Anime.nSelect++;
	}

	// アニメーション
	if ((m_Anime.nSelect == MENU_CONTROLLER || m_Anime.nSelect == MENU_SERRING ) && !m_Anime.bRightMove && !m_Anime.bRightDisp) {

		m_Anime.bRightMove = true;
		SubTextCreate();
	}
	else if (m_Anime.nSelect != m_Anime.nRightTextType && !m_Anime.bRightMove && m_Anime.bRightDisp) {

		TextRelease(TEXT_RIGHT);
		m_Anime.bRightMove = true;
	}
	// ループ制御
	IntLoopControl(&m_Anime.nSelect, MENU_MAX, 0);
}

//========================================
// ステージ選択の生成
// Author:KEISUKE OTONO
//========================================
void CMode_Title::StageLoad(void)
{
	Manager::StgEd()->Uninit();
	Manager::StgEd()->FileLoad();
	int nPlanetMax = Manager::StgEd()->GetPlanetMax();

	m_PlanetType = new PlanetType[nPlanetMax];
	m_PlanetAngle = 0.0f;

	for (int nCnt = 0; nCnt < nPlanetMax; nCnt++)
	{
		char *aTexFile = Manager::StgEd()->GetType()[nCnt].aTexFile;
		char *aStgName = Manager::StgEd()->GetType()[nCnt].aName;

		m_PlanetType[nCnt].nModel = RNLib::Model().Load(aTexFile);
		sprintf(m_PlanetType[nCnt].Text, aStgName);
	}
}

//========================================
// ステージ選択
// Author:KEISUKE OTONO
//========================================
void CMode_Title::StageSelect(void)
{
	int nPlanetMax = Manager::StgEd()->GetPlanetMax();
	int nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
	int nChoiceTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\mark_smiley_000.png");
	int nNoChoiceTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Circle_005.png");
	int nPrevTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_01.png");
	int nNextTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_00.png");

	int nTexIdx = 0;

	// 惑星
	RNLib::Model().Put(PRIORITY_OBJECT, m_PlanetType[m_nPlanetIdx].nModel, D3DXVECTOR3(0.0f, -4.0f, 50.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), false)
		->SetOutLine(true);

	// 矢印
	if ((m_nPlanetIdx == 0 && m_nSelect != 0) || (m_nPlanetIdx != 0))
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(400.0f, 550.0f, 0.0), 0.0f, false)
		->SetSize(100.0f, 100.0f)
		->SetCol(Color{ 50,255,0,255 })
		->SetTex(nPrevTex);

	if ((m_nPlanetIdx != nPlanetMax-1) || (m_nPlanetIdx == nPlanetMax-1 && m_nSelect != nStageMax-1))
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(880.0f, 550.0f, 0.0), 0.0f, false)
		->SetSize(100.0f, 100.0f)
		->SetCol(Color{ 50,255,0,255 })
		->SetTex(nNextTex);

	// 選択アイコン
	for (int nCnt = 0; nCnt < nStageMax; nCnt++)
	{
		if (nCnt == m_nSelect)
		{
			nTexIdx = nChoiceTex;
		}
		else
		{
			nTexIdx = nNoChoiceTex;
		}
		D3DXVECTOR3 pos = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 680,1.0f);
		pos.x += ((nStageMax * -0.5f) + nCnt + 0.5f) * 50;

		RNLib::Polygon2D().Put(PRIORITY_UI, pos, 0.0f, false)
			->SetSize(40.0f, 40.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(nTexIdx);
	}

	bool bInput = false;
	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK))
	{
		TextRelease(TEXT_MENU);
		SwapMode(TITLE_MENU_ANIME);
		return;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT))
	{
		m_nSelect--;
		bInput = true;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT))
	{
		m_nSelect++;
		bInput = true;
	}

	if (bInput)
	{
		if (m_nSelect < 0 && m_nPlanetIdx != 0)
		{
			m_nPlanetIdx--;
			nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
			m_nSelect = nStageMax - 1;
			
		}
		else if (m_nSelect >= nStageMax && m_nPlanetIdx != nPlanetMax-1)
		{
			m_nPlanetIdx++;
			m_nSelect = 0;
			nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
		}

		IntControl(&m_nSelect, nStageMax - 1, 0);

		if (m_nSelect != m_nOldSelect)
		{
			m_nOldSelect = m_nSelect;

			m_pMenu[1]->Uninit();
			m_pMenu[1] = NULL;
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };
			m_pMenu[1] = CFontText::Create(
				CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f),
				Manager::StgEd()->GetType()[m_nPlanetIdx].StageType[m_nSelect].aName, CFont::FONT_ROND_B, &pFont);
		}
		if (m_nPlanetIdx != m_nOldnPlanet)
		{
			m_nOldnPlanet = m_nPlanetIdx;
			m_PlanetAngle = 0.0f;

			m_pMenu[0]->Uninit();
			m_pMenu[0] = NULL;
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };
			m_pMenu[0] = CFontText::Create(
				CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(360.0f, 70.0f),
				m_PlanetType[m_nPlanetIdx].Text, CFont::FONT_ROND_B, &pFont,true);
		}
	}

}

//========================================
// モード切替
// Author:KEISUKE OTONO
//========================================
void CMode_Title::SwapMode(TITLE aTitle)
{
	Title = aTitle;

	switch (aTitle)
	{
	case CMode_Title::TITLE_TITLE:
	{
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
	}
		break;
	case CMode_Title::TITLE_OUTSET:
	{
		TextRelease(TEXT_MENU);
		TextRelease(TEXT_RIGHT);

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
		MenuCreate();
		MenuAnime();

		Title = TITLE_MENU;
	}
		break;
	case CMode_Title::TITLE_SELECT:
	{
		m_nSelect = 0;
		m_nOldSelect = 0;
		m_nPlanetIdx = 0;
		m_nOldnPlanet = 0;

		s_bStageSelect = false;

		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };// 45
		m_pMenu[0] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(360.0f, 70.0f),
			m_PlanetType[0].Text, CFont::FONT_ROND_B, &pFont);

		m_pMenu[1] = CFontText::Create(
			CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f),
			Manager::StgEd()->GetType()[0].StageType[0].aName, CFont::FONT_ROND_B, &pFont);
	}
		break;
	case CMode_Title::TITLE_NEXT:
		TextRelease(TEXT_ALL);
		break;
	}
}

//========================================
// テキスト読込
// Author:KEISUKE OTONO
//========================================
void CMode_Title::TextLoad(void)
{
	int nCntPlanet = 0;
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(TEXT_FILE, "r");

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

		if (!strcmp(aDataSearch, "SetOperation"))
		{
			int nText = 0;
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索
				if (!strcmp(aDataSearch, "EndOperation")){
					break;
				}
				if (!strcmp(aDataSearch, "TextMax")){
					int nMax = -1;

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &nMax);

					if (nMax <= 0)
						nMax = 0;

					m_Anime.OperationMax = nMax;
					m_Anime.pOperation = new Operation[nMax];
					assert(m_Anime.pOperation != NULL);
				}
				if (!strcmp(aDataSearch, "Text")){
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Anime.pOperation[nText]);
					nText++;
				}
			}
		}
		else if (!strcmp(aDataSearch, "SetSetting"))
		{
			int nText = 0;
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索
				if (!strcmp(aDataSearch, "EndSetting")) {
					break;
				}
				if (!strcmp(aDataSearch, "TextMax")) {
					int nMax = -1;

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &nMax);

					if (nMax <= 0)
						nMax = 0;

					m_Anime.SettingMax = nMax;
					m_Anime.pSetting = new Setting[nMax];
					assert(m_Anime.pSetting != NULL);
				}
				if (!strcmp(aDataSearch, "Text")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Anime.pSetting[nText]);
					nText++;
				}
			}
		}
	}
}

//========================================
// テキスト削除
// Author:KEISUKE OTONO
//========================================
void CMode_Title::TextRelease(TEXT type)
{
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

	// 操作方法と設定
	if (type == TEXT_RIGHT || type == TEXT_ALL) {
		for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++) {
			if (m_pSubMenu[nCnt] != NULL) {
				m_pSubMenu[nCnt]->Uninit();
				m_pSubMenu[nCnt] = NULL;
			}
		}
	}
}