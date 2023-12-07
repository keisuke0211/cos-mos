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
#include "../System/words/words.h"
#include "../System/words/font-text.h"
#include "../resource.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Titleクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
const D3DXVECTOR3 SELECTBOX = D3DXVECTOR3(-30.0f, -10.0f, -130.0f);
const D3DXVECTOR3 UNSELECTBOX = D3DXVECTOR3(-30.0f, -20.0f, -120.0f);
const D3DXVECTOR3 NUMPOSSELBOX = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
const char* CMode_Title::TEXT_FILE = "data\\GAMEDATA\\TITLE\\MenuFile.txt";
bool CMode_Title::s_bStageSelect = false;

//========================================
// コンストラクタ
//========================================
CMode_Title::CMode_Title(void) {

	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++) {
		m_BgPos[nCnt] = INITD3DXVECTOR3;
		m_TexIdx[nCnt] = 0;
	}

	Title              = TITLE_TITLE;
	m_nSelect          = 0;
	m_nOldSelect       = 0;
	m_nPlanetIdx       = 0;
	m_nOldnPlanet      = 0;
	m_PlanetAngle      = 0.0f;
	m_nSelect          = 0;
	m_nOldSelect       = 0;
	m_PlanetType       = NULL;
	m_bBackMode        = false;
	m_RocketIdx        = RNLib::Model().Load("data\\MODEL\\Rocket_Body.x");
	m_SelIdx           = RNLib::Model().Load("data\\MODEL\\Select_Box.x");
	m_StgBoardIdx      = RNLib::Model().Load("data\\MODEL\\Stage_Board.x");
	m_Menu.pOperation  = NULL;
	m_Menu.pSetting    = NULL;
	m_Menu.bFullScreen = RNSettings::GetInfo().isFullScreen;

	{// 音量設定の初期化
		float BGM = RNLib::Sound().GetCategoryState(CSound::CATEGORY::BGM).settingVolume;
		float SE  = RNLib::Sound().GetCategoryState(CSound::CATEGORY::SE).settingVolume;
		m_Menu.nBGMVolume    = BGM * VOLUME_MSX;
		m_Menu.nSEVolume     = SE * VOLUME_MSX;
		m_Menu.nBGMOldVolume = BGM * VOLUME_MSX;
		m_Menu.nSEOldVolume  = SE * VOLUME_MSX;
	}
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

	// テキストの破棄
	TextRelease(TEXT_ALL);

	if (m_Menu.pOperation != NULL) {
		delete[] m_Menu.pOperation;
		m_Menu.pOperation = NULL;
	}

	if (m_Menu.pSetting != NULL) {
		delete[] m_Menu.pSetting;
		m_Menu.pSetting = NULL;
	}
}

//========================================
// 初期化処理
//========================================
void CMode_Title::Init(void) {
	CMode::Init();

	// 遷移設定
	RNLib::Transition().Open(CTransition::TYPE::FADE, 30);

	// テキストの初期化
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		m_pMenu[nCnt] = NULL;
	for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++) 
		m_pSubMenu[nCnt] = NULL;
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++) {
		m_bMove[nCnt] = false; 
		m_TITLE[nCnt] = m_TitleShadow[nCnt] = NULL;
	}

	// テキスト読込
	TextLoad();

	// ステージ読込
	CreateStageSelectInfo();

	if (s_bStageSelect) 
	{// ステージ選択時、
		SwapMode(TITLE_SELECT);
		s_bStageSelect = false;
	}
	else
	{// ステージ非選択時、
		// モード設定
		SwapMode(TITLE_TITLE);
	}

	// テクスチャ
	m_BgPos[0] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, -100.0f);
	m_BgPos[1] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1060, -50.0f);

	// 選択番号
	m_nSelect = 0;

	for (int nCnt = 1; nCnt < TEX_MAX; nCnt++) {
		m_TexIdx[nCnt] = 0;
	}

	// テクスチャの読み込み
	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Space.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Planet.png");
	m_TexIdx[2] = RNLib::Texture().Load("data\\TEXTURE\\StageSelect\\Number.png");

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 状態設定
	SetState((int)STATE::NONE);

	// 背景色変更
	SetBGColor(Color{ 200,0,0,255 });
}

