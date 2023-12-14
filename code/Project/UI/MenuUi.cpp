//========================================
// 
// メニュー
// Author:KEISUKE OTONO
// 
//========================================
#include "MenuUi.h"
#include "../main.h"
#include "../../_RNLib/RNsettings.h"
#include "../../_RNLib/Basis/Mechanical/sound.h"
#include "../System/words/words.h"
#include "../System/words/font-text.h"

#include "../Mode/mode_title.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMenuUIクラス
//----------|---------------------------------------------------------------------
//================================================================================
const char* CMenuUI::TITLE_MENU_FILE = "data\\GAMEDATA\\MENU\\TitleMenu.txt";
const char* CMenuUI::PAUSE_MENU_FILE = "data\\GAMEDATA\\MENU\\PauseMenu.txt";
const char* CMenuUI::SUB_MENU_FILE = "data\\GAMEDATA\\MENU\\SubMenu.txt";

//========================================
// コンストラクタ
//========================================
CMenuUI::CMenuUI(void) {

	m_MenuEnd = false;

	m_Menu.LeftScaleX = 0.0f;
	m_Menu.RightScaleX = 0.0f;

	m_Menu.LeftScaleMaxX = 450.0f;
	m_Menu.RightScaleMaxX = 630.0f;

	m_Menu.LeftPos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	m_Menu.RightPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Menu.nCntLeftAnime = 0;
	m_Menu.nCntRightAnime = 0;
	m_Menu.nMaineSelect = 0;
	m_Menu.nMaineOldSelect = 0;
	m_Menu.nSubSelect = 1;
	m_Menu.bSubMenuMove = false;
	m_Menu.bSubMenuDisp = false;
	m_Menu.nRightCoolDown = COOLDOWN;
	m_Menu.SubMenuCD = false;
	m_Menu.nRightTextType = 0;
	m_Menu.bMenu = false;
	m_Menu.bSubMenu = false;
	m_Menu.bClose = false;
	m_MenuEnd = false;
	m_Menu.bBackMode = false;
	m_Menu.bGameEnd = false;
	m_Menu.pOperation = NULL;
	m_Menu.pSetting = NULL;
	m_Menu.bFullScreen = RNSettings::GetInfo().isFullScreen;

	m_Menu.MainMenuMax = 0;
	m_Menu.SubMenuMax = 0;

	{// 音量設定の初期化
		float BGM = RNLib::Sound().GetCategoryState(CSound::CATEGORY::BGM).settingVolume;
		float SE = RNLib::Sound().GetCategoryState(CSound::CATEGORY::SE).settingVolume;
		m_Menu.nBGMVolume = BGM * VOLUME_MSX;
		m_Menu.nSEVolume = SE * VOLUME_MSX;
		m_Menu.nBGMOldVolume = BGM * VOLUME_MSX;
		m_Menu.nSEOldVolume = SE * VOLUME_MSX;
	}
}

//========================================
// デストラクタ
//========================================
CMenuUI::~CMenuUI(void) {

	// テキストの破棄
	TextRelease(TEXT_ALL);

	if (m_MaineMenu != NULL) {
		delete[] m_MaineMenu;
		m_MaineMenu = NULL;
	}

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
void CMenuUI::Init(void)
{
	// テキストの初期化
	for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++)
		m_pMenu[nCnt] = NULL;
	for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++)
		m_pSubMenu[nCnt] = NULL;
}

//========================================
// 終了処理
//========================================
void CMenuUI::Uninit(void)
{

}

//========================================
// 更新処理
//========================================
void CMenuUI::Update(void)
{
	if (!m_Menu.bClose)
		MenuSelect();

	MenuAnime();

	// メインメニュー
	RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, D3DXVECTOR3(m_Menu.LeftPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
		->SetSize(m_Menu.LeftScaleX, RNLib::Window().GetCenterY() * 2)
		->SetCol(Color{ 150,150,150,150 });

	// サブメニュー
	RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Menu.RightPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
		->SetSize(m_Menu.RightScaleX, RNLib::Window().GetCenterY() * 2)
		->SetCol(Color{ 150,150,150,150 });

	if (m_pSubMenu[0] != NULL)
		RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Menu.RightPos.x + 10, 400.0f, 100.0f), 0.0f, false)
		->SetSize(560.0f, 600.0f)
		->SetTex(m_Menu.BoxTex);

	// 入力
	DecisionInput();
}

//========================================
// 生成
//========================================
CMenuUI *CMenuUI::Create(CMode::TYPE type)
{
	CMenuUI *pMenu = new CMenuUI;

	// 初期化処理
	pMenu->Init();

	// メニュー読込
	pMenu->MaineMenuLoad(type);
	pMenu->SubMenuLoad();

	return pMenu;
}

//========================================
// 決定処理
//========================================
void CMenuUI::DecisionInput(void)
{
	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && Manager::Transition().GetState() == CTransition::STATE::NONE)
	{
		CMode::TYPE Mode = Manager::GetMode();

		if (!m_Menu.bSubMenu) {

			if (Mode == CMode::TYPE::TITLE)	{
				switch (m_Menu.nMaineSelect)
				{
				case TITLE_MENU_GAME:
					if (!m_Menu.bClose) {
						m_Menu.bClose = true;
					}
					break;
				case TITLE_MENU_SETTING:
					if (!m_Menu.bSubMenu) {
						m_Menu.bSubMenu = true;
						m_pMenu[m_Menu.nMaineSelect]->SetBoxColor(Color{ 155,155,155,255 });
					}
					break;
				case TITLE_MENU_END:
					//ゲームの終了
					m_Menu.bGameEnd = true;
					break;
				}
			}
			else if (Mode == CMode::TYPE::GAME)
			{
				switch (m_Menu.nMaineSelect)
				{
				case PAUSE_MENU_RESUME:
					m_Menu.bClose = true;
					break;
				case PAUSE_MENU_RESET:
					TextRelease(CMenuUI::TEXT_ALL);
					Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);
					m_Menu.bClose = true;
					Manager::EffectMgr()->ReleaseAll();
					break;
				case PAUSE_MENU_SELECT:
					TextRelease(CMenuUI::TEXT_ALL);
					Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
					CMode_Title::SetSelect(true);
					Manager::EffectMgr()->ReleaseAll();
					break;
				case PAUSE_MENU_CONTROLLER:
					break;
				case PAUSE_MENU_SETTING:
					m_Menu.bSubMenu = true;
					m_pMenu[m_Menu.nMaineSelect]->SetBoxColor(Color{ 155,155,155,255 });
					break;
				}
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

}

//========================================
// 選択処理
//========================================
void CMenuUI::SelectInput(void)
{
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK))
	{
		if (!m_Menu.bSubMenu) {
			m_Menu.bBackMode = true;
			m_Menu.bSubMenuMove = true;
			m_Menu.bClose = true;
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

	// ループ制御
	IntLoopControl(&m_Menu.nMaineSelect, m_Menu.MainMenuMax, 0);
	IntLoopControl(&m_Menu.nSubSelect, m_Menu.SettingMax - 2, 1);

	// アニメーション
	if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID != -1 && !m_Menu.bSubMenuMove && !m_Menu.bSubMenuDisp) {

		m_Menu.bSubMenuMove = true;
		m_Menu.SubMenuCD = true;
	}
	else if (m_Menu.nMaineSelect != m_Menu.nRightTextType && !m_Menu.bSubMenuMove && m_Menu.bSubMenuDisp && !m_Menu.SubMenuCD) {

		m_Menu.bSubMenuMove = true;
		m_Menu.SubMenuCD = true;
	}
	else if (m_Menu.bSubMenuMove && m_Menu.bSubMenuDisp && m_Menu.SubMenuCD) {
		if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
			if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID >= 0)
				m_Menu.nRightCoolDown = COOLDOWN;
		}
		else if (m_Menu.nMaineSelect == m_Menu.nRightTextType) {
			m_Menu.bSubMenuMove = false;
			m_Menu.SubMenuCD = false;
			m_Menu.nRightCoolDown = COOLDOWN;
		}
	}

	if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
		m_Menu.nMaineOldSelect = m_Menu.nMaineSelect;
	}

	// ループ制御
	IntControl(&m_Menu.nBGMVolume, VOLUME_MSX, 0);
	IntControl(&m_Menu.nSEVolume, VOLUME_MSX, 0);
}

//========================================
// メニュー生成
//========================================
void CMenuUI::MenuCreate(void)
{
	m_Menu.LeftPos = D3DXVECTOR3(280, 0.0f, 0.0f);
	m_Menu.RightPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Menu.LeftScaleX = 0.0f;
	m_Menu.RightScaleX = 0.0f;
	m_Menu.nCntLeftAnime = 0;
	m_Menu.nCntRightAnime = 0;
	m_Menu.nMaineSelect = 0;
	m_Menu.nMaineOldSelect = 0;
	m_Menu.nSubSelect = 1;
	m_Menu.bSubMenuMove = false;
	m_Menu.bSubMenuDisp = false;
	m_Menu.nRightCoolDown = COOLDOWN;
	m_Menu.SubMenuCD = false;
	m_Menu.nRightTextType = 0;
	m_Menu.bMenu = false;
	m_Menu.bSubMenu = false;
	m_Menu.bClose = false;
	m_MenuEnd = false;
	m_Menu.bBackMode = false;
	m_Menu.bGameEnd = false;

	m_Menu.BoxTex = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox20.png");

	CMode::TYPE Mode = Manager::GetMode();
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),35.0f,1,1,-1, };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,0.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	if (Mode == CMode::TYPE::TITLE) {
		for (int nText = 0; nText < m_Menu.MainMenuMax; nText++) {
			m_pMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Menu.LeftPos.x - 10, 300.0f + (100.0f * nText), 0.0f), D3DXVECTOR2(450.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont, false, true, &pShadow);

			m_pMenu[nText]->SetBoxColor(Color{0,0,0,0});

			if (m_MaineMenu[nText].Tex.bSet)
				m_pMenu[nText]->SetBoxTex(m_MaineMenu[nText].Tex.TexFile, m_MaineMenu[nText].Tex.PtnIdx, m_MaineMenu[nText].Tex.PtnX, m_MaineMenu[nText].Tex.PtnY);
		}
	}
	else if (Mode == CMode::TYPE::GAME) {
		for (int nText = 0; nText < m_Menu.MainMenuMax; nText++) {
			m_pMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Menu.LeftPos.x - 20, 150.0f + (100.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont);

			m_pMenu[nText]->SetBoxColor(Color{ 0,0,0,0 });
		}
	}
}

//========================================
// サブテキストの生成
//========================================
void CMenuUI::SubTextCreate(void)
{
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	if (m_pSubMenu != NULL)
		TextRelease(TEXT_RIGHT);

	if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 0) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Menu.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
			"", CFont::FONT_ROND_B, &pFont);

		for (int nText = 1; nText < m_Menu.OperationMax; nText++) {
			m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Menu.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont, false, false);
		}
	}
	else if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 1) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Menu.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),//-210
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
void CMenuUI::MenuAnime(void)
{
	// 左画面のアニメーション
	if (!m_Menu.bMenu || m_Menu.bClose)
	{
		float ScaleRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_Menu.nCntLeftAnime, PAUSE_LEFT_ANIME);

		m_Menu.LeftScaleX = m_Menu.LeftScaleMaxX * ScaleRate;

		int nAnime = 0;

		if (!m_Menu.bClose) {
			++m_Menu.nCntLeftAnime;
			nAnime = PAUSE_LEFT_ANIME;
		}
		else if (m_Menu.bClose) {
			--m_Menu.nCntLeftAnime;
			nAnime = 0;
		}

		if (m_Menu.nCntLeftAnime == nAnime) {
			m_Menu.LeftScaleX = m_Menu.LeftScaleMaxX;

			if (!m_Menu.bClose) {
				m_Menu.bMenu = true;

				FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
				FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

				// Textの再生成
				for (int nCnt = 0; nCnt < m_Menu.MainMenuMax; nCnt++) {
					m_pMenu[nCnt]->Regeneration(m_MaineMenu[nCnt].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
					m_pMenu[nCnt]->SetBoxColor(Color{ 255,255,255,255 });
				}
			}
			else if (m_Menu.bClose) {
				m_Menu.LeftScaleX = 0;
				m_MenuEnd = true;
			}
		}
	}

	// 右画面のアニメーション
	if (m_Menu.bSubMenuMove && m_Menu.SubMenuCD) {
		if (--m_Menu.nRightCoolDown <= 0) {

			m_Menu.SubMenuCD = false;
			m_Menu.nRightCoolDown = COOLDOWN;
			if (!m_Menu.bSubMenuDisp) {
				//SubTextCreate();
			}
			else if (m_Menu.bSubMenuDisp && !m_Menu.bClose) {
				TextRelease(TEXT_RIGHT);
			}
		}
	}
	else if (m_Menu.bSubMenuMove && !m_Menu.SubMenuCD || (m_Menu.bClose && m_Menu.bSubMenuDisp))
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;

			float ScaleRate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_Menu.nCntRightAnime, PAUSE_RIGHT_ANIME);
			m_Menu.RightScaleX = m_Menu.RightScaleMaxX * ScaleRate;

			int nAnime = 0;

			if (!m_Menu.bClose && !m_Menu.bSubMenuDisp) {
				++m_Menu.nCntRightAnime;
				nAnime = PAUSE_LEFT_ANIME;
			}
			else if (m_Menu.bClose || m_Menu.bSubMenuDisp) {
				--m_Menu.nCntRightAnime;
				nAnime = 0;
			}

			int nTextMax = -1;
			if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 0) nTextMax = m_Menu.OperationMax;
			else if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 1) nTextMax = m_Menu.SettingMax;

			for (int nCnt = 0; nCnt < nTextMax; nCnt++) {
				if (m_pSubMenu[nCnt] != NULL) {
					m_pSubMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
				}
			}

			if (m_Menu.nCntRightAnime == nAnime) {
				m_Menu.bSubMenuMove = false;

				if (m_Menu.bClose || m_Menu.bSubMenuDisp)
					m_Menu.RightScaleX = 0;

				if (!m_Menu.bSubMenuDisp)
				{
					SubTextCreate();
					m_Menu.bSubMenuDisp = true;
					FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
					FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

					// テキストの再生成
					if (m_MaineMenu[m_Menu.nRightTextType].nSubMenuID == 0) {
						for (int nText = 0; nText < m_Menu.OperationMax; nText++) {
							if (m_pSubMenu[nText] != NULL)
								m_pSubMenu[nText]->Regeneration(m_Menu.pOperation[nText].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
						}
					}
					if (m_MaineMenu[m_Menu.nRightTextType].nSubMenuID == 1) {
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
				else if (m_Menu.bSubMenuDisp && !m_Menu.bClose) {
					m_Menu.bSubMenuDisp = false;
					TextRelease(TEXT_RIGHT);
				}
			}
	}
}

//========================================
// メニュー
//========================================
void CMenuUI::MenuSelect(void)
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
	for (int nCnt = 0; nCnt < m_Menu.MainMenuMax; nCnt++)
	{
		if (!m_Menu.bSubMenu) {
			if (m_pMenu[nCnt] != NULL) {
				if (!m_MaineMenu[nCnt].Tex.bSet) {
					if (nCnt == m_Menu.nMaineSelect)
						m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
					else
						m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
				}
				else if (m_MaineMenu[nCnt].Tex.bSet) {
					if (nCnt == m_Menu.nMaineSelect)
						m_pMenu[nCnt]->SetBoxPthIdx(1);
					else
						m_pMenu[nCnt]->SetBoxPthIdx(0);
				}
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

	// メニュー選択
	SelectInput();

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
// メインメニュー読込
//========================================
void CMenuUI::MaineMenuLoad(CMode::TYPE type)
{
	int nCntSelect = 0;
	char aDataSearch[TXT_MAX];

	FILE *pFile = NULL;

	// ファイルの読み込み
	if (type == CMode::TYPE::TITLE)
		pFile = fopen(TITLE_MENU_FILE, "r");
	else if (type == CMode::TYPE::GAME)
		pFile = fopen(PAUSE_MENU_FILE, "r");
	else
		pFile = fopen(TITLE_MENU_FILE, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END")) {
			fclose(pFile);
			break;
		}
		else if (aDataSearch[0] == '#')	continue;

		else if (!strcmp(aDataSearch, "MenuSelectMax"))
		{
			int nMax = -1;

			fscanf(pFile, "%s", &aDataSearch[0]);
			fscanf(pFile, "%d", &nMax);

			if (nMax <= 0)
				nMax = 0;
			
			m_Menu.MainMenuMax = nMax;
			m_MaineMenu = new MaineMenu[nMax];
			assert(m_MaineMenu != NULL);
		}
		else if (!strcmp(aDataSearch, "SetSelect"))
		{
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索
				if (!strcmp(aDataSearch, "EndSelect")) {
					nCntSelect++;
					break;
				}
				else if (!strcmp(aDataSearch, "Text")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_MaineMenu[nCntSelect].Text);
					m_MaineMenu[nCntSelect].Tex.bSet = false;

				}
				else if (!strcmp(aDataSearch, "SubMenuID")) {

					int ID = -1;

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &ID);

					if (ID <= -1)
						ID = -1;

					m_MaineMenu[nCntSelect].nSubMenuID = ID;
				}
				else if (!strcmp(aDataSearch, "SetTexture"))
				{
					while (1)
					{
						fscanf(pFile, "%s", aDataSearch);	// 検索
						if (!strcmp(aDataSearch, "EndTexture")) {
							break;
						}
						if (!strcmp(aDataSearch, "FilePath")) {
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%s", &m_MaineMenu[nCntSelect].Tex.TexFile);
							m_MaineMenu[nCntSelect].Tex.bSet = true;
						}
						else if (!strcmp(aDataSearch, "PtnIdx")) {
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_MaineMenu[nCntSelect].Tex.PtnIdx);
						}
						else if (!strcmp(aDataSearch, "PtnX")) {
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_MaineMenu[nCntSelect].Tex.PtnX);
						}
						else if (!strcmp(aDataSearch, "PtnY")) {
							fscanf(pFile, "%s", &aDataSearch[0]);
							fscanf(pFile, "%d", &m_MaineMenu[nCntSelect].Tex.PtnY);
						}
					}
				}
			}
		}
	}
}

//========================================
// サブメニュー読込
//========================================
void CMenuUI::SubMenuLoad(void) {
	int nCntPlanet = 0;
	char aDataSearch[TXT_MAX];	// データ検索用

	// ファイルの読み込み
	FILE *pFile = fopen(SUB_MENU_FILE, "r");

	if (pFile == NULL)
	{// 種類毎の情報のデータファイルが開けなかった場合、
	 //処理を終了する
		return;
	}

	// ENDが見つかるまで読み込みを繰り返す
	while (1)
	{
		fscanf(pFile, "%s", aDataSearch);	// 検索

		if (!strcmp(aDataSearch, "END")) {
			fclose(pFile);
			break;
		}
		if (aDataSearch[0] == '#') continue;

		if (!strcmp(aDataSearch, "SetOperation"))
		{
			int nText = 0;
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索
				if (!strcmp(aDataSearch, "EndOperation")) {
					break;
				}
				if (!strcmp(aDataSearch, "TextMax")) {
					int nMax = -1;

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &nMax);

					if (nMax <= 0)
						nMax = 0;

					m_Menu.OperationMax = nMax;
					m_Menu.pOperation = new Operation[nMax];
					assert(m_Menu.pOperation != NULL);
				}
				if (!strcmp(aDataSearch, "Text")) {
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
void CMenuUI::TextRelease(TEXT type) {
	
	// メニュー
	if (type == TEXT_MENU || type == TEXT_ALL) {
		for (int nCnt = 0; nCnt < FONT_TEXT_MAX; nCnt++) {
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