//========================================
// 
// モード:ゲームの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../_RNLib/RNlib.h"
#include "../../_RNLib/RNsettings.h"
#include "../../_RNLib/Basis/Mechanical/sound.h"
#include "../main.h"
#include "../Character/player.h"
#include "../System/StageEditor.h"
#include "../System/words/font-text.h"
#include "../Mode/mode_title.h"
#include "../UI/partsUI.h"
#include "../UI/coinUI.h"
#include "../System/BG-Editor.h"
#include "../Sound/ambient-sound-player.h"
#include "../resource.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Gameクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
const char* CMode_Game::TEXT_FILE = "data\\GAMEDATA\\TITLE\\MenuFile.txt";
static const int s_PlanetMaxSummon = 8;		// 出現する位置の最大数
static const int s_StarMaxSummon = 10;		// 出現する位置の最大数

//****************************************
// 静的変数定義
//****************************************
Color CMode_Game::m_BgColorUp = INITCOLOR;
Color CMode_Game::m_BgColorDown = INITCOLOR;
CPlayer *CMode_Game::s_pPlayer = NULL;
CPlayer* CMode_Game::GetPlayer(void) { return s_pPlayer; }
int CMode_Game::m_nStageIdx = 0;
int CMode_Game::m_nPlanetIdx = 0;
CRocketPartsUI *CMode_Game::m_rocketparts = NULL;
CCoinUI *CMode_Game::m_Coin = NULL;

//========================================
// コンストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::CMode_Game(void) {

	m_rocketparts = NULL;

	m_Pause.bFullScreen = RNSettings::GetInfo().isFullScreen;

	float BGM = RNLib::Sound().GetCategoryState(CSound::CATEGORY::BGM).settingVolume;
	float SE = RNLib::Sound().GetCategoryState(CSound::CATEGORY::SE).settingVolume;
	m_Pause.nBGMVolume = BGM * VOLUME_MSX;
	m_Pause.nSEVolume = SE * VOLUME_MSX;
	m_Pause.nBGMOldVolume = BGM * VOLUME_MSX;
	m_Pause.nSEOldVolume = SE * VOLUME_MSX;
}

//========================================
// デストラクタ
// Author:RIKU NISHIMURA
//========================================
CMode_Game::~CMode_Game(void) {

	// テキストの解放
	TextRelease(TEXT_ALL);

	// テキスト関連
	if (m_Pause.pOperation != NULL) {
		delete[] m_Pause.pOperation;
		m_Pause.pOperation = NULL;
	}

	if (m_Pause.pSetting != NULL) {
		delete[] m_Pause.pSetting;
		m_Pause.pSetting = NULL;
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

	// 状態設定
	SetState((int)STATE::NONE);

	// ステージオブジェクトの読み込み
	Manager::StageObjectMgr()->Load();

	// プレイヤーを生成
	if (s_pPlayer == NULL)
		s_pPlayer = CPlayer::Create();

	if (m_Coin == NULL)
		m_Coin = CCoinUI::Create();

	// ステージ生成
	Manager::StgEd()->StageLoad(m_nPlanetIdx, m_nStageIdx);

	{// [[[ カメラ ]]]
		// カメラの視点/注視点を設定
		Manager::GetMainCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		Manager::GetSubCamera()->SetPosVAndPosR(Manager::StgEd()->GetCameraPos(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	// 背景情報を読み込み
	char *pBgFile = Manager::StgEd()->GetBgFile();
	if (pBgFile != NULL) {
		CBGEditor::Load(pBgFile);
	}

	// テキストの初期化
	for (int nCnt = 0; nCnt < MENU_MAX; m_pMenu[nCnt] = NULL, nCnt++);
	for (int nCnt = 0; nCnt < FONT_TEXT_MAX; m_pSubMenu[nCnt] = NULL, nCnt++);

	// テキスト読込
	TextLoad();

	// 壁モデル読み込み
	m_wallModelIdx[0] = RNLib::Model().Load("data\\MODEL\\Wall_Left.x");
	m_wallModelIdx[1] = RNLib::Model().Load("data\\MODEL\\Wall_Right.x");

	// BGM読み込み&再生
	m_BGMIdx = RNLib::Sound().Load("data\\SOUND\\BGM\\STAGE1.wav");
	m_BGMID = RNLib::Sound().Play(m_BGMIdx, CSound::CATEGORY::BGM, 1.0f, true);

	// 環境音プレイヤーの開始処理
	AmbientSoundPlayer::Start();
}

//========================================
// 終了処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Uninit(void) {
	CMode::Uninit();

	// 再生停止
	RNLib::Sound().GetPlay(m_BGMID).Delete();

	// プレイヤーを解放
	if (s_pPlayer != NULL)	{
		s_pPlayer->Uninit();
		delete s_pPlayer;
		s_pPlayer = NULL;
	}

	// ステージオブジェクトと背景を解放
	Manager::StageObjectMgr()->ReleaseAll();
	Manager::BGMgr()->ReleaseAll();

	// ロケットパーツUIを解放
	if (m_rocketparts != NULL) {
		m_rocketparts->Uninit();
		delete m_rocketparts;
		m_rocketparts = NULL;
	}

	// コインUIを解放
	if (m_Coin != NULL) {
		m_Coin->Uninit();
		delete m_Coin;
		m_Coin = NULL;
	}

	// 環境音プレイヤーの終了処理
	AmbientSoundPlayer::End();
}

//========================================
// 更新処理
// Author:RIKU NISHIMURA
//========================================
void CMode_Game::Update(void) {
	CMode::Update();

	// 環境音プレイヤーの更新処理
	AmbientSoundPlayer::Update();

	if (CPlayer::GetZoomUpCounter() > 0) {
		if (Manager::StgEd()->GetPlanetIdx() == 0) {
			if (Manager::StgEd()->GetType()[0].nStageIdx == 0) {
				// [[[ 上下カメラ描画 ]]]
				const Pos2D windowCenterPos = RNLib::Window().GetCenterPos();
				const float windowWidth = RNLib::Window().GetWidth();
				const float windowHeight = RNLib::Window().GetHeight();
				const float windowHeightHalf = windowHeight * 0.5f;
				const float windowHeightHalf2 = windowHeightHalf * 0.5f;

				// 下
				RNLib::Polygon2D().Put(0, true)
					->SetPos(windowCenterPos + Pos2D(0.0f, windowHeightHalf2))
					->SetTexUV(Manager::GetSubCamera(), Pos2D(0.0f, 0.5f), Pos2D(1.0f, 0.5f), Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f))
					->SetSize(windowWidth, windowHeightHalf);
			}
		}
	}

	{// [[[ 背景描画 ]]]
		// 上
		RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_WILDERNESS])
			->SetVtxPos(Pos3D(-1024.0f, 512.0f, 700.0f), Pos3D(1024.0f, 512.0f, 700.0f), Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f))
			->SetBillboard(true);
		RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
			->SetTex(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_FOREST])
			->SetVtxPos(Pos3D(-400.0f, 100.0f + 32.0f, 200.0f), Pos3D(400.0f, 100.0f + 32.0f, 200.0f), Pos3D(-400.0f, 0.0f + 32.0f, 200.0f), Pos3D(400.0f, 0.0f + 32.0f, 200.0f))
			->SetBillboard(true);

		// 下
		RNLib::Polygon3D().Put(PRIORITY_BACKGROUND, INITMATRIX)
			->SetTexUV(CResources::TEXTURE_IDXES[(int)CResources::TEXTURE::BG_CAVE], Pos2D(0.0f, 1.0f), Pos2D(1.0f, 1.0f), Pos2D(0.0f, 0.0f), Pos2D(1.0f, 0.0f))
			->SetVtxPos(Pos3D(-1024.0f, 0.0f, 700.0f), Pos3D(1024.0f, 0.0f, 700.0f), Pos3D(-1024.0f, -512.0f, 700.0f), Pos3D(1024.0f, -512.0f, 700.0f))
			->SetBillboard(true);
	}

	// [[[ 壁モデル描画 ]]]
	RNLib::Model().Put(PRIORITY_BACKGROUND, m_wallModelIdx[0], Pos3D(-CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);
	RNLib::Model().Put(PRIORITY_BACKGROUND, m_wallModelIdx[1], Pos3D( CStageObject::SIZE_OF_1_SQUARE * 23, 0.0f, 0.0f), INITROT3D);

	// ロケットパーツの更新処理
	if (m_rocketparts != NULL) {
		m_rocketparts->Update();
	}

	if (m_Coin != NULL) {
		m_Coin->Update();
	}

	// [[[ 非ポーズ時の処理 ]]]
	if (m_state != (int)STATE::PAUSE) {

		// ポーズ
		if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START)) {
			SetState((int)STATE::PAUSE);
		}

		// プレイヤーの更新処理
		if (s_pPlayer != NULL)
			s_pPlayer->Update();
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

			m_Pause.BoxTex = RNLib::Texture().Load("data\\TEXTURE\\TextBox\\TextBox10.png");
		}break;
			// [[[ 終了処理 ]]]
		case PROCESS::UNINIT: {
			TextRelease(TEXT_ALL);
		}break;
			// [[[ 更新処理 ]]]
		case PROCESS::UPDATE: {

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, 0.0f), 0.0f, false)
				->SetSize(RNLib::Window().GetCenterX() * 2, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 0,0,0,120 });

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Pause.LeftPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
				->SetSize(450.0f, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 150,150,150,150 });

			RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Pause.RightPos.x, RNLib::Window().GetCenterPos().y, 100.0f), 0.0f, false)
				->SetSize(630.0f, RNLib::Window().GetCenterY() * 2)
				->SetCol(Color{ 150,150,150,150 });

			if (m_Pause.RightPos.x < 1800.0f)
			{
				RNLib::Polygon2D().Put(PRIORITY_UI, D3DXVECTOR3(m_Pause.RightPos.x, 400.0f, 100.0f), 0.0f, false)
					->SetSize(560.0f, 600.0f)
					->SetTex(m_Pause.BoxTex);
			}

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
// ポーズ生成の処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseCreate(void)
{
	m_Pause.LeftPos = D3DXVECTOR3(-340.0f, 0.0f, 0.0f);
	m_Pause.RightPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
	m_Pause.LeftTargetPos = D3DXVECTOR3(280.0f, 0.0f, 0.0f);
	m_Pause.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
	m_Pause.nCntLeftAnime = 0;
	m_Pause.nCntRightAnime = 0;
	m_Pause.nMaineSelect = 0;
	m_Pause.nMaineOldSelect = 0;
	m_Pause.nSubSelect = 1;
	m_Pause.bRightMove = false;
	m_Pause.bRightDisp = false;
	m_Pause.nRightCoolDown = COOLDOWN;
	m_Pause.bRightCoolDown = false;
	m_Pause.nRightTextType = 0;
	m_Pause.bMenu = false;
	m_Pause.bSubMenu = false;
	m_Pause.bClose = false;

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	for (int nText = MENU_RESUME; nText < MENU_MAX; nText++) {
		m_pMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
			D3DXVECTOR3(m_Pause.LeftPos.x - 20, 150.0f + (100.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
			"", CFont::FONT_ROND_B, &pFont);
	}
}

//========================================
// サブテキスト生成
// Author:KEISUKE OTONO
//========================================
void CMode_Game::SubTextCreate(void)
{
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,1,1,-1, };

	if (m_pSubMenu != NULL)
		TextRelease(TEXT_RIGHT);

	if (m_Pause.nMaineSelect == MENU_CONTROLLER) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Pause.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
			"", CFont::FONT_ROND_B, &pFont);

		for (int nText = 1; nText < m_Pause.OperationMax; nText++) {
			m_pSubMenu[nText] = CFontText::Create(CFontText::BOX_NORMAL_GRAY,
				D3DXVECTOR3(m_Pause.RightPos.x - 50, 100.0f + (50.0f * nText), 0.0f), D3DXVECTOR2(370.0f, 80.0f),
				"", CFont::FONT_ROND_B, &pFont, false, false);
		}
	}
	else if (m_Pause.nMaineSelect == MENU_SETTING) {
		m_pSubMenu[INPUT_TITLE] = CFontText::Create(
			CFontText::BOX_NORMAL_GREEN, D3DXVECTOR3(m_Pause.RightPos.x - 210, 50.0f, 0.0f), D3DXVECTOR2(175.0f, 70.0f),
			"", CFont::FONT_ROND_B, &pFont);

		pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),30.0f,1,1,-1, };

		for (int nText = 1; nText < m_Pause.SettingMax; nText++) {

			D3DXVECTOR3 pos = INITD3DXVECTOR3;
			D3DXVECTOR2 size = INITD3DXVECTOR2;

			// 位置
			if (nText <= SETTING_BGM)
				pos = D3DXVECTOR3(m_Pause.RightPos.x, 100.0f + (80.0f * nText), 0.0f);
			else if (nText == SETTING_SE)
				pos = D3DXVECTOR3(m_Pause.RightPos.x, 100.0f + (160.0f * ((nText - 2) + 1)), 0.0f);
			else if (nText == SETTING_BACK)
				pos = D3DXVECTOR3(m_Pause.RightPos.x + 190.0f, 650.0f, 0.0f);
			else if (nText == SETTING_BGM_TEXT)
				pos = D3DXVECTOR3(m_Pause.RightPos.x + 150.0f, 100.0f + (80.0f * (nText - 2)), 0.0f);
			else if (nText == SETTING_SE_TEXT)
				pos = D3DXVECTOR3(m_Pause.RightPos.x + 150.0f, 100.0f + (80.0f * ((nText - 2) + 1)), 0.0f);

			// サイズ
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

	m_Pause.nRightTextType = m_Pause.nMaineSelect;
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
		if (!m_Pause.bSubMenu) {
			if (m_pMenu[nCnt] != NULL) {
				if (nCnt == m_Pause.nMaineSelect)
					m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
				else
					m_pMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}

	for (int nCnt = 1; nCnt < m_Pause.SettingMax; nCnt++)
	{
		if (m_Pause.bSubMenu) {
			if (m_pSubMenu[nCnt] != NULL) {
				if (nCnt == m_Pause.nSubSelect)
					m_pSubMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_BLUE);
				else
					m_pSubMenu[nCnt]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
			}
		}
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::B) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::BACK))
	{
		if (m_Pause.bSubMenu) {
			m_Pause.bSubMenu = false;
			m_pMenu[m_Pause.nMaineSelect]->SetBoxColor(INITCOLOR);
			m_pSubMenu[m_Pause.nSubSelect]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::UP))
	{
		if (!m_Pause.bSubMenu)
			m_Pause.nMaineSelect--;
		else if (m_Pause.bSubMenu)
			m_Pause.nSubSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		if (!m_Pause.bSubMenu)
			m_Pause.nMaineSelect++;
		else if (m_Pause.bSubMenu)
			m_Pause.nSubSelect++;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::LEFT))
	{
		if (m_Pause.nSubSelect == SETTING_BGM) {
			m_Pause.nBGMVolume--;
		}
		else if (m_Pause.nSubSelect == SETTING_SE) {
			m_Pause.nSEVolume--;
		}
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::RIGHT))
	{
		if (m_Pause.nSubSelect == SETTING_BGM) {
			m_Pause.nBGMVolume++;
		}
		else if (m_Pause.nSubSelect == SETTING_SE) {
			m_Pause.nSEVolume++;
		}
	}

	// アニメーション
	if ((m_Pause.nMaineSelect == MENU_CONTROLLER || m_Pause.nMaineSelect == MENU_SETTING) && !m_Pause.bRightMove && !m_Pause.bRightDisp) {

		m_Pause.bRightMove = true;
		m_Pause.bRightCoolDown = true;
	}
	else if (m_Pause.nMaineSelect != m_Pause.nRightTextType && !m_Pause.bRightMove && m_Pause.bRightDisp && !m_Pause.bRightCoolDown) {

		m_Pause.bRightMove = true;
		m_Pause.bRightCoolDown = true;

	}
	else if (m_Pause.bRightMove && m_Pause.bRightDisp && m_Pause.bRightCoolDown) {
		if (m_Pause.nMaineSelect != m_Pause.nMaineOldSelect) {
			if (m_Pause.nMaineSelect == MENU_CONTROLLER || m_Pause.nMaineSelect == MENU_SETTING)
				m_Pause.nRightCoolDown = COOLDOWN;
		}
		else if (m_Pause.nMaineSelect == m_Pause.nRightTextType) {
			m_Pause.bRightMove = false;
			m_Pause.bRightCoolDown = false;
			m_Pause.nRightCoolDown = COOLDOWN;
		}
	}

	if (m_Pause.nMaineSelect != m_Pause.nMaineOldSelect) {
		m_Pause.nMaineOldSelect = m_Pause.nMaineSelect;
	}

	// ループ制御
	IntLoopControl(&m_Pause.nMaineSelect, MENU_MAX, 0);
	IntLoopControl(&m_Pause.nSubSelect, m_Pause.SettingMax-2, 1);
	IntControl(&m_Pause.nBGMVolume, VOLUME_MSX, 0);
	IntControl(&m_Pause.nSEVolume, VOLUME_MSX, 0);

	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	// サウンド
	if (m_Pause.nBGMVolume != m_Pause.nBGMOldVolume) {
		m_Pause.nBGMOldVolume = m_Pause.nBGMVolume;

		char data[TXT_MAX];		int nData = m_Pause.nBGMVolume * 5;
		sprintf(data, "%d%s", nData, m_Pause.pSetting[SETTING_BGM_TEXT].Text);

		if (m_pSubMenu[SETTING_BGM_TEXT] != NULL)
			m_pSubMenu[SETTING_BGM_TEXT]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

		float volume = (float)nData / (float)100.0f;
		RNLib::Sound().ChangeSetVolume(CSound::CATEGORY::BGM, volume);
	}
	if (m_Pause.nSEVolume != m_Pause.nSEOldVolume) {
		m_Pause.nSEOldVolume = m_Pause.nSEVolume;

		char data[TXT_MAX];		int nData = m_Pause.nSEVolume * 5;
		sprintf(data, "%d%s", nData, m_Pause.pSetting[SETTING_SE_TEXT].Text);

		if (m_pSubMenu[SETTING_SE_TEXT] != NULL)
			m_pSubMenu[SETTING_SE_TEXT]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

		float volume = (float)nData / (float)100.0f;
		RNLib::Sound().ChangeSetVolume(CSound::CATEGORY::SE, volume);
	}
}

//========================================
// ポーズアニメーションの処理
// Author:KEISUKE OTONO
//========================================
void CMode_Game::PauseAnime(void)
{
	// 左画面のアニメーション
	if (!m_Pause.bMenu || m_Pause.bClose)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		move.x = (m_Pause.LeftTargetPos.x - m_Pause.LeftPos.x) * 0.3f;

		m_Pause.LeftPos.x += move.x;
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{
			if (m_pMenu[nCnt] != NULL)
			{
				m_pMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
			}
		}

		if (++m_Pause.nCntLeftAnime == PAUSE_LEFT_ANIME) {
			m_Pause.LeftPos = m_Pause.LeftTargetPos;
			m_Pause.nCntLeftAnime = 0;
			if (!m_Pause.bClose){
				m_Pause.bMenu = true;

				FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
				FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

				m_pMenu[0]->Regeneration("続ける", CFont::FONT_ROND_B, &pFont,&pShadow);
				m_pMenu[1]->Regeneration("やり直す", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[2]->Regeneration("選択画面", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[3]->Regeneration("操作方法", CFont::FONT_ROND_B, &pFont, &pShadow);
				m_pMenu[4]->Regeneration("設定", CFont::FONT_ROND_B, &pFont, &pShadow);
			}
			else if (m_Pause.bClose){
				if (RNLib::Transition().GetState() == CTransition::STATE::NONE){
					SetState((int)STATE::NONE);
					TextRelease(TEXT_ALL);
				}
			}
		}
	}

	// 右画面のアニメーション
	if (m_Pause.bRightMove && m_Pause.bRightCoolDown) {
		if (--m_Pause.nRightCoolDown <= 0) {

			m_Pause.bRightCoolDown = false;
			m_Pause.nRightCoolDown = COOLDOWN;
			if (!m_Pause.bRightDisp) {
				SubTextCreate();
			}
			else if (m_Pause.bRightDisp && !m_Pause.bClose) {
				TextRelease(TEXT_RIGHT);
			}
		}
	}
	else if (m_Pause.bRightMove && !m_Pause.bRightCoolDown || (m_Pause.bClose && m_Pause.bRightDisp))
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;
		if (m_Pause.nMaineSelect != MENU_CONTROLLER && m_Pause.nMaineSelect != MENU_SETTING && !m_Pause.bRightDisp) {
			TextRelease(TEXT_RIGHT);
			m_Pause.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
			move.x = (m_Pause.RightTargetPos.x - m_Pause.RightPos.x) * 0.3f;
			m_Pause.RightPos.x += move.x;

			if (++m_Pause.nCntRightAnime == PAUSE_RIGHT_ANIME) {
				m_Pause.RightPos = m_Pause.RightTargetPos;
				m_Pause.bRightMove = false;
				m_Pause.nCntRightAnime = 0;
				m_Pause.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
			}
		}
		else {
			move.x = (m_Pause.RightTargetPos.x - m_Pause.RightPos.x) * 0.3f;

			m_Pause.RightPos.x += move.x;

			int nTextMax = -1;
			if (m_Pause.nMaineSelect == MENU_CONTROLLER) nTextMax = m_Pause.OperationMax;
			else if (m_Pause.nMaineSelect == MENU_SETTING) nTextMax = m_Pause.SettingMax;

			for (int nCnt = 0; nCnt < nTextMax; nCnt++) {
				if (m_pSubMenu[nCnt] != NULL) {
					m_pSubMenu[nCnt]->SetMove(D3DXVECTOR3(move.x, 0.0f, 0.0f));
				}
			}

			if (++m_Pause.nCntRightAnime == PAUSE_RIGHT_ANIME) {
				m_Pause.RightPos = m_Pause.RightTargetPos;
				m_Pause.bRightMove = false;
				m_Pause.nCntRightAnime = 0;

				if (!m_Pause.bRightDisp)
				{
					m_Pause.bRightDisp = true;
					m_Pause.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
					FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
					FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

					// テキストの再生成
					if (m_Pause.nRightTextType == MENU_CONTROLLER) {
						for (int nText = 0; nText < m_Pause.OperationMax; nText++) {
							if (m_pSubMenu[nText] != NULL)
								m_pSubMenu[nText]->Regeneration(m_Pause.pOperation[nText].Text, CFont::FONT_ROND_B, &pFont, &pShadow);
						}
					}
					if (m_Pause.nRightTextType == MENU_SETTING) {
						for (int nText = 0; nText < m_Pause.SettingMax; nText++) {
							if (m_pSubMenu[nText] != NULL) {

								char data[TXT_MAX];
								if (nText == SETTING_SCREEN) {
									if (!m_Pause.bFullScreen)	sprintf(data, "%s ：OFF", m_Pause.pSetting[nText].Text);
									else if (m_Pause.bFullScreen)	sprintf(data, "%s ：ON", m_Pause.pSetting[nText].Text);
								}
								else if (nText == SETTING_BGM_TEXT) {
									int nData = m_Pause.nBGMVolume * 5;
									sprintf(data, "%d%s", nData, m_Pause.pSetting[nText].Text);
								}
								else if (nText == SETTING_SE_TEXT) {
									int nData = m_Pause.nSEVolume * 5;
									sprintf(data, "%d%s", nData, m_Pause.pSetting[nText].Text);
								}
								else
									sprintf(data, "%s", m_Pause.pSetting[nText].Text);

								m_pSubMenu[nText]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);
							}
						}
					}
				}
				else if (m_Pause.bRightDisp && !m_Pause.bClose) {
					m_Pause.RightTargetPos = D3DXVECTOR3(900.0f, 0.0f, 0.0f);
					m_Pause.bRightDisp = false;
					TextRelease(TEXT_RIGHT);
				}
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
	if (m_Pause.nCntScrChg >= 0)
	{// 画面モード切り替えカウンターが0以上の時、
	 // 画面モード切り替えカウンターを減算
		m_Pause.nCntScrChg--;

		if (m_Pause.nCntScrChg == 0)
		{// 画面モード切り替えカウンターが0の時、
		 // ウインドウのモードを切り替える
			RNSettings::SetFulScreen(m_Pause.bFullScreen);
		}
	}

	if (RNLib::Input().GetTrigger(DIK_P, CInput::BUTTON::START) && !m_Pause.bClose)
	{
		m_Pause.bClose = true;
	}

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && !m_Pause.bClose)
	{
		if (!m_Pause.bSubMenu) {
			switch (m_Pause.nMaineSelect)
			{
			case MENU_RESUME:
				m_Pause.bClose = true;
				break;
			case MENU_RESET:
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::FADE);
				m_Pause.bClose = true;
				Manager::EffectMgr()->ReleaseAll();
				break;
			case MENU_SELECT:
				Manager::Transition(CMode::TYPE::TITLE, CTransition::TYPE::FADE);
				CMode_Title::SetSelect(true);
				Manager::EffectMgr()->ReleaseAll();
				break;
			case MENU_CONTROLLER:
				break;
			case MENU_SETTING:
				m_Pause.bSubMenu = true;
				m_pMenu[m_Pause.nMaineSelect]->SetBoxColor(Color{ 155,155,155,255 });
				break;
			}
		}
		else if (m_Pause.bSubMenu) {
			switch (m_Pause.nSubSelect)
			{
			case SETTING_SCREEN:
				if (m_Pause.nCntScrChg <= 0) {
					int nText = m_Pause.nSubSelect;
					m_Pause.bFullScreen = !m_Pause.bFullScreen;

					char data[TXT_MAX] = {};
					if (!m_Pause.bFullScreen)	sprintf(data, "%s ：OFF", m_Pause.pSetting[nText].Text);
					else if (m_Pause.bFullScreen)	sprintf(data, "%s ：ON", m_Pause.pSetting[nText].Text);

					FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),35.0f,3,1,-1, };
					FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };
					m_pSubMenu[nText]->Regeneration(data, CFont::FONT_ROND_B, &pFont, &pShadow);

					m_Pause.nCntScrChg = 20;
				}

				break;
			case SETTING_BGM:
				break;
			case SETTING_SE:
				break;
			case SETTING_BACK:
				m_Pause.bSubMenu = false;
				m_pMenu[m_Pause.nMaineSelect]->SetBoxColor(INITCOLOR);
				m_pSubMenu[m_Pause.nSubSelect]->SetBoxType(CFontText::BOX_NORMAL_GRAY);
				break;
			}
		}
	}

	if(m_Pause.bClose){ 
		m_Pause.LeftTargetPos *= -1;
		m_Pause.RightTargetPos = D3DXVECTOR3(1800.0f, 0.0f, 0.0f);
		m_Pause.nCntLeftAnime = 0;

		ProcessState(PROCESS::UNINIT);
	}
}

//========================================
// テキスト読込
// Author:KEISUKE OTONO
//========================================
void CMode_Game::TextLoad(void)
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
				if (!strcmp(aDataSearch, "EndOperation")) {
					break;
				}
				if (!strcmp(aDataSearch, "TextMax")) {
					int nMax = -1;

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%d", &nMax);

					if (nMax <= 0)
						nMax = 0;

					m_Pause.OperationMax = nMax;
					m_Pause.pOperation = new Operation[nMax];
					assert(m_Pause.pOperation != NULL);
				}
				if (!strcmp(aDataSearch, "Text")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Pause.pOperation[nText].Text);
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

					m_Pause.SettingMax = nMax;
					m_Pause.pSetting = new Setting[nMax];
					assert(m_Pause.pSetting != NULL);
				}
				if (!strcmp(aDataSearch, "Text")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &m_Pause.pSetting[nText].Text);
					nText++;
				}
			}
		}
	}
}

//========================================
// テキストの開放
// Author:KEISUKE OTONO
//========================================
void CMode_Game::TextRelease(TEXT type)
{
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