//========================================
// 終了処理
//========================================
void CMode_Title::Uninit(void) {
	CMode::Uninit();
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

		FloatLoopControl(&m_PlanetAngle, D3DX_PI, -D3DX_PI);
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

	// メニューの背景
	if (Title == TITLE_MENU) {
		RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, D3DXVECTOR3(m_Menu.LeftPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
			->SetSize(450.0f, RNLib::Window().GetCenterY() * 2)
			->SetCol(Color{ 150,150,150,150 });

		if (m_Menu.RightPos.x < 1800)
		{
			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Menu.RightPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
				->SetSize(630.0f, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 150,150,150,150 });

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Menu.RightPos.x + 10, 400.0f, 100.0f), 0.0f, false)
				->SetSize(560.0f, 600.0f)
				->SetTex(m_Menu.BoxTex);
		}
	}

	// 各モードの処理
	if (Title == TITLE_TITLE)
		TextAnime();
	else if (Title == TITLE_MENU) {
		if (!m_Menu.bClose)MenuSelect();
		MenuAnime();
	}
	else if (Title == TITLE_SELECT)
		StageSelect();
	else if (Title == TITLE_NEXT)
		return;

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && RNLib::Transition().GetState() == CTransition::STATE::NONE)
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
		{
			if (!m_Menu.bSubMenu) {
				switch (m_Menu.nMaineSelect)
				{
				case MENU_GAME:
					if (!m_Menu.bClose) {
						m_Menu.bClose = true;
						m_Menu.LeftTargetPos *= -1;
						m_Menu.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
						m_Menu.nCntLeftAnime = 0;
					}
					break;
				case MENU_CONTROLLER:
					break;
				case MENU_SETTING:
					if (!m_Menu.bSubMenu) {
						m_Menu.bSubMenu = true;
						m_pMenu[m_Menu.nMaineSelect]->SetBoxColor(Color{ 155,155,155,255 });
					}
					break;
				case MENU_END:
					//ゲームの終了
					PostQuitMessage(0);
					break;
				}
			}
			else if (m_Menu.bSubMenu) {
				switch (m_Menu.nSubSelect)
				{
				case SETTING_SCREEN:
					if (m_Menu.nCntScrChg <= 0) {
						int nText = m_Menu.nSubSelect;
						m_Menu.bFullScreen = !m_Menu.bFullScreen;

						char data[TXT_MAX] = {};
						if (!m_Menu.bFullScreen)	sprintf(data, "%s ：OFF", m_Menu.pSetting[nText].Text);
						else if (m_Menu.bFullScreen)	sprintf(data, "%s ：ON", m_Menu.pSetting[nText].Text);

						FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
						FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };
						m_pSubMenu[nText]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

						m_Menu.nCntScrChg = 20;
					}
					
					break;
				case SETTING_BGM:
					break;
				case SETTING_SE:
					break;
				case SETTING_BACK:
					m_Menu.bSubMenu = false;
					m_pMenu[m_Menu.nMaineSelect]->SetBoxColor(INITCOLOR);
					m_pSubMenu[m_Menu.nSubSelect]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
					break;
				}

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
// メニュー生成
//========================================
void CMode_Title::MenuCreate(void)
{
	m_Menu.LeftPos = D3DXVECTOR3(-340.0f, 0.0f, 0.0f);
	m_Menu.RightPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
	m_Menu.LeftTargetPos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	m_Menu.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Menu.nCntLeftAnime = 0;
	m_Menu.nCntRightAnime = 0;
	m_Menu.nMaineSelect = 0;
	m_Menu.nMaineOldSelect = 0;
	m_Menu.nSubSelect = 1;
	m_Menu.bRightMove = false;
	m_Menu.bRightDisp = false;
	m_Menu.nRightCoolDown = COOLDOWN;
	m_Menu.bRightCoolDown = false;
	m_Menu.nRightTextType = 0;
	m_Menu.bMenu = false;
	m_Menu.bSubMenu = false;
	m_Menu.bClose = false;

	m_Menu.BoxTex = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox10.png");

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	for (int nText = MENU_GAME; nText < MENU_MAX; nText++) {
		m_pMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
			D3DXVECTOR3(m_Menu.LeftPos.x - 20, 150.0f + (100.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
			"", CFont::FONT_ROND_B, &pFont, false, true, &pShadow);
	}
}

//========================================
// サブテキストの生成
//========================================
void CMode_Title::SubTextCreate(void)
{
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	if (m_pSubMenu != NULL)
		TextRelease(TEXT_RIGHT);

	if (m_Menu.nMaineSelect == MENU_CONTROLLER) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Menu.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
			"", CFont::FONT_ROND_B, &pFont);

		for (int nText = 1; nText < m_Menu.OperationMax; nText++) {
			m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Menu.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont, false, false);
		}
	}
	else if (m_Menu.nMaineSelect == MENU_SETTING) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
		CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Menu.RightPos.x-210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),//-210
		"", CFont::FONT_ROND_B, &pFont);

		pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),30.0f,1,1,-1, };

		for (int nText = 1; nText < m_Menu.SettingMax; nText++) {

			D3DXVECTOR3 pos = INITD3DXVECTOR3;
			D3DXVECTOR2 size = INITD3DXVECTOR2;
			if (nText <= SETTING_BGM)
				pos = D3DXVECTOR3(m_Menu.RightPos.x, 100.0f + (80.0f * nText), 0.0f);
			else if (nText == SETTING_SE)
				pos = D3DXVECTOR3(m_Menu.RightPos.x, 100.0f + (160.0f * ((nText - 2) + 1)), 0.0f);
			else if (nText == SETTING_BACK)
				pos = D3DXVECTOR3(m_Menu.RightPos.x + 190.0f, 650.0f, 0.0f);//190
			else if (nText == SETTING_BGM_TEXT)
				pos = D3DXVECTOR3(m_Menu.RightPos.x + 150.0f, 100.0f + (80.0f * (nText - 2)), 0.0f);//150
			else if (nText == SETTING_SE_TEXT)
				pos = D3DXVECTOR3(m_Menu.RightPos.x + 150.0f, 100.0f + (80.0f * ((nText - 2) + 1)), 0.0f);//150



			if (nText > SETTING_BACK)
				size = D3DXVECTOR2(180.0f, 80.0f);
			else if (nText == SETTING_BACK)
				size = D3DXVECTOR2(100.0f, 80.0f);
			else
				size = D3DXVECTOR2(480.0f, 60.0f);

			m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				pos, size,
				"", CFont::FONT_ROND_B, &pFont, false, true);
		}
	}

	m_Menu.nRightTextType = m_Menu.nMaineSelect;
}

//========================================
// メニュー演出
//========================================
void CMode_Title::MenuAnime(void)
{
	// 左画面のアニメーション
	if (!m_Menu.bMenu || m_Menu.bClose)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Menu.LeftTargetPos.x - m_Menu.LeftPos.x) * 0.3f;

		m_Menu.LeftPos.x += move.x;
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)		{
			if (m_pMenu[nCnt] != NULL)
				m_pMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
		}

		if (++m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME) {
			m_Menu.LeftPos = m_Menu.LeftTargetPos;
			m_Menu.nCntLeftAnime = 0;

			if (!m_Menu.bClose) {
				m_Menu.bMenu = true;

				FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
				FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

				m_pMenu[0]->Regeneration("ゲーム", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[1]->Regeneration("操作方法", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[2]->Regeneration("設定", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[3]->Regeneration("やめる", CFont::FONT_ROND_B, &pFont, &pShadow);
			}
			else if (m_Menu.bClose) {

				if (m_bBackMode)
					SwapMode(TITLE_TITLE);
				else if (!m_bBackMode){
					TextRelease(TEXT_ALL);
					SwapMode(TITLE_SELECT);
				}
			}
		}
	}

	// 右画面のアニメーション
	if (m_Menu.bRightMove && m_Menu.bRightCoolDown) {
		if (--m_Menu.nRightCoolDown <= 0) {

			m_Menu.bRightCoolDown = false;
			m_Menu.nRightCoolDown = COOLDOWN;
			if (!m_Menu.bRightDisp){
				SubTextCreate();
			}
			else if (m_Menu.bRightDisp && !m_Menu.bClose) {
				TextRelease(TEXT_RIGHT);
			}
		}
	}
	else if (m_Menu.bRightMove && !m_Menu.bRightCoolDown || (m_Menu.bClose && m_Menu.bRightDisp))
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		if (m_Menu.nMaineSelect != MENU_CONTROLLER && m_Menu.nMaineSelect != MENU_SETTING && !m_Menu.bRightDisp) {
			TextRelease(TEXT_RIGHT);
			m_Menu.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
			move.x = (m_Menu.RightTargetPos.x - m_Menu.RightPos.x) * 0.3f;
			m_Menu.RightPos.x += move.x;

			if (++m_Menu.nCntRightAnime == PAUSE_RIGHT_ANIME) {
				m_Menu.RightPos = m_Menu.RightTargetPos;
				m_Menu.bRightMove = false;
				m_Menu.nCntRightAnime = 0;
				m_Menu.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
			}
		}
		else {
			move.x = (m_Menu.RightTargetPos.x - m_Menu.RightPos.x) * 0.3f;

			m_Menu.RightPos.x += move.x;

			int nTextMax = -1;
			if (m_Menu.nMaineSelect == MENU_CONTROLLER) nTextMax = m_Menu.OperationMax;
			else if (m_Menu.nMaineSelect == MENU_SETTING) nTextMax = m_Menu.SettingMax;

			for (int nCnt = 0; nCnt < nTextMax; nCnt++) {
				if (m_pSubMenu[nCnt] != NULL) {
					m_pSubMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
				}
			}

			if (++m_Menu.nCntRightAnime == PAUSE_RIGHT_ANIME) {
				m_Menu.RightPos = m_Menu.RightTargetPos;
				m_Menu.bRightMove = false;
				m_Menu.nCntRightAnime = 0;

				if (!m_Menu.bRightDisp)
				{
					m_Menu.bRightDisp = true;
					m_Menu.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
					FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
					FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

					// テキストの再生成
					if (m_Menu.nRightTextType == MENU_CONTROLLER) {
						for (int nText = 0; nText < m_Menu.OperationMax; nText++) {
							if (m_pSubMenu[nText] != NULL)
								m_pSubMenu[nText]->Regeneration(m_Menu.pOperation[nText].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
						}
					}
					if (m_Menu.nRightTextType == MENU_SETTING) {
						for (int nText = 0; nText < m_Menu.SettingMax; nText++) {
							if (m_pSubMenu[nText] != NULL) {

								char data[TXT_MAX];
								if (nText == SETTING_SCREEN) {
									if (!m_Menu.bFullScreen)	sprintf(data, "%s ：OFF", m_Menu.pSetting[nText].Text);
									else if (m_Menu.bFullScreen)	sprintf(data, "%s ：ON", m_Menu.pSetting[nText].Text);
								}
								else if (nText == SETTING_BGM_TEXT) {
									int nData = m_Menu.nBGMVolume * 5;
									sprintf(data, "%d%s", nData, m_Menu.pSetting[nText].Text);
								}
								else if (nText == SETTING_SE_TEXT) {
									int nData = m_Menu.nSEVolume * 5;
									sprintf(data, "%d%s", nData, m_Menu.pSetting[nText].Text);
								}
								else
									sprintf(data, "%s", m_Menu.pSetting[nText].Text);

								m_pSubMenu[nText]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);
							}
						}
					}
				}
				else if (m_Menu.bRightDisp && !m_Menu.bClose) {
					m_Menu.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
					m_Menu.bRightDisp = false;
					TextRelease(TEXT_RIGHT);
				}
			}
		}
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
// メニュー
//========================================
void CMode_Title::MenuSelect(void)
{
	if (m_Menu.nCntScrChg >= 0)
	{// 画面モード切り替えカウンターが0以上の時、
	 // 画面モード切り替えカウンターを減算
		m_Menu.nCntScrChg--;

		if (m_Menu.nCntScrChg == 0)
		{// 画面モード切り替えカウンターが0の時、
		 // ウインドウのモードを切り替える
			RNSettings::SetFulScreen(m_Menu.bFullScreen);
		}
	}

	// 選択・非選択
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (!m_Menu.bSubMenu) {
			if (m_pMenu[nCnt] != NULL) {
				if (nCnt == m_Menu.nMaineSelect)
					m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
				else
					m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}
	for (int nCnt = 1; nCnt < m_Menu.SettingMax; nCnt++)
	{
		if (m_Menu.bSubMenu) {
			if (m_pSubMenu[nCnt] != NULL) {
				if (nCnt == m_Menu.nSubSelect)
					m_pSubMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
				else
					m_pSubMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}

	// 矢印の表示

	if (m_Menu.nSubSelect == SETTING_BGM || m_Menu.nSubSelect == SETTING_SE) {

		int nPrevTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_01.png");
		int nNextTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_00.png");

		D3DXVECTOR2 pos = INITD3DXVECTOR2;	D3DXVECTOR2 TexSize = INITD3DXVECTOR2;	float TxtSize;	int Volume = 0;
		if (m_Menu.nSubSelect == SETTING_BGM) {
			pos = m_pSubMenu[SETTING_BGM_TEXT]->GetTexPos();
			TexSize = m_pSubMenu[SETTING_BGM_TEXT]->GetTexSize();
			TxtSize = m_pSubMenu[SETTING_BGM_TEXT]->GetTxtSize() * 1.5;
			Volume = m_Menu.nBGMVolume;
		}
		else if (m_Menu.nSubSelect == SETTING_SE) {
			pos = m_pSubMenu[SETTING_SE_TEXT]->GetTexPos();
			TexSize = m_pSubMenu[SETTING_SE_TEXT]->GetTexSize();
			TxtSize = m_pSubMenu[SETTING_BGM_TEXT]->GetTxtSize() * 1.5;
			Volume = m_Menu.nSEVolume;
		}



		if (Volume != 0)
			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(pos.x - (TexSize.x - TxtSize), pos.y, 0.0), 0.0f, false)
			->SetSize(80.0f, 100.0f)
			->SetCol(Color{ 50,255,0,255 })
			->SetTex(nPrevTex);

		if (Volume != VOLUME_MSX)
			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(pos.x + (TexSize.x - TxtSize), pos.y, 0.0), 0.0f, false)
			->SetSize(80.0f, 100.0f)
			->SetCol(Color{ 50,255,0,255 })
			->SetTex(nNextTex);
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK))
	{
		if (!m_Menu.bSubMenu) {
			m_bBackMode = true;
			m_Menu.bRightMove = true;
			m_Menu.bClose = true;
			m_Menu.LeftTargetPos *= -1;
			m_Menu.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
			m_Menu.nCntLeftAnime = 0;
			TextRelease(TEXT_ALL);
			return;
		}
		else if (m_Menu.bSubMenu) {
			m_Menu.bSubMenu = false;
			m_pMenu[m_Menu.nMaineSelect]->SetBoxColor(INITCOLOR);
			m_pSubMenu[m_Menu.nSubSelect]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::UP))
	{
		if (!m_Menu.bSubMenu)
			m_Menu.nMaineSelect--;
		else if (m_Menu.bSubMenu)
			m_Menu.nSubSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		if (!m_Menu.bSubMenu)
			m_Menu.nMaineSelect++;
		else if (m_Menu.bSubMenu)
			m_Menu.nSubSelect++;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT))
	{
		if (m_Menu.nSubSelect == SETTING_BGM) {
			m_Menu.nBGMVolume--;
		}
		else if (m_Menu.nSubSelect == SETTING_SE) {
			m_Menu.nSEVolume--;
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT))
	{
		if (m_Menu.nSubSelect == SETTING_BGM) {
			m_Menu.nBGMVolume++;
		}
		else if (m_Menu.nSubSelect == SETTING_SE) {
			m_Menu.nSEVolume++;
		}
	}

	// アニメーション
	if ((m_Menu.nMaineSelect == MENU_CONTROLLER || m_Menu.nMaineSelect == MENU_SETTING ) && !m_Menu.bRightMove && !m_Menu.bRightDisp) {

		m_Menu.bRightMove = true;
		m_Menu.bRightCoolDown = true;
	}
	else if (m_Menu.nMaineSelect != m_Menu.nRightTextType && !m_Menu.bRightMove && m_Menu.bRightDisp && !m_Menu.bRightCoolDown) {

		m_Menu.bRightMove = true;
		m_Menu.bRightCoolDown = true;
	}
	else if (m_Menu.bRightMove && m_Menu.bRightDisp && m_Menu.bRightCoolDown) {
		if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
			if (m_Menu.nMaineSelect == MENU_CONTROLLER || m_Menu.nMaineSelect == MENU_SETTING)
				m_Menu.nRightCoolDown = COOLDOWN;
		}
		else if (m_Menu.nMaineSelect == m_Menu.nRightTextType) {
			m_Menu.bRightMove = false;
			m_Menu.bRightCoolDown = false;
			m_Menu.nRightCoolDown = COOLDOWN;
		}
	}

	if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
		m_Menu.nMaineOldSelect = m_Menu.nMaineSelect;
	}

	// ループ制御
	IntLoopControl(&m_Menu.nMaineSelect, MENU_MAX, 0);
	IntLoopControl(&m_Menu.nSubSelect, m_Menu.SettingMax-2, 1);
	IntControl(&m_Menu.nBGMVolume, VOLUME_MSX, 0);
	IntControl(&m_Menu.nSEVolume, VOLUME_MSX, 0);

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	// サウンド
	if (m_Menu.nBGMVolume != m_Menu.nBGMOldVolume) {
		m_Menu.nBGMOldVolume = m_Menu.nBGMVolume;

		char data[TXT_MAX];		int nData = m_Menu.nBGMVolume * 5;
		sprintf(data, "%d%s", nData, m_Menu.pSetting[SETTING_BGM_TEXT].Text);

		if (m_pSubMenu[SETTING_BGM_TEXT] != NULL)
		m_pSubMenu[SETTING_BGM_TEXT]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

		float volume = (float)nData / (float)100.0f;
		RNLib::Sound().ChangeSetVolume(CSound::CATEGORY::BGM, volume);
	}
	if (m_Menu.nSEVolume != m_Menu.nSEOldVolume) {
		m_Menu.nSEOldVolume = m_Menu.nSEVolume;

		char data[TXT_MAX];		int nData = m_Menu.nSEVolume * 5;
		sprintf(data, "%d%s", nData, m_Menu.pSetting[SETTING_SE_TEXT].Text);

		if (m_pSubMenu[SETTING_SE_TEXT] != NULL)
		m_pSubMenu[SETTING_SE_TEXT]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

		float volume = (float)nData / (float)100.0f;
		RNLib::Sound().ChangeSetVolume(CSound::CATEGORY::SE, volume);
	}

}

//========================================
// 設定処理
//========================================
void CMode_Title::SettingMenu(void)
{

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
		sprintf(m_PlanetType[nCnt].Text, aStgName);
	}
}

//========================================
// ステージ選択
//========================================
void CMode_Title::StageSelect(void) {

	int nPlanetMax = Manager::StgEd()->GetPlanetMax();
	int nStageMax  = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
	 
	//----------------------------------------
	// 描画処理
	//----------------------------------------
	{// 惑星の描画
		/*RNLib::Model().Put(PRIORITY_OBJECT, m_PlanetType[m_nPlanetIdx].nModel, D3DXVECTOR3(0.0f, -4.0f, 50.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), false)
			->SetOutLineIdx(5);*/
	}

	{//ステージの看板
		RNLib::Model().Put(PRIORITY_OBJECT,m_StgBoardIdx, D3DXVECTOR3(0.0f, 0.0f, -60.0f),INITD3DXVECTOR3, INITD3DXVECTOR3)
			->SetCol(INITCOLOR)
			->SetOutLineIdx(5);
	}

	{// 矢印の描画
		if ((m_nPlanetIdx == 0 && m_nSelect != 0) || (m_nPlanetIdx != 0)) {
			// 矢印の描画(左)

		}

		if ((m_nPlanetIdx != nPlanetMax - 1) || (m_nPlanetIdx == nPlanetMax - 1 && m_nSelect != nStageMax - 1)) {
			// 矢印の描画(右)

		}
	}

	// 選択アイコンの処理
	for (int nCnt = 0; nCnt < nStageMax; nCnt++) {
		if (nCnt == m_nSelect) {
			 //選択時
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, SELECTBOX + nCnt * NUMPOSSELBOX, INITD3DXVECTOR3, INITSCALE3D, false)
				->SetCol(Color{ 243,191,63,255 });
			
			RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(SELECTBOX.x + nCnt * NUMPOSSELBOX.x, SELECTBOX.y, SELECTBOX.z - 5.0f), INITROT3D)
				->SetSize(5.0f, 5.0f)
				->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
		}
		else {
			// 非選択時
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, UNSELECTBOX + nCnt * NUMPOSSELBOX, INITD3DXVECTOR3, INITSCALE3D, false)
				->SetCol(Color{ 81,63,21,255 });

			RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(UNSELECTBOX.x + nCnt * NUMPOSSELBOX.x, UNSELECTBOX.y, UNSELECTBOX.z - 5.0f), INITROT3D)
				->SetSize(5.0f, 5.0f)
				->SetCol(Color{ 85,85,85,255 })
				->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
		}
	}

	//----------------------------------------
	// ステージ選択処理
	//----------------------------------------
	bool bInput = false;
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK)) {
		TextRelease(TEXT_MENU);
		SwapMode(TITLE_MENU_ANIME);
		return;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT)) {
		m_nSelect--;
		bInput = true;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT)) {
		m_nSelect++;
		bInput = true;
	}

	if (bInput) {
		if (m_nSelect < 0 && m_nPlanetIdx != 0) {
			m_nPlanetIdx--;
			nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
			m_nSelect = nStageMax - 1;
		}
		else if (m_nSelect >= nStageMax && m_nPlanetIdx != nPlanetMax-1) {
			m_nPlanetIdx++;
			m_nSelect = 0;
			nStageMax = Manager::StgEd()->GetType()[m_nPlanetIdx].nStageMax;
		}

		IntControl(&m_nSelect, nStageMax - 1, 0);

		if (m_nSelect != m_nOldSelect) {
			m_nOldSelect = m_nSelect;

			m_pMenu[1]->Uninit();
			m_pMenu[1] = NULL;
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };
			m_pMenu[1] = CFontText::Create(
				CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f),
				Manager::StgEd()->GetType()[m_nPlanetIdx].StageType[m_nSelect].aName, CFont::FONT_ROND_B, &pFont);
		}

		if (m_nPlanetIdx != m_nOldnPlanet) {
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
//========================================
void CMode_Title::SwapMode(TITLE aTitle) {
	Title = aTitle;

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
//========================================
void CMode_Title::TextLoad(void) {
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

					m_Menu.OperationMax = nMax;
					m_Menu.pOperation = new Operation[nMax];
					assert(m_Menu.pOperation != NULL);
				}
				if (!strcmp(aDataSearch, "Text")){
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Menu.pOperation[nText].Text);
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

					m_Menu.SettingMax = nMax;
					m_Menu.pSetting = new Setting[nMax];
					assert(m_Menu.pSetting != NULL);
				}
				if (!strcmp(aDataSearch, "Text")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Menu.pSetting[nText].Text);
					nText++;
				}
			}
		}
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