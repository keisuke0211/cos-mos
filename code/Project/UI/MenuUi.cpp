//========================================
// 
// メニュー
// Author:KEISUKE OTONO
// 
//========================================
#include "MenuUi.h"
#include "../stage.h"
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

	m_Menu.RightScale = INITD3DXVECTOR2;

	m_Menu.RightScaleMax = D3DXVECTOR2(560.0f,600.0f);

	m_Menu.LeftPos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	m_Menu.RightPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Menu.nCntLeftAnime = 0;
	m_Menu.nCntRightAnime = 0;
	m_Menu.nCntRightTextAnime = 0;
	m_Menu.BoxTexIdx = 0;
	m_Menu.BoxColor = Color{ 255,255,255,0 };
	m_Menu.nMaineSelect = 0;
	m_Menu.nMaineOldSelect = 0;
	m_Menu.nSubSelect = 1;
	m_Menu.nSubMenuIdx = 0;
	m_Menu.bSubMenuMove = false;
	m_Menu.bSubMenuDisp = false;
	m_Menu.nRightCoolDown = COOLDOWN;
	m_Menu.bSubMenuCD = false;
	m_Menu.nRightTextType = 0;
	m_Menu.bAllElasticity = false;
	m_Menu.bMenu = false;
	m_Menu.bSubMenu = false;
	m_Menu.bSubTextMenu = false;
	m_Menu.bSubText = false;
	m_Menu.bSubMenuCreate = false;
	m_Menu.bClose = false;
	m_MenuEnd = false;
	m_Menu.bBackMode = false;
	m_Menu.bGameEnd = false;
	m_Menu.pOperation = NULL;
	m_Menu.pSetting = NULL;
	m_Menu.bFullScreen = RNLib::Options().GetIsFullScreen();

	m_Menu.SubMenuSize = new D3DXVECTOR2[TEXT_MAX];
	m_Menu.SubMenuSizeMax = new D3DXVECTOR2[TEXT_MAX];

	for (int nTxt = 0; nTxt < TEXT_MAX; nTxt++)
	{
		m_Menu.SubMenuSize[nTxt] = INITD3DXVECTOR2;
		m_Menu.SubMenuSizeMax[nTxt] = INITD3DXVECTOR2;
	}

	m_Menu.MainMenuMax = 0;
	m_Menu.SubMenuMax = 0;

	{// 音量設定の初期化
		float BGM = RNLib::Options().GetCategoryVolume(_RNC_Sound::CATEGORY::BGM);
		float SE = RNLib::Options().GetCategoryVolume(_RNC_Sound::CATEGORY::SE);
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

	if (m_Menu.SubMenuSize != NULL){
		delete[] m_Menu.SubMenuSize;
		m_Menu.SubMenuSize = NULL;
	}

	if (m_Menu.SubMenuSizeMax != NULL) {
		delete[] m_Menu.SubMenuSizeMax;
		m_Menu.SubMenuSizeMax = NULL;
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
	if (!m_Menu.bClose && m_Menu.bMenu)
		MenuSelect();

	PartElasticity();

	MenuAnime();

	// サブメニュー
	RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Menu.RightPos.x + 10, 400.0f, 100.0f), 0.0f, false)
		->SetSize(m_Menu.RightScale.x, m_Menu.RightScale.y)
		->SetTex(m_Menu.BoxTex[m_Menu.BoxTexIdx])
		->SetCol(m_Menu.BoxColor);
		
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
	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::A)) && Manager::Transition().GetState() == CTransition::STATE::NONE)
	{
		CMode::TYPE Mode = Manager::GetMode();

		RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::SE_ENTER], _RNC_Sound::CATEGORY::SE, 0.7f, false);

		if (!m_Menu.bSubMenu) {
			if(m_pMenu[m_Menu.nMaineSelect] != NULL)
				m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxPthIdx(0);
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

						if (m_pMenu[m_Menu.nMaineSelect] != NULL)
							m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxColor(Color{ 155,155,155,255 });
					}
					break;
				case TITLE_MENU_END:
					//ゲームの終了
					if (!m_Menu.bClose) {
						m_Menu.bClose = true;
					}
					break;
				}
			}
			else if (Mode == CMode::TYPE::GAME)
			{
				switch (m_Menu.nMaineSelect)
				{
				case PAUSE_MENU_RESUME:
				case PAUSE_MENU_RESET:
				case PAUSE_MENU_SELECT:
					m_Menu.bClose = true;
					break;
				case PAUSE_MENU_CONTROLLER:
					break;
				case PAUSE_MENU_SETTING:
					m_Menu.bSubMenu = true;

					if (m_pMenu[m_Menu.nMaineSelect] != NULL)
						m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxColor(Color{ 155,155,155,255 });
					break;
				}
			}
		}
		else if (m_Menu.bSubMenu) {

			switch (m_Menu.nSubSelect)
			{
			case SETTING_SCREEN:

				if (m_Menu.nCntScrChg <= 0) {
					int nText = SETTING_SCREEN_TEXT;
					m_Menu.bFullScreen = !m_Menu.bFullScreen;

					// ウインドウのモードを切り替える
					RNLib::Options().SetIsFullScreen(m_Menu.bFullScreen);

					char data[TXT_MAX] = {};
					if (!m_Menu.bFullScreen)	sprintf(data, " OFF");
					else if (m_Menu.bFullScreen)	sprintf(data, " ON");

					FontFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };

					if (m_pSubMenu[nText] != NULL)
						m_pSubMenu[nText]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFont);

					m_Menu.nCntScrChg = 30;
				}

				break;
			case SETTING_BGM:
				break;
			case SETTING_SE:
				break;
			case SETTING_BACK:
				m_Menu.bSubMenu = false;
				m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxColor(COLOR_WHITE);
				m_pSubMenu[m_Menu.nSubSelect]->SetTxtBoxType(CFontText::BOX_SUB_MENU00);
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
	if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
		m_Menu.nMaineOldSelect = m_Menu.nMaineSelect;
	}

	if ((RNLib::Input().GetTrigger(DIK_BACKSPACE, _RNC_Input::BUTTON::B) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::BACK)) &&
		Manager::GetMode() == CMode::TYPE::TITLE) {
		if (!m_Menu.bSubMenu) {
			m_Menu.nCntLeftAnime = 0;
			m_Menu.bClose = true;
			m_Menu.bBackMode = true;
			m_Menu.bSubMenuMove = false;
			m_Menu.bSubMenuCreate = false;

			return;
		}
		else if (m_Menu.bSubMenu) {
			m_Menu.bSubMenu = false;

			m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxPthIdx(1);
			m_pMenu[m_Menu.nMaineSelect]->SetTxtBoxColor(COLOR_WHITE);

			for (int nText = 0; nText < m_Menu.SettingMax; nText++) {
				if (m_pSubMenu[nText] != NULL) {

					CFontText::Box BoxType = m_pSubMenu[nText]->GetTxtBoxType();

					if (BoxType == CFontText::BOX_SUB_SELECTU00)
						m_pSubMenu[nText]->SetTxtBoxType(CFontText::BOX_SUB_MENU00);
					else if (BoxType == CFontText::BOX_SUB_SELECTU01)
						m_pSubMenu[nText]->SetTxtBoxType(CFontText::BOX_SUB_MENU01);
				}
			}
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::UP))
	{
		if (!m_Menu.bSubMenu)
			m_Menu.nMaineSelect--;
		else if (m_Menu.bSubMenu)
			m_Menu.nSubSelect--;

		RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::SE_SELECT], _RNC_Sound::CATEGORY::SE, 0.7f, false);

		m_Menu.bPartElasticity = true;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::DOWN))
	{
		if (!m_Menu.bSubMenu)
			m_Menu.nMaineSelect++;
		else if (m_Menu.bSubMenu)
			m_Menu.nSubSelect++;

		RNLib::Sound().Play(CResources::SOUND_IDXES[(int)CResources::SOUND::SE_SELECT], _RNC_Sound::CATEGORY::SE, 0.7f, false);


		m_Menu.bPartElasticity = true;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::LEFT))
	{
		if (m_Menu.nSubSelect == SETTING_BGM) {
			m_Menu.nBGMVolume--;
		}
		else if (m_Menu.nSubSelect == SETTING_SE) {
			m_Menu.nSEVolume--;
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::RIGHT))
	{
		if (m_Menu.nSubSelect == SETTING_BGM) {
			m_Menu.nBGMVolume++;
		}
		else if (m_Menu.nSubSelect == SETTING_SE) {
			m_Menu.nSEVolume++;
		}
	}

	// ループ制御
	RNLib::Number().LoopClamp(&m_Menu.nMaineSelect, 0, m_Menu.MainMenuMax - 1);
	RNLib::Number().LoopClamp(&m_Menu.nSubSelect, 1, SETTING_SCREEN_TEXT - 1);

	// アニメーション
	if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID != -1 && !m_Menu.bSubMenuMove && !m_Menu.bSubMenuDisp) {

		m_Menu.bSubMenuMove = true;
		m_Menu.bSubMenuCD = true;
	}
	else if (m_Menu.nMaineSelect != m_Menu.nRightTextType && !m_Menu.bSubMenuMove && m_Menu.bSubMenuDisp && !m_Menu.bSubMenuCD) {

		m_Menu.bSubMenuMove = true;
		m_Menu.bSubMenuCD = true;
	}
	else if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == -1 && m_Menu.bSubMenuMove)
	{
		m_Menu.bSubMenuDisp = true;
	}
	else if (m_Menu.bSubMenuMove && m_Menu.bSubMenuDisp && m_Menu.bSubMenuCD) {
		if (m_Menu.nMaineSelect != m_Menu.nMaineOldSelect) {
			if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID >= 0)
				m_Menu.nRightCoolDown = COOLDOWN;
		}
		else if (m_Menu.nMaineSelect == m_Menu.nRightTextType) {
			m_Menu.bSubMenuCD = false;
			m_Menu.nRightCoolDown = COOLDOWN;
		}
	}

	// ループ制御
	RNLib::Number().Clamp(&m_Menu.nBGMVolume, 0, VOLUME_MSX);
	RNLib::Number().Clamp(&m_Menu.nSEVolume, 0, VOLUME_MSX);
}

//========================================
// メニュー生成
//========================================
void CMenuUI::MenuCreate(void)
{
	m_Menu.LeftPos = D3DXVECTOR3(280, 0.0f, 0.0f);
	m_Menu.RightPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Menu.RightScale = INITD3DXVECTOR2;
	m_Menu.nCntLeftAnime = 0;
	m_Menu.nCntRightAnime = 0;
	m_Menu.nMaineSelect = 0;
	m_Menu.nMaineOldSelect = 0;
	m_Menu.nSubSelect = 1;
	m_Menu.bSubMenuMove = false;
	m_Menu.bSubMenuDisp = false;
	m_Menu.nRightCoolDown = COOLDOWN;
	m_Menu.bSubMenuCD = false;
	m_Menu.nRightTextType = 0;
	m_Menu.nNumLeftMenu = 0;
	m_Menu.bPartElasticity = false;
	m_Menu.bAllElasticity = false;
	m_Menu.bMenu = false;
	m_Menu.bSubMenu = false;
	m_Menu.bClose = false;
	m_MenuEnd = false;
	m_Menu.bBackMode = false;
	m_Menu.bGameEnd = false;

	m_Menu.BoxTex[0] = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox20.png");
	m_Menu.BoxTex[1] = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox21.png");

	CMode::TYPE Mode = Manager::GetMode();
	FontFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),35.0f,1,1,-1, };

	D3DXVECTOR3 pos = INITD3DXVECTOR3;
	D3DXVECTOR2 size = INITD3DXVECTOR2;
	D3DXVECTOR2 TargetSize = INITD3DXVECTOR2;

	if (Mode == CMode::TYPE::TITLE) {
		pos = D3DXVECTOR3(m_Menu.LeftPos.x - 10, 400.0f, 0.0f);
		size = D3DXVECTOR2(0.0f, 0.0f);
		TargetSize = D3DXVECTOR2(400.0f*1.2f, 100.0f*1.2f);
	}
	else if (Mode == CMode::TYPE::GAME) {
		pos = D3DXVECTOR3(m_Menu.LeftPos.x - 20, 150.0f, 0.0f);
		size = D3DXVECTOR2(0.0f, 0.0f);
		TargetSize = D3DXVECTOR2(320.0f, 80.0f);
	}

	for (int nText = 0; nText < m_Menu.MainMenuMax; nText++) {
		m_pMenu[nText] = CFontText::Create(CFontText::BOX_MAINE_MENU,
			D3DXVECTOR3(pos.x, pos.y + (100.0f * nText), pos.z), size,
			"", CFont::FONT_CONVENIE, &pFont);

		if (nText == m_Menu.nMaineSelect) {
			m_pMenu[nText]->SetTxtBoxColor(Color{ 255,255,255,255 });
		}
		else {
			m_pMenu[nText]->SetTxtBoxColor(Color{ 155,155,155,255 });
		}

		m_pMenu[nText]->SetTxtBoxTgtSize(TargetSize.x, TargetSize.y);

		if (m_MaineMenu[nText].Tex.bSet)
			m_pMenu[nText]->SetTxtBoxTex(m_MaineMenu[nText].Tex.TexFile, m_MaineMenu[nText].Tex.PtnIdx, m_MaineMenu[nText].Tex.PtnX, m_MaineMenu[nText].Tex.PtnY);
	}
}

//========================================
// サブテキストの生成
//========================================
void CMenuUI::SubTextCreate(void)
{
	FontFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	if (!m_Menu.bSubMenuCreate)
	{
		if (m_pSubMenu != NULL)
			TextRelease(TEXT_RIGHT);

		if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 0) {

			m_Menu.SubMenuSizeMax[0] = D3DXVECTOR2(256.0f, 70.0f);
			m_pSubMenu[INPUT_TITLE] = CFontText::Create(
				CFontText::BOX_SUB_TITLE, D3DXVECTOR3(m_Menu.RightPos.x - 130, 50.0f, 0.0f), D3DXVECTOR2(256.0f, 70.0f),
				"", CFont::FONT_WAKUWAKU, &pFont);
			m_pSubMenu[INPUT_TITLE]->SetTxtBoxColor(Color{ 0,0,0,0 });

			for (int nText = 1; nText < m_Menu.OperationMax; nText++) {
				m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NONE,
					D3DXVECTOR3(m_Menu.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
					"", CFont::FONT_WAKUWAKU, &pFont, false, false);

				m_Menu.SubMenuSizeMax[nText] = D3DXVECTOR2(370.0f, 80.0f);
			}
		}
		else if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 1) {

			m_Menu.SubMenuSizeMax[0] = D3DXVECTOR2(175.0f, 70.0f);
			m_pSubMenu[INPUT_TITLE] = CFontText::Create(
				CFontText::BOX_SUB_TITLE, D3DXVECTOR3(m_Menu.RightPos.x - 180, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
				"", CFont::FONT_WAKUWAKU, &pFont);
			m_pSubMenu[INPUT_TITLE]->SetTxtBoxColor(Color{ 0,0,0,0 });

			pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

			for (int nText = 1; nText < m_Menu.SettingMax; nText++) {

				D3DXVECTOR3 pos = INITD3DXVECTOR3;
				D3DXVECTOR2 size = INITD3DXVECTOR2;

				// 位置
				if (nText <= SETTING_SE)
					pos = D3DXVECTOR3(m_Menu.RightPos.x, 180.0f + (130 * (nText - 1)), 0.0f);
				else if (nText == SETTING_BACK)
					pos = D3DXVECTOR3(m_Menu.RightPos.x + 160.0f, 620.0f, 0.0f);//190
				else if (nText >= SETTING_SCREEN_TEXT)
					pos = D3DXVECTOR3(m_Menu.RightPos.x + 85.0f, 230.0f + (130.0f * (nText - SETTING_SCREEN_TEXT)), 0.0f);

				// サイズとボックス種類
				CFontText::Box type = CFontText::BOX_NONE;
				if (nText > SETTING_BACK) {
					size = D3DXVECTOR2(256.0f, 64.0f);
					type = CFontText::BOX_SUB_MENU01;
				}
				else if (nText == SETTING_BACK) {
					size = D3DXVECTOR2(150.0f, 80.0f);
					type = CFontText::BOX_SUB_MENU02;
				}
				else {
					size = D3DXVECTOR2(480.0f, 60.0f);
					type = CFontText::BOX_SUB_MENU00;
				}

				m_pSubMenu[nText] = CFontText::Create(type,
					pos, size,
					"", CFont::FONT_WAKUWAKU, &pFont, false, true);

				m_pSubMenu[nText]->SetTxtBoxColor(Color{ 255,255,255,0 });
			}
		}

		m_Menu.nRightTextType = m_Menu.nMaineSelect;
		m_Menu.bSubMenuCreate = true;
	}
}

//========================================
// メニュー演出
//========================================
void CMenuUI::MenuAnime(void)
{
	// 左画面のアニメーション

	// 出現
	if (!m_Menu.bMenu)
	{
		int Txt = m_Menu.nNumLeftMenu;
		float TgtSizeX = m_pMenu[Txt]->GetTxtBoxTgtSize().x;
		float ScaleRate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntLeftAnime, PAUSE_LEFT_ANIME);
		float SizeX = TgtSizeX * ScaleRate;
		float SizeY = m_pMenu[Txt]->GetTxtBoxTgtSize().y;

		m_pMenu[Txt]->SetTxtBoxSize(SizeX, SizeY);

		if (++m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME) {
			m_Menu.nCntLeftAnime = 0;

			m_pMenu[Txt]->SetTxtBoxSize(TgtSizeX, SizeY);
			{// Textの再生成
				D3DXCOLOR col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				if (Txt == m_Menu.nMaineSelect) {
					col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
				}

				FontFont pFont = { col,35.0f,3,1,-1, };

				m_pMenu[Txt]->Regeneration(m_MaineMenu[Txt].Text, CFont::FONT_WAKUWAKU, &pFont);
				m_pMenu[Txt]->SetTextSkip(true);

				if (Txt == m_Menu.nMaineSelect) {
					m_pMenu[Txt]->SetTxtBoxColor(Color{ 255,255,255,255 });
				}
				else {
					m_pMenu[Txt]->SetTxtBoxColor(Color{ 155,155,155,255 });
				}
			}

			if (++m_Menu.nNumLeftMenu >= m_Menu.MainMenuMax)
			{
				m_Menu.bMenu = true;
				m_Menu.bPartElasticity = true;
			}
		}
	}

	// 閉じる
	if (m_Menu.bMenu && m_Menu.bClose) {

		// 膨らむ ・ 縮む
		if (!m_Menu.bAllElasticity) {
			for (int Txt = 0; Txt < m_Menu.MainMenuMax; Txt++) {

				if (m_Menu.nCntLeftAnime == 0) {
					FontFont pFont = { D3DXCOLOR(0.0f,0.0f,0.0f,0.0f),35.0f,3,1,-1, };

					m_pMenu[Txt]->Regeneration("", CFont::FONT_CONVENIE, &pFont);
				}

				// 膨縮
				float TgtSizeX = m_pMenu[Txt]->GetTxtBoxTgtSize().x;
				float SizeY = m_pMenu[Txt]->GetTxtBoxTgtSize().y;

				if (Txt == m_Menu.nMaineSelect && !m_Menu.bBackMode)
					TgtSizeX = 80.0f;

				float ScaleRate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntLeftAnime, PAUSE_LEFT_ANIME);
				float SizeX = 0;

				if (Txt == m_Menu.nMaineSelect && !m_Menu.bBackMode)
					SizeX = m_pMenu[Txt]->GetTxtBoxTgtSize().x + (TgtSizeX * ScaleRate);
				else if (Txt != m_Menu.nMaineSelect || m_Menu.bBackMode)
					SizeX = TgtSizeX - (TgtSizeX * ScaleRate);

				m_pMenu[Txt]->SetTxtBoxSize(SizeX, SizeY);

				if ((m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME - 1 && Txt != m_Menu.nMaineSelect) || (m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME - 1 && m_Menu.bBackMode)) {
					m_pMenu[Txt]->SetTxtBoxSize(0, SizeY);
				}
			}
			if (++m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME)
			{
				m_Menu.nCntLeftAnime = 0;

				if (!m_Menu.bBackMode)
					m_Menu.bAllElasticity = true;
				else if (m_Menu.bBackMode)
				{
					m_MenuEnd = true;
					TextRelease(TEXT_ALL);
				}
			}
		}
		else if (m_Menu.bAllElasticity && !m_MenuEnd)
		{
			int Txt = m_Menu.nMaineSelect;
			float TgtSizeX = m_pMenu[Txt]->GetTxtBoxTgtSize().x + 80.0f;
			float SizeY = m_pMenu[Txt]->GetTxtBoxTgtSize().y;
			float ScaleRate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntLeftAnime, PAUSE_LEFT_ANIME);
			float SizeX = TgtSizeX - (TgtSizeX * ScaleRate);

			m_pMenu[Txt]->SetTxtBoxSize(SizeX, SizeY);

			if (++m_Menu.nCntLeftAnime == PAUSE_LEFT_ANIME) {
				m_pMenu[Txt]->SetTxtBoxSize(0, SizeY);
				m_MenuEnd = true;
			}
		}
	}

	// 右画面のアニメーション
	if (m_Menu.bSubMenuMove && m_Menu.bSubMenuCD) {
		if (--m_Menu.nRightCoolDown <= 0) {

			m_Menu.bSubMenuCD = false;
			m_Menu.nRightCoolDown = COOLDOWN;

			if (!m_Menu.bClose && !m_Menu.bSubMenuDisp)
				m_Menu.BoxColor.a = 255;
		}
	}
	else if (m_Menu.bSubMenuMove && !m_Menu.bSubMenuCD && !m_Menu.bSubTextMenu || (m_Menu.bClose && m_Menu.bSubMenuDisp))
	{
		int nTextMax = 0;
		int nMainSelect = 0;

		if (!m_Menu.bClose && !m_Menu.bSubMenuDisp) nMainSelect = m_Menu.nMaineSelect;
		else if (m_Menu.bClose || m_Menu.bSubMenuDisp) nMainSelect = m_Menu.nSubMenuIdx;

		if (m_MaineMenu[nMainSelect].nSubMenuID == 0) { nTextMax = m_Menu.OperationMax; m_Menu.BoxTexIdx = 1; }
		else if (m_MaineMenu[nMainSelect].nSubMenuID == 1) { nTextMax = m_Menu.SettingMax; m_Menu.BoxTexIdx = 0; }

		D3DXVECTOR3 move = INITD3DXVECTOR3;

		float Rate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntRightAnime, PAUSE_RIGHT_ANIME);
		m_Menu.RightScale.x = m_Menu.RightScaleMax.x * Rate;
		m_Menu.RightScale.y = m_Menu.RightScaleMax.y * Rate;

		if (m_pSubMenu[0] != NULL) {
			m_Menu.SubMenuSize[0].x = m_Menu.SubMenuSizeMax[0].x * Rate;
			m_Menu.SubMenuSize[0].y = m_Menu.SubMenuSizeMax[0].y * Rate;

			m_pSubMenu[0]->SetTxtBoxSize(m_Menu.SubMenuSize[0].x, m_Menu.SubMenuSize[0].y);
		}

		int nAnime = 0;
		if (!m_Menu.bClose && !m_Menu.bSubMenuDisp) {
			++m_Menu.nCntRightAnime;
			nAnime = PAUSE_RIGHT_ANIME;

			if (m_Menu.nCntRightAnime >= nAnime)
				m_Menu.nCntRightAnime = nAnime;
		}
		else if (m_Menu.bClose || m_Menu.bSubMenuDisp) {
			--m_Menu.nCntRightAnime;
			nAnime = 0;

			if (m_Menu.nCntRightAnime <= nAnime)
				m_Menu.nCntRightAnime = nAnime;
		}

		if (m_Menu.nCntRightAnime == nAnime) {

			if (!m_Menu.bSubMenuDisp)
			{
				SubTextCreate();
				m_Menu.bSubTextMenu = true;
			}
			else if (m_Menu.bSubMenuDisp && !m_Menu.bClose && !m_Menu.bSubTextMenu) {
				m_Menu.bSubMenuDisp = false;
				m_Menu.bSubMenuMove = false;
				m_Menu.bSubMenuCreate = false;
				m_Menu.BoxColor.a = 0;
				TextRelease(TEXT_RIGHT);
			}
		}
	}
	else if ((m_Menu.bSubMenuMove && !m_Menu.bSubMenuCD && m_Menu.bSubTextMenu) || (m_Menu.bClose && m_Menu.bSubMenuDisp))
	{
		if (!m_Menu.bSubText)
		{
			float ScaleRate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntRightTextAnime, PAUSE_RIGHT_TEXT_ANIME);

			int nTextMax = 0;
			if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 0) nTextMax = m_Menu.OperationMax;
			else if (m_MaineMenu[m_Menu.nMaineSelect].nSubMenuID == 1) nTextMax = m_Menu.SettingMax;

			for (int nTxt = 1; nTxt < nTextMax; nTxt++)
			{
				if (m_pSubMenu[nTxt] != NULL)
				{
					float a = 255 * ScaleRate;
					m_pSubMenu[nTxt]->SetTxtBoxColor(Color{ 255,255,255,a });
				}
			}

			if (++m_Menu.nCntRightTextAnime > PAUSE_RIGHT_TEXT_ANIME) {
				m_Menu.nCntRightTextAnime = PAUSE_RIGHT_TEXT_ANIME;
				m_Menu.bSubMenuMove = false;
				m_Menu.bSubText = true;

				if (!m_Menu.bSubMenuDisp)
				{
					m_Menu.bSubMenuDisp = true;
					m_Menu.nSubMenuIdx = m_Menu.nMaineSelect;

					FontFont pFontTitle = { D3DXCOLOR(1.0f,1.0f,1.0f,0.0f),35.0f,3,1,-1, };
					FontFont pFontText = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };

					// テキストの再生成
					if (m_MaineMenu[m_Menu.nRightTextType].nSubMenuID == 0) {
						for (int nText = 0; nText < m_Menu.OperationMax; nText++) {
							if (m_pSubMenu[nText] != NULL)
							{
								if (nText != 0)
									m_pSubMenu[nText]->Regeneration(m_Menu.pOperation[nText].Text, CFont::FONT_WAKUWAKU, &pFontText);
								else
								{
									char data[TXT_MAX];
									sprintf(data, " %s", m_Menu.pOperation[nText].Text);

									m_pSubMenu[nText]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFontTitle);
								}
							}
						}
					}
					if (m_MaineMenu[m_Menu.nRightTextType].nSubMenuID == 1) {
						for (int nText = 0; nText < m_Menu.SettingMax; nText++) {
							if (m_pSubMenu[nText] != NULL) {

								char data[TXT_MAX];
								if (nText == SETTING_SCREEN_TEXT) {
									if (!m_Menu.bFullScreen)	sprintf(data, " OFF");
									else if (m_Menu.bFullScreen)	sprintf(data, " ON");
								}
								else if (nText == SETTING_BGM_TEXT) {
									int nData = m_Menu.nBGMVolume * 5;
									sprintf(data, " %d%s", nData, m_Menu.pSetting[nText].Text);
								}
								else if (nText == SETTING_SE_TEXT) {
									int nData = m_Menu.nSEVolume * 5;
									sprintf(data, " %d%s", nData, m_Menu.pSetting[nText].Text);
								}
								else
								{
									if (nText != 0 && nText != SETTING_BACK)
										sprintf(data, " %s", m_Menu.pSetting[nText].Text);
									else
										sprintf(data, "%s", m_Menu.pSetting[nText].Text);
								}

								if (nText != 0)
									m_pSubMenu[nText]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFontText);
								else
									m_pSubMenu[nText]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFontTitle);
							}
						}
					}
				}
			}
		}
		else if (m_Menu.bSubText)
		{
			float Rate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntRightTextAnime, PAUSE_RIGHT_TEXT_ANIME);

			int nTextMax = 0;
			if (m_MaineMenu[m_Menu.nSubMenuIdx].nSubMenuID == 0) nTextMax = m_Menu.OperationMax;
			else if (m_MaineMenu[m_Menu.nSubMenuIdx].nSubMenuID == 1) nTextMax = m_Menu.SettingMax;

			float size = 35.0f * Rate;
			if (m_pSubMenu[0] != NULL)
				m_pSubMenu[0]->SetTxtSize(size);
			for (int nTxt = 1; nTxt < nTextMax; nTxt++)
			{
				if (m_pSubMenu[nTxt] != NULL)
				{
					float a = 255 * Rate;
					m_pSubMenu[nTxt]->SetTxtBoxColor(Color{ 255,255,255,a });
					m_pSubMenu[nTxt]->SetTxtSize(size);
				}
			}

			if (--m_Menu.nCntRightTextAnime < 0)
			{
				m_Menu.nCntRightTextAnime = 0;
				m_Menu.bSubTextMenu = false;
				m_Menu.bSubText = false;
			}
		}
	}

	// メニュー終了時、
	if (m_MenuEnd && !m_Menu.bSubMenuCreate)
	{
		CMode::TYPE Mode = Manager::GetMode();
		if (Mode == CMode::TYPE::TITLE) {
			switch (m_Menu.nMaineSelect)
			{
			case TITLE_MENU_END:
				m_Menu.bGameEnd = true;
				break;
			}
		}
		else if (Mode == CMode::TYPE::GAME)
		{
			switch (m_Menu.nMaineSelect)
			{
			case PAUSE_MENU_RESET:
				Stage::SetIsReset(true);
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::NUI);
				Manager::EffectMgr()->DeleteAll();
				break;
			case PAUSE_MENU_SELECT:
				Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::NUI);
				CMode_Title::SetSelect(true);
				Manager::EffectMgr()->DeleteAll();
				break;
			}
		}

		TextRelease(TEXT_ALL);
	}
}

//========================================
// 一部の伸縮処理
//========================================
void CMenuUI::PartElasticity(void)
{
	if (m_Menu.bPartElasticity)
	{
		for (int Txt = 0; Txt < m_Menu.MainMenuMax; Txt++)
		{
			float TgtSizeX = m_pMenu[Txt]->GetTxtBoxTgtSize().x;
			float TgtSizeY = m_pMenu[Txt]->GetTxtBoxTgtSize().y;

			float TxtSizeX = m_pMenu[Txt]->GetTxtBoxSize().x;
			float TxtSizeY = m_pMenu[Txt]->GetTxtBoxSize().y;

			bool bChg = false;

			float Rate = RNLib::Ease().Easing(EASE_TYPE::INOUT_SINE, m_Menu.nCntLeftAnime, PAUSE_LEFT_ANIME);
			float SizeX = 0;
			float SizeY = 0;
			float size = 0;

			if (Txt == m_Menu.nMaineSelect && TxtSizeX < TgtSizeX)
			{
				bChg = true;
				SizeX = (TgtSizeX * 0.8f) + ((TgtSizeX * 0.2f) * Rate);
				SizeY = (TgtSizeY * 0.8f) + ((TgtSizeY * 0.2f) * Rate);
			}
			else if (Txt != m_Menu.nMaineSelect && TxtSizeX > (TgtSizeX * 0.8f))
			{
				bChg = true;
				SizeX = TgtSizeX - ((TgtSizeX * 0.2f) * Rate);
				SizeY = TgtSizeY - ((TgtSizeY * 0.2f) * Rate);
			}


			if (bChg)
			{
				if (m_pMenu[Txt] != NULL)
					m_pMenu[Txt]->SetTxtBoxSize(SizeX, SizeY);
			}
		}

		if (++m_Menu.nCntLeftAnime > PAUSE_LEFT_ANIME) {

			m_Menu.nCntLeftAnime = 0;
			m_Menu.bPartElasticity = false;
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
			m_Menu.nCntScrChg = 0;
	}

	// 選択・非選択
	for (int nCnt = 0; nCnt < m_Menu.MainMenuMax; nCnt++)
	{
		if (!m_Menu.bSubMenu) {
			if (m_pMenu[nCnt] != NULL) {
				if (nCnt == m_Menu.nMaineSelect)
				{
					m_pMenu[nCnt]->SetTxtBoxColor(Color{ 255,255,255,255 });
					m_pMenu[nCnt]->SetTxtColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
				else
				{
					m_pMenu[nCnt]->SetTxtBoxColor(Color{ 155,155,155,255 });
					m_pMenu[nCnt]->SetTxtColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
				}
			}
		}
	}
	for (int nCnt = 1; nCnt < m_Menu.SettingMax; nCnt++)
	{
		if (m_Menu.bSubMenu) {
			if (m_pSubMenu[nCnt] != NULL) {

				CFontText::Box BoxType = m_pSubMenu[nCnt]->GetTxtBoxType();

				if (nCnt == m_Menu.nSubSelect)
				{
					if (BoxType == CFontText::Box::BOX_SUB_MENU00)
						m_pSubMenu[nCnt]->SetTxtBoxType(CFontText::BOX_SUB_SELECTU00);
					else if (BoxType == CFontText::Box::BOX_SUB_MENU02)
						m_pSubMenu[nCnt]->SetTxtBoxType(CFontText::BOX_SUB_SELECTU02);

					if(nCnt == SETTING_SCREEN)
						m_pSubMenu[SETTING_SCREEN_TEXT]->SetTxtBoxType(CFontText::BOX_SUB_SELECTU01);
					else if (nCnt == SETTING_BGM)
						m_pSubMenu[SETTING_BGM_TEXT]->SetTxtBoxType(CFontText::BOX_SUB_SELECTU01);
					else if (nCnt == SETTING_SE)
						m_pSubMenu[SETTING_SE_TEXT]->SetTxtBoxType(CFontText::BOX_SUB_SELECTU01);
				}
				else
				{
					if (BoxType == CFontText::Box::BOX_SUB_SELECTU00)
						m_pSubMenu[nCnt]->SetTxtBoxType(CFontText::BOX_SUB_MENU00);
					else if (BoxType == CFontText::Box::BOX_SUB_SELECTU02)
						m_pSubMenu[nCnt]->SetTxtBoxType(CFontText::BOX_SUB_MENU00);
					else if (BoxType == CFontText::Box::BOX_SUB_SELECTU01 && 
						((nCnt == SETTING_SCREEN_TEXT && m_Menu.nSubSelect != SETTING_SCREEN) ||
						(nCnt == SETTING_BGM_TEXT && m_Menu.nSubSelect != SETTING_BGM) ||
						(nCnt == SETTING_SE_TEXT && m_Menu.nSubSelect != SETTING_SE)))
						m_pSubMenu[nCnt]->SetTxtBoxType(CFontText::BOX_SUB_MENU01);
				}
			}
		}
	}

	// メニュー選択
	SelectInput();

	FontFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };

	// サウンド
	if (m_Menu.nBGMVolume != m_Menu.nBGMOldVolume) {
		m_Menu.nBGMOldVolume = m_Menu.nBGMVolume;

		char data[TXT_MAX];		int nData = m_Menu.nBGMVolume * 5;
		sprintf(data, " %d%s", nData, m_Menu.pSetting[SETTING_BGM_TEXT].Text);

		if (m_pSubMenu[SETTING_BGM_TEXT] != NULL)
			m_pSubMenu[SETTING_BGM_TEXT]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFont);

		float volume = (float)nData / (float)100.0f;
		RNLib::Options().SetCategoryVolume(_RNC_Sound::CATEGORY::BGM, volume);
	}
	if (m_Menu.nSEVolume != m_Menu.nSEOldVolume) {
		m_Menu.nSEOldVolume = m_Menu.nSEVolume;

		char data[TXT_MAX];		int nData = m_Menu.nSEVolume * 5;
		sprintf(data, " %d%s", nData, m_Menu.pSetting[SETTING_SE_TEXT].Text);

		if (m_pSubMenu[SETTING_SE_TEXT] != NULL)
			m_pSubMenu[SETTING_SE_TEXT]->Regeneration(data, CFont::FONT_WAKUWAKU, &pFont);

		float volume = (float)nData / (float)100.0f;
		RNLib::Options().SetCategoryVolume(_RNC_Sound::CATEGORY::SE, volume);
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
