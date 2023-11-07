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
bool CMode_Title::s_bStageSelect = false;

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

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_MenuPos[nCnt] = INITD3DXVECTOR3;
	}

	m_nSelect = 0;
	m_nOldSelect = 0;
	m_nPlanetIdx = 0;
	m_nOldnPlanet = 0;
	m_PlanetAngle = 0.0f;
	m_nSelect = 0;
	m_nOldSelect = 0;
	m_PlanetType = NULL;
	m_bMenuAnime = false;
	m_bBackMode = false;
	s_bStageSelect = false;
}

//========================================
// デストラクタ
// Author:KEISUKE OTONO
//========================================
CMode_Title::~CMode_Title(void) {

	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		if (m_Words[nCnt] != NULL)
		{
			m_Words[nCnt]->Uninit();
			m_Words[nCnt] = NULL;
		}
		if (m_WordsShadow[nCnt] != NULL)
		{
			m_WordsShadow[nCnt]->Uninit();
			m_WordsShadow[nCnt] = NULL;
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

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_Menu[nCnt] = {NULL};
	}
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		m_bMove[nCnt] = false;
		m_Words[nCnt] = NULL;
		m_WordsShadow[nCnt] = NULL;
	}

	if (s_bStageSelect)
	{
		SelectCreate();
		s_bStageSelect = false;
	}
	else
	{
		SwapMode(TITLE_PLANET);
	}

	m_bMenuAnime = false;
	m_bBackMode = false;
	m_bBackAnime = false;

	// テクスチャ
	m_BgPos[0] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, RNLib::Window().GetCenterPos().y, -100.0f);
	m_BgPos[1] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1460.0f, 0.0f);
	m_BgPos[2] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1460.0f, 0.0f);
	m_BgPos[3] = D3DXVECTOR3(50, 140.0f, 0.0f);
	m_nSelect = 0;

	// メニュー
	m_MenuPos[0] = D3DXVECTOR3(280.0f, 150.0f, 1.0f);
	m_MenuPos[1] = D3DXVECTOR3(280.0f, 300.0f, 1.0f);
	m_MenuPos[2] = D3DXVECTOR3(280.0f, 450.0f, 1.0f);

	m_TexIdx[0] = RNLib::Texture().Load("data\\TEXTURE\\BackGround\\Space.png");
	m_TexIdx[1] = RNLib::Texture().Load("data\\TEXTURE\\Planet\\blue.png");
	m_TexIdx[2] = RNLib::Texture().Load("data\\TEXTURE\\Planet\\Orange.png");
	m_TexIdx[3] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Arrow_00.png");

	// 遷移設定
	RNLib::Transition().Set(CTransition::STATE::OPEN, CTransition::TYPE::FADE);

	// カメラの視点/注視点を設定
	RNLib::Camera3D().SetGeometryInfo(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

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

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			m_Menu[nCnt]->Uninit();
			m_Menu[nCnt] = NULL;
		}
	}
}

//========================================
// 更新処理
// Author:KEISUKE OTONO
//========================================
void CMode_Title::Update(void) {
	CMode::Update();

	// 各モードの処理
	if (Title == TITLE_PLANET)
		PlanetAnime();
	else if (Title == TITLE_TITLE)
		TitleAnime();
	else if (Title == TITLE_OUTSET && m_bBackMode)
		TitleAnime();
	else if (Title == TITLE_MENU_ANIME)
		MenuAnime();
	else if (Title == TITLE_MENU)
		Menu();
	else if (Title == TITLE_SELECT)
		StageSelect();

	RNLib::Polygon2D().Put(m_BgPos[0], 0.0f, false)
		->SetSize(1280.0f, 720.0f)
		->SetCol(Color{ 255,255,255,255 })
		->SetTex(m_TexIdx[0]);

	if (Title == TITLE_OUTSET || Title == TITLE_MENU || Title == TITLE_SELECT)
	{
		if(Title == TITLE_OUTSET)
			m_PlanetAngle += -0.002f;
		else if (Title == TITLE_MENU)
			m_PlanetAngle += -0.002f;
		else if(Title == TITLE_SELECT)
			m_PlanetAngle += -0.002f;

		FloatLoopControl(&m_PlanetAngle, D3DX_PI, -D3DX_PI);
	}

	if (Title <= TITLE_MENU)
	{
		RNLib::Polygon2D().Put(m_BgPos[1], m_PlanetAngle, false)
			->SetSize(1300.0f, 1300.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(m_TexIdx[1]);

		RNLib::Polygon2D().Put(m_BgPos[2], m_PlanetAngle, false)
			->SetSize(600.0f, 600.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(m_TexIdx[2]);
	}

	if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::A)) && RNLib::Transition().GetState() == CTransition::STATE::NONE)
	{
		switch (Title)
		{
		case TITLE_OUTSET:
		{
			SwapMode(TITLE_MENU_ANIME);
		}
		break;
		case TITLE_MENU:
		{
			switch (m_nSelect)
			{
			case MENU_GAME:
				SelectCreate();
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
// 惑星アニメーション
// Author:KEISUKE OTONO
//========================================
void CMode_Title::PlanetAnime(void)
{
	if (m_BgPos[1].y >= 1000.0f)
	{
		D3DXVECTOR3 move = INITD3DXVECTOR3;

		move.y = -25.0f;

		m_BgPos[1] += move;
		if (m_BgPos[1].y <= 1000.0f)
		{
			move.y = 0.0f;
			m_BgPos[1].y = 1000;
			SwapMode(TITLE_TITLE);
		}
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
		if (m_Words[nCnt] != NULL)
		{
			D3DXVECTOR3 pos = m_Words[nCnt]->GetPos();

			if (!m_bBackMode)
			{
				if (pos.y <= 210.0f && m_bMove[nCnt])
				{
					D3DXVECTOR3 move;

					move.y = 15.0f;

					m_Words[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
					m_WordsShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

					if (pos.y >= 200.0f)
					{
						move.y = 0.0f;
						pos.y = 200;

						m_Words[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
						m_WordsShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));

						if (nCnt == WORDS_MAX - 1)
						{
							SwapMode(TITLE_OUTSET);
						}

					}
					else if (pos.y >= 20 && nCnt != WORDS_MAX - 1 && !m_bMove[nCnt + 1])
					{
						m_bMove[nCnt + 1] = true;
					}
				}
			}
		}

		if (m_bBackMode)
		{
			bool bReset1 = false;
			bool bReset2 = false;

			if (m_BgPos[1].x >= -640.0f)
			{
				D3DXVECTOR3 move = INITD3DXVECTOR3;
				move.x = -5.0f;

				m_BgPos[1] += move;

				if (m_BgPos[1].x <= -640.0f)
				{
					move.x = 0.0f;
					m_BgPos[1].x = -640.0f;
					bReset1 = true;
				}
			}
			if (m_BgPos[2].y >= -250.0f)
			{
				D3DXVECTOR3 move01 = INITD3DXVECTOR3;
				move01.y = -3.0f;

				m_BgPos[2] += move01;

				if (m_BgPos[2].y <= -250.0f)
				{
					move01.y = 0.0f;
					m_BgPos[2].y = -250.0f;
					bReset2 = true;
				}
			}

			if (bReset1 && bReset2)
			{
				m_bBackMode = false;
				SwapMode(TITLE_PLANET);
			}
		}
	}
}

//========================================
// メニュー演出
// Author:KEISUKE OTONO
//========================================
void CMode_Title::MenuAnime(void)
{
	if (!m_bMenuAnime)
	{
		for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
		{
			if (m_Words[nCnt] != NULL)
			{
				D3DXVECTOR3 pos = m_Words[nCnt]->GetPos();

				if (pos.y >= -60.0f && m_bMove[nCnt])
				{
					D3DXVECTOR3 move;

					move.y = -15.0f;

					m_Words[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
					m_WordsShadow[nCnt]->SetMove(D3DXVECTOR3(0.0f, move.y, 0.0f));
				}
			}
		}

		if (m_Menu[0] != NULL)
		{
			m_Menu[0]->Uninit();
			m_Menu[0] = NULL;
		}

		if (m_BgPos[1].y <= 1460.0f)
		{
			D3DXVECTOR3 move = INITD3DXVECTOR3;
			move.y = 25.0f;

			m_BgPos[1] += move;
			if (m_BgPos[1].y >= 1460.0f)
			{
				move.y = 0.0f;
				m_BgPos[1].y = 1460;
				m_bMenuAnime = true;

				m_BgPos[1] = D3DXVECTOR3(-640.0f, 600.0f, 0.0f);
				m_BgPos[2] = D3DXVECTOR3(1240.0f, -250.0f, 0.0f);
			}
		}
	}
	else if (m_bMenuAnime)
	{
		if (m_BgPos[1].x <= 120.0f)
		{
			D3DXVECTOR3 move = INITD3DXVECTOR3;
			move.x = 40.0f;

			m_BgPos[1] += move;

			if (m_BgPos[1].x >= 120.0f)
			{
				move.x = 0.0f;
				m_BgPos[1].x = 120.0f;

				MenuCreate();
			}
		}
		if (m_BgPos[2].y <= 0)
		{
			D3DXVECTOR3 move01 = INITD3DXVECTOR3;
			move01.y = 10.0f;

			m_BgPos[2] += move01;

			if (m_BgPos[2].y >= 0.0f)
			{
				move01.y = 0.0f;
				m_BgPos[2].y = 0.0f;
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
	FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),45.0f,5,10,-1};// 45
	FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f), true, D3DXVECTOR3(4.0f,4.0f,0.0f), D3DXVECTOR2(4.0f,4.0f) };

	SwapMode(TITLE_MENU);

	m_Menu[0] = CFontText::Create(
		CFontText::BOX_NORMAL_RECT, m_MenuPos[0],D3DXVECTOR2(360.0f, 80.0f),// 360,100
		"ゲーム",CFont::FONT_ROND_B,&pFont,false,false,&pShadow);

	m_Menu[1] = CFontText::Create(
		CFontText::BOX_NORMAL_RECT, m_MenuPos[1], D3DXVECTOR2(360.0f, 80.0f),
		"オプション", CFont::FONT_ROND_B, &pFont, false, false, &pShadow);

	m_Menu[2] = CFontText::Create(
		CFontText::BOX_NORMAL_RECT, m_MenuPos[2], D3DXVECTOR2(360.0f, 80.0f),
		"ゲームをやめる", CFont::FONT_ROND_B, &pFont, false, false, &pShadow);
}

//========================================
// メニュー
// Author:KEISUKE OTONO
//========================================
void CMode_Title::Menu(void)
{
	// 色
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			if (nCnt == m_nSelect)
			{
				m_Menu[nCnt]->SetTexBox(true);
				m_Menu[nCnt]->SetTextColor(D3DXCOLOR(240,255,0,255));
			}
			else
			{
				m_Menu[nCnt]->SetTexBox(false);
				m_Menu[nCnt]->SetTextColor(INITD3DCOLOR);
			}
		}
	}

	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::BACK))
	{
		m_bBackMode = true;
		SwapMode(TITLE_OUTSET);
		return;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_W) || RNLib::Input().GetKeyTrigger(DIK_UP) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::UP) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT,CInput::INPUT_ANGLE::UP))
	{
		m_nSelect--;
	}
	else if (RNLib::Input().GetKeyTrigger(DIK_S) || RNLib::Input().GetKeyTrigger(DIK_DOWN) || RNLib::Input().GetButtonTrigger(CInput::BUTTON::DOWN) || RNLib::Input().GetStickAngleTrigger(CInput::STICK::LEFT, CInput::INPUT_ANGLE::DOWN))
	{
		m_nSelect++;
	}

	// ループ制御
	IntLoopControl(&m_nSelect, MENU_MAX, 0);

	m_BgPos[3].y = m_MenuPos[m_nSelect].y;

	RNLib::Polygon2D().Put(m_BgPos[3], 0.0f, false)
		->SetSize(75.0f, 75.0f)
		->SetCol(Color{ 255,255,255,255 })
		->SetTex(m_TexIdx[3]);
}

//========================================
// ステージ選択の生成
// Author:KEISUKE OTONO
//========================================
void CMode_Title::SelectCreate(void)
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

	SwapMode(TITLE_SELECT);
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
	RNLib::Model().Put(D3DXVECTOR3(0.0f, -4.0f, 0.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), 0, false)
		->SetModel(m_PlanetType[m_nPlanetIdx].nModel)
		->SetPriority(1);

	// 矢印
	if ((m_nPlanetIdx == 0 && m_nSelect != 0) || (m_nPlanetIdx != 0))
		RNLib::Polygon2D().Put(D3DXVECTOR3(400.0f, 550.0f, 0.0), 0.0f, false)
		->SetSize(100.0f, 100.0f)
		->SetCol(Color{ 50,255,0,255 })
		->SetTex(nPrevTex)
		->SetPriority(1);

	if ((m_nPlanetIdx != nPlanetMax-1) || (m_nPlanetIdx == nPlanetMax-1 && m_nSelect != nStageMax-1))
		RNLib::Polygon2D().Put(D3DXVECTOR3(880.0f, 550.0f, 0.0), 0.0f, false)
		->SetSize(100.0f, 100.0f)
		->SetCol(Color{ 50,255,0,255 })
		->SetTex(nNextTex)
		->SetPriority(1);

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

		RNLib::Polygon2D().Put(pos, 0.0f, false)
			->SetSize(40.0f, 40.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(nTexIdx)
			->SetPriority(1);
	}

	bool bInput = false;
	// -- メニュー選択 ---------------------------
	if (RNLib::Input().GetTrigger(DIK_BACKSPACE, CInput::BUTTON::BACK))
	{
		TextClear();
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

			m_Menu[1]->Uninit();
			m_Menu[1] = NULL;
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };
			m_Menu[1] = CFontText::Create(
				CFontText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f),
				Manager::StgEd()->GetType()[m_nPlanetIdx].StageType[m_nSelect].aName, CFont::FONT_ROND_B, &pFont);
		}
		if (m_nPlanetIdx != m_nOldnPlanet)
		{
			m_nOldnPlanet = m_nPlanetIdx;
			m_PlanetAngle = 0.0f;

			m_Menu[0]->Uninit();
			m_Menu[0] = NULL;
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };
			m_Menu[0] = CFontText::Create(
				CFontText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(360.0f, 70.0f),
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
	switch (aTitle)
	{
	case CMode_Title::TITLE_PLANET:
	{
		TextClear();
		for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
		{
			m_bMove[nCnt] = false;
		}

		m_BgPos[1] = D3DXVECTOR3(RNLib::Window().GetCenterPos().x, 1360.0f, 0.0f);
	}
		break;
	case CMode_Title::TITLE_TITLE:
	{
		{
			m_WordsShadow[0] = CWords::Create("Ｃ", D3DXVECTOR3(786.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_WordsShadow[1] = CWords::Create("Ｏ", D3DXVECTOR3(946.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_WordsShadow[2] = CWords::Create("Ｓ", D3DXVECTOR3(1096.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_WordsShadow[3] = CWords::Create("／", D3DXVECTOR3(1246.0f, -54.0f, 0.0f), D3DXVECTOR3(100.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_WordsShadow[4] = CWords::Create("Ｍ", D3DXVECTOR3(1406.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_WordsShadow[5] = CWords::Create("Ｏ", D3DXVECTOR3(1566.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
			m_WordsShadow[6] = CWords::Create("Ｓ", D3DXVECTOR3(1706.0f, -52.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));
		}
		{
			m_Words[0] = CWords::Create("Ｃ", D3DXVECTOR3(780.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_Words[1] = CWords::Create("Ｏ", D3DXVECTOR3(940.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_Words[2] = CWords::Create("Ｓ", D3DXVECTOR3(1090.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_Words[3] = CWords::Create("／", D3DXVECTOR3(1234.0f, -66.0f, 0.0f), D3DXVECTOR3(100.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.2f, 0.8f, 0.5f, 1.0f));
			m_Words[4] = CWords::Create("Ｍ", D3DXVECTOR3(1400.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_Words[5] = CWords::Create("Ｏ", D3DXVECTOR3(1560.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
			m_Words[6] = CWords::Create("Ｓ", D3DXVECTOR3(1700.0f, -60.0f, 0.0f), D3DXVECTOR3(125.0f, 125.0f, 0.0f), CFont::FONT_ROND_B, D3DXCOLOR(0.8f, 0.2f, 0.4f, 1.0f));
		}
		m_bMove[0] = true;
	}
		break;
	case CMode_Title::TITLE_OUTSET:
	{
		if (!m_bBackMode)
		{
			FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),70.0f,5,10,-1, };// 45
			FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),true, D3DXVECTOR3(6.0f,6.0f,0.0f) ,D3DXVECTOR2(4.0f,4.0f) };

			m_Menu[0] = CFontText::Create(CFontText::BOX_NORMAL_RECT, D3DXVECTOR3(230.0f, 600.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
				"ボタンを押して始めてね", CFont::FONT_ROND_B, &pFont, true, false, &pShadow);
		}
		else if (m_bBackMode)
		{
			TextClear();
			m_bBackAnime = false;
		}
	}
		break;
	case CMode_Title::TITLE_MENU_ANIME:
	{
		m_bMenuAnime = false;
		m_BgPos[1] = D3DXVECTOR3(-640.0f, 1000.0f, 0.0f);
		m_BgPos[2] = D3DXVECTOR3(1240.0f, -250.0f, 0.0f);
	}
		break;
	case CMode_Title::TITLE_MENU:
	{
		TextClear();
		m_nSelect = 0;
	}
		break;
	case CMode_Title::TITLE_SELECT:
	{
		TextClear();
		m_nSelect = 0;
		m_nOldSelect = 0;
		m_nPlanetIdx = 0;
		m_nOldnPlanet = 0;

		s_bStageSelect = false;

		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };// 45
		m_Menu[0] = CFontText::Create(
			CFontText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 50.0f, 0.0f), D3DXVECTOR2(360.0f, 70.0f),
			m_PlanetType[0].Text, CFont::FONT_ROND_B, &pFont);

		m_Menu[1] = CFontText::Create(
			CFontText::BOX_NORMAL_RECT, D3DXVECTOR3(640.0f, 550.0f, 0.0f), D3DXVECTOR2(400.0f, 80.0f),
			Manager::StgEd()->GetType()[0].StageType[0].aName, CFont::FONT_ROND_B, &pFont);
	}
		break;
	case CMode_Title::TITLE_NEXT:
		TextClear();
		break;
	}

	Title = aTitle;
}

//========================================
// テキスト削除
// Author:KEISUKE OTONO
//========================================
void CMode_Title::TextClear(void)
{
	for (int nCnt = 0; nCnt < WORDS_MAX; nCnt++)
	{
		if (m_Words[nCnt] != NULL)
		{
			m_Words[nCnt]->Uninit();
			m_Words[nCnt] = NULL;
		}

		if (m_WordsShadow[nCnt] != NULL)
		{
			m_WordsShadow[nCnt]->Uninit();
			m_WordsShadow[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_Menu[nCnt] != NULL)
		{
			m_Menu[nCnt]->Uninit();
			m_Menu[nCnt] = NULL;
		}
	}
}