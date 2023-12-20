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
#include "../Sound/title-sound.h"
#include "../Object/Item/coin.h"
#include "../UI/MenuUi.h"
#include "../System/words/words.h"
#include "../System/words/font-text.h"
#include "../resource.h"
#include "../stage.h"
#include "../UI/coinUI.h"
#include "../../_RNLib/Basis/3DObject/rail3D.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CMode_Titleクラス
//----------|---------------------------------------------------------------------
//================================================================================
const char* CMode_Title::TITLE_LOGO_FILE = "data\\GAMEDATA\\TitleLogo.txt";
const D3DXVECTOR3 SELECTBOX = D3DXVECTOR3(7.5f, -15.0f, -125.0f);
const D3DXVECTOR3 UNSELECTBOX = D3DXVECTOR3(7.5f, -20.0f, -120.0f);
const D3DXVECTOR3 SELBOXRATE = SELECTBOX - UNSELECTBOX;
const D3DXVECTOR3 NUMPOSSELBOX = D3DXVECTOR3(15.0f, 0.0f, 0.0f);
const D3DXVECTOR3 NUMPOSROCKET = D3DXVECTOR3(0.0f, 15.0f, 0.0f);
const D3DXVECTOR3 FADEROCKET = D3DXVECTOR3(70.0f, UNSELECTBOX.y, UNSELECTBOX.z) + NUMPOSROCKET;
const D3DXVECTOR3 COINUIPOS = D3DXVECTOR3(25.0f, 16.7f, -136.0f);
const D3DXVECTOR3 IMAGE_STG_POS = D3DXVECTOR3(-40.0f, 8.0f, -110.0f);
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
CMode_Title::CMode_Title(void) : m_RocketRail("data\\RAIL3D\\rocket.txt") {

	for (int nCnt = 0; nCnt < TEX_MAX; nCnt++) {
		m_BgPos[nCnt] = INITD3DXVECTOR3;
		m_TexIdx[nCnt] = 0;
	}

	Title              = TITLE_TITLE;
	NextTitle		   = TITLE_TITLE;
	TitleAnima		   = ANIME_NUI;
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
	m_EffTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");
	m_AnimCnt = NULL;
	m_RotCnt = 0;
	m_bStgEnter = false;
	m_bStageChange = false;
	m_bRocketMove = false;
	m_bRocketRot = false;
}

//========================================
// デストラクタ
//========================================
CMode_Title::~CMode_Title(void) {

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
	Manager::Transition().Open(CTransition::TYPE::NUI, 60);

	//BGM開始処理
	titleSound::Start();

	// タイトルロゴの初期化
	TitleLogoInit(false);

	// タイトルロゴの読込
	TitleLoad();

	// テキストの初期化
	m_pMenu = NULL;

	// メニュー生成
	m_MenuUI = CMenuUI::Create(CMode::TYPE::TITLE );

	// ステージ読込
	CreateStageSelectInfo();

	if (m_bStageSelect) 
	{// ステージ選択時、
		TitleLogoInit(true);
		SwapMode(TITLE_SELECT);
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
	m_TexIdx[3] = RNLib::Texture().Load("data\\TEXTURE\\StageSelect\\Lock.png");

	// カメラの視点/注視点を設定
	Manager::GetMainCamera()->SetPosVAndPosR(D3DXVECTOR3(0.0f, 0.0f, -200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_CoinUI == NULL) {
		m_CoinUI = CCoinUI::Create(COINUIPOS,Scale2D(4.0f,4.0f),false, D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f));
	}

	// カメラのライト設定
	Manager::GetMainCamera()->SetLightID(Manager::GetLightIdx(0));

	// 状態設定
	SetState((int)STATE::NONE);
}

//========================================
// 終了処理
//========================================
void CMode_Title::Uninit(void) {
	CMode::Uninit();

	if (m_CoinUI != NULL) {
		delete m_CoinUI;
		m_CoinUI = NULL;
	}

	RNLib::Memory().Release(&m_AnimCnt);

	//BGM終了処理
	titleSound::End();
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
		// タイトルロゴ
		for (int nCnt = 0; nCnt < TITLE_LOGO_MAX; nCnt++) {

			RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, false)
				->SetCol(m_TitleLogo.Logo[nCnt].color)
				->SetTexUV(m_TitleLogo.Logo[nCnt].TexIdx,
					m_TitleLogo.Logo[nCnt].TexUV[0], m_TitleLogo.Logo[nCnt].TexUV[1],
					m_TitleLogo.Logo[nCnt].TexUV[2], m_TitleLogo.Logo[nCnt].TexUV[3])
				->SetVtxPos(
					m_TitleLogo.Logo[nCnt].VtxPos[0], m_TitleLogo.Logo[nCnt].VtxPos[1],
					m_TitleLogo.Logo[nCnt].VtxPos[2], m_TitleLogo.Logo[nCnt].VtxPos[3]);
		}

		RNLib::Polygon2D().Put(PRIORITY_BACKGROUND, m_BgPos[TEX_PLANET], m_PlanetAngle, false)
			->SetSize(1400.0f, 1400.0f)
			->SetCol(Color{ 255,255,255,255 })
			->SetTex(m_TexIdx[TEX_PLANET]);

		// ロケット
		Matrix baseMtx = RNLib::Matrix().ConvPosRotToMtx(D3DXVECTOR3(60.0f, -40.0f, -20.0f), D3DXVECTOR3(0.0f, D3DX_PI, 1.9f));
		Matrix rocketMtx = RNLib::Matrix().ConvRotToMtx(D3DXVECTOR3(0.0f, ((RNLib::Count().GetCount() % 60) / 60.0f) * D3DX_PI_DOUBLE, 0.0f));
		RNLib::Model().Put(PRIORITY_OBJECT, m_RocketIdx, RNLib::Matrix().MultiplyMtx(rocketMtx, baseMtx), false)
			->SetOutLineIdx(5);
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

		if (m_bStageChange == false && m_bStgEnter == false) {
			if (m_bRocketMove == false) {
				if ((RNLib::Input().GetKeyTrigger(DIK_RETURN) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::A)) && Manager::Transition().GetState() == CTransition::STATE::NONE)
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
						if(m_Direction == RIGHT)
							m_RocketPos = FADEROCKET;
						else if (m_Direction == LEFT)
							m_RocketPos = Pos3D(-FADEROCKET.x, FADEROCKET.y, FADEROCKET.z);

						m_RocketposDiff = m_RocketPos - m_RocketPosOld;
						m_RocketAnimCnt = 0;
						m_nStgStartCnt = 0;
						m_bStgEnter = true;
					}
					break;
					}
				}
			}
		}

		if (m_nStgStartCnt == m_RocketRail.GetPointNum() * 10) {
			bool bStgRel = Manager::StgEd()->GetStageRel(m_nPlanetIdx, m_nStageSelect);

			if (!bStgRel || RNSystem::GetMode() == RNSystem::MODE::DEBUG)
			{
				SwapMode(TITLE_NEXT);
				Stage::SetStageNumber(m_nPlanetIdx, m_nStageSelect);
				Manager::Transition(CMode::TYPE::GAME, CTransition::TYPE::NUI);

				if (m_PlanetType != NULL)
				{
					delete[] m_PlanetType;
					m_PlanetType = NULL;
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
	if (TitleAnima == ANIME_NUI)
	{
		D3DXVECTOR3 CenterPos = m_TitleLogo.Logo[LOGO_NUI].pos;
		D3DXVECTOR2 size = m_TitleLogo.Logo[LOGO_NUI].size;
		float DownPosY = m_TitleLogo.Logo[LOGO_NUI].pos.y + size.y;

		float Rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_TitleLogo.nCntAnime, NUI_ANIME);
		float ScaleY = (m_TitleLogo.Logo[LOGO_NUI].size.y * 2) * Rate;

		m_TitleLogo.Logo[LOGO_NUI].VtxPos[0].y = DownPosY - ScaleY;
		m_TitleLogo.Logo[LOGO_NUI].VtxPos[1].y = DownPosY - ScaleY;

		if (++m_TitleLogo.nCntAnime > NUI_ANIME){
			m_TitleLogo.nCntAnime = 0;
			TitleAnima = ANIME_TEXT1;
		}
	}
	else if (TitleAnima == ANIME_TEXT1)
	{
		for (int nCnt = 0; nCnt < TITLE_LOGO_MAX; nCnt++)
		{
			if (nCnt == LOGO_NUI || nCnt == LOGO_TM)	continue;

			D3DXVECTOR3 CenterPos = m_TitleLogo.Logo[nCnt].pos;
			D3DXVECTOR2 size = m_TitleLogo.Logo[nCnt].size;
			float RightPosX = m_TitleLogo.Logo[nCnt].pos.x + size.x;
			float LeftPosX = m_TitleLogo.Logo[nCnt].pos.x - size.x;
			float Rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_TitleLogo.nCntAnime, TITLE_TEXT_ANIME);
			float PosRateX = 0.0f;
			float TexRateX = 1 * Rate;

			// 割合
			if (nCnt == LOGO_COS1 || nCnt == LOGO_COS2)
			{
				PosRateX = (m_TitleLogo.Logo[nCnt].size.x * 2.0f) * Rate;
			}
			else if (nCnt == LOGO_MOS1 || nCnt == LOGO_MOS2)
			{
				PosRateX = (m_TitleLogo.Logo[nCnt].size.x * 2.0f) * Rate;
			}

			// 設定
			if (nCnt == LOGO_COS1 || nCnt == LOGO_COS2)
			{
				m_TitleLogo.Logo[nCnt].VtxPos[0].x = RightPosX - PosRateX;
				m_TitleLogo.Logo[nCnt].VtxPos[2].x = RightPosX - PosRateX;

				m_TitleLogo.Logo[nCnt].TexUV[1].x = TexRateX;
				m_TitleLogo.Logo[nCnt].TexUV[3].x = TexRateX;
			}
			else if (nCnt == LOGO_MOS1 || nCnt == LOGO_MOS2)
			{
				m_TitleLogo.Logo[nCnt].VtxPos[1].x = LeftPosX + PosRateX;
				m_TitleLogo.Logo[nCnt].VtxPos[3].x = LeftPosX + PosRateX;
				 
				m_TitleLogo.Logo[nCnt].TexUV[0].x = (1 - TexRateX);
				m_TitleLogo.Logo[nCnt].TexUV[2].x = (1 - TexRateX);
			}
			
		}

		if (++m_TitleLogo.nCntAnime > TITLE_TEXT_ANIME)
		{
			m_TitleLogo.nCntAnime = 0;
			TitleAnima = ANIME_NUI_LEAN;
		}
	}
	else if (TitleAnima == ANIME_NUI_LEAN)
	{
		D3DXVECTOR2 size = m_TitleLogo.Logo[LOGO_NUI].size;
		float Rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_TitleLogo.nCntAnime, NUI_ANIME);
		float PosRate = 20 * Rate;
		float AngleRate = -0.3f * Rate;

		// 位置
		m_TitleLogo.Logo[LOGO_NUI].pos.x = m_TitleLogo.Logo[LOGO_NUI].InitPos.x + PosRate;
		D3DXVECTOR3 CenterPos = m_TitleLogo.Logo[LOGO_NUI].pos;

		// 角度
		m_TitleLogo.Logo[LOGO_NUI].angle = AngleRate;

		if (++m_TitleLogo.nCntAnime > NUI_ANIME) {
			m_TitleLogo.nCntAnime = 0;
			TitleAnima = ANIME_TM;
		}

		CenterPos.y = m_TitleLogo.Logo[LOGO_NUI].InitPos.y + (size.y * 0.025f);

		{
			// 対角線の長さと向き
			const float length = sqrtf(((size.x * 2) * (size.x * 2)) + ((size.y * 2) * (size.y * 2))) * 0.5f;
			const float vtxAngle = atan2f((size.x * 2), (size.y * 2));
			const float inverseVtxAngle = D3DX_PI - vtxAngle;

			// 頂点座標を設定
			{
				float angle = m_TitleLogo.Logo[LOGO_NUI].angle;
				float resultAngle = angle - inverseVtxAngle;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[0].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[0].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle + inverseVtxAngle;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[1].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[1].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle - vtxAngle;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[2].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[2].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle + vtxAngle;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[3].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_NUI].VtxPos[3].y = CenterPos.y + cosf(resultAngle) * length;
			}
		}			
	}
	else if (TitleAnima == ANIME_TM)
	{
		D3DXVECTOR3 CenterPos = m_TitleLogo.Logo[LOGO_TM].pos;
		D3DXVECTOR2 size = m_TitleLogo.Logo[LOGO_TM].size;
		float Rate1 = RNLib::Ease().Easing(_RNC_Ease::TYPE::INOUT_SINE, m_TitleLogo.nCntAnime, NUI_ANIME);
		float Rate2 = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_TitleLogo.nCntAnime, NUI_ANIME);
		float SizeRateX = (size.x * 2) * Rate1;
		float SizeRateY = (size.y * 2) * Rate1;
		float ColorRate = 255 * Rate2;

		m_TitleLogo.Logo[LOGO_TM].color.a = ColorRate;

		if (++m_TitleLogo.nCntAnime > NUI_ANIME) {
			TitleAnima = ANIME_NONE;
			SwapMode(TITLE_OUTSET);
			m_TitleLogo.nCntAnime = 0;
		}

		{
			// 対角線の長さと向き
			const float length = sqrtf((SizeRateX * SizeRateX) + (SizeRateY * SizeRateY)) * 0.5f;
			const float vtxAngle = atan2f(SizeRateX, SizeRateY);
			const float inverseVtxAngle = D3DX_PI - vtxAngle;

			// 頂点座標を設定
			{
				float angle = m_TitleLogo.Logo[LOGO_TM].angle;
				float resultAngle = angle - inverseVtxAngle;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[0].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[0].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle + inverseVtxAngle;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[1].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[1].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle - vtxAngle;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[2].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[2].y = CenterPos.y + cosf(resultAngle) * length;
				resultAngle = angle + vtxAngle;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[3].x = CenterPos.x + sinf(resultAngle) * length;
				m_TitleLogo.Logo[LOGO_TM].VtxPos[3].y = CenterPos.y + cosf(resultAngle) * length;
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
	RNLib::Memory().Alloc(&m_AnimCnt,nStageMax);

	const Pos3D PosCor = Pos3D(nStageMax * (NUMPOSSELBOX.x * 0.5f), 0.0f, 0.0f);
	m_RocketPos = m_RocketPosOld = UNSELECTBOX - PosCor + NUMPOSROCKET;
	m_RocketposDiff = INITD3DXVECTOR3;
	m_RocketRot = m_RocketRotOld = D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * 0.5f);
	m_RocketRotDiff = INITD3DXVECTOR3;
	m_RotCnt = ANIMCOUNT;
	m_nDrawPlanet = m_nPlanetIdx;
	m_RocketAnimCnt = 0;
	m_ImageStgCnt = 0;
	m_NumAnimCnt = ANIMCOUNT * 0.5f;
	m_Direction = RIGHT;
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

	StageRel(m_nPlanetIdx, nStageMax);

	//描画処理
	StageDraw(nPlanetMax, nStageMax, PosCor,RocketAnimRate);

	//----------------------------------------
	// ステージ選択処理
	//----------------------------------------
	if(m_bStageChange == false)
	m_nOldSelect = m_nStageSelect;
	bool bInput = false;

	if (m_bStageChange == false && !m_bStgEnter) {
		if (m_bRocketMove == false) {

			if (RNLib::Input().GetTrigger(DIK_BACKSPACE, _RNC_Input::BUTTON::B) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::BACK)) {
				TextRelease(TEXT_MENU);
				SwapMode(TITLE_MENU_ANIME);
				return;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_A) || RNLib::Input().GetKeyTrigger(DIK_LEFT) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::LEFT) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::LEFT)) {
				m_nStageSelect--;
				bInput = true;
			}
			else if (RNLib::Input().GetKeyTrigger(DIK_D) || RNLib::Input().GetKeyTrigger(DIK_RIGHT) || RNLib::Input().GetButtonTrigger(_RNC_Input::BUTTON::RIGHT) || RNLib::Input().GetStickAngleTrigger(_RNC_Input::STICK::LEFT, _RNC_Input::INPUT_ANGLE::RIGHT)) {
				m_nStageSelect++;
				bInput = true;
			}

			if (bInput) {
				m_nOldnPlanet = m_nPlanetIdx;
				m_nDrawPlanet = m_nOldnPlanet;

				if (m_nStageSelect > -1 && m_nStageSelect < nStageMax) {
					m_RocketPosOld = m_RocketPosOld + (m_RocketposDiff * RocketAnimRate);
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

	float CountRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_nCnt, MAX_COUNT);
	float ImageCntRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_ImageStgCnt, ANIMCOUNT * 0.5);
	float AnimRate;
	D3DXVECTOR3 numpos;

	{// 惑星の描画

		//惑星
		if(!m_bStgEnter)
			RNLib::Model().Put(PRIORITY_OBJECT, m_PlanetType[m_nDrawPlanet].nModel, D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f) * CountRate, false)
			->SetOutLineIdx(5);
		else if(m_bStgEnter)
			RNLib::Model().Put(PRIORITY_OBJECT, m_PlanetType[m_nDrawPlanet].nModel, D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, m_PlanetAngle, 0.0f), D3DXVECTOR3(1.0f, 1.0f, 1.0f), false)
			->SetOutLineIdx(5);

		//ワールド遷移の処理
		if (m_nCnt < MAX_COUNT && m_bStageChange == false && !m_bStgEnter)
			m_nCnt++;
		else if (m_nCnt > 0 && m_bStageChange == true || m_bStgEnter)
			if(m_nCnt != 0)
				m_nCnt--;

		if (m_nCnt == 0 && m_bStageChange == true) {

			if (m_nOldSelect == nStage)
				m_nOldSelect = 0;
			else if (m_nOldSelect == -1)
				m_nOldSelect = nStage;
			m_StgFlag = STAGE::POP;
		}

		//ステージのデモ画像
		RNLib::Polygon3D().Put(PRIORITY_UI, D3DXVECTOR3(IMAGE_STG_POS), D3DXVECTOR3(0.0f, -0.58875f, 0.0f))
			->SetSize(36.0f * ImageCntRate, 27.0f * ImageCntRate)
			->SetCol(COLOR_WHITE)
			/*->SetTex(m_TexIdx[0])*/
			->SetZTest(true);

		//デモ画像のアニメーション処理
		if (m_bStageChange == false) {
			if (m_bRocketMove == false) {

				if (m_RocketAnimCnt < ANIMCOUNT * 0.5) {

					if (m_ImageStgCnt > 0)
						m_ImageStgCnt--;
				}
				else if (m_RocketAnimCnt >= ANIMCOUNT * 0.5 && !m_bStgEnter) {

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
	}

	{//看板
		//ステージ看板
		RNLib::Model().Put(PRIORITY_OBJECT, m_StgBoardIdx, D3DXVECTOR3(0.0f, 16.5f + 12.0f * (1.0f - CountRate), -145.0f), INITD3DXVECTOR3, INITSCALE3D)
			->SetOutLineIdx(5);

		//コイン看板
		if(!m_bStgEnter)
			RNLib::Model().Put(PRIORITY_OBJECT, m_CoinBoardIdx, D3DXVECTOR3(30.0f, 18.0f, -135.0f), D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f), INITSCALE3D)
			->SetOutLineIdx(5);
		else if (m_bStgEnter)
			RNLib::Model().Put(PRIORITY_OBJECT, m_CoinBoardIdx, D3DXVECTOR3(30.0f, 18.0f + (18.0f * (1.0f - CountRate)), -135.0f), D3DXVECTOR3(-0.3925f, 0.58875f, 0.0f), INITSCALE3D)
			->SetOutLineIdx(5);

		if (m_CoinUI != NULL) {
			if(m_bStgEnter)
				m_CoinUI->SetPos(D3DXVECTOR3(COINUIPOS.x, COINUIPOS.y + (COINUIPOS.y * (1.0f - CountRate)), COINUIPOS.z));
			m_CoinUI->Update();
		}
	}

	{// 矢印の描画
		if (!m_bStgEnter) {
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
	}

	// 選択アイコンの処理
	for (int nCnt = 0; nCnt < nStage; nCnt++) {
		bool bStgRel = Manager::StgEd()->GetStageRel(m_nPlanetIdx, nCnt);

		if (nCnt == m_nStageSelect) {

			//アニメーション割合
			AnimRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_AnimCnt[nCnt], ANIMCOUNT);
			if (m_AnimCnt[nCnt] < ANIMCOUNT) m_AnimCnt[nCnt]++;
			//傾き割合
			float RotRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_RotCnt, ANIMCOUNT * 0.5);
			if (m_RotCnt < ANIMCOUNT * 0.5) m_RotCnt++;

			//ロケットアニメーション割合
			RktAnimRt = RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_RocketAnimCnt, ANIMCOUNT);

			//ロケットのアニメーション処理(選択中)
			if (!m_bStgEnter) {

				if (m_RocketAnimCnt < ANIMCOUNT) m_RocketAnimCnt++;
				if (m_RocketAnimCnt == ANIMCOUNT) {

					if (m_bStageChange) {
						m_RocketAnimCnt = 0;
						m_bStageChange = false;
						m_bRocketMove = true;
						m_bRocketRot = false;

						if (m_nStageSelect == 0)
							m_RocketPosOld = FADEROCKET;
						else if (m_nStageSelect == nStage - 1)
							m_RocketPosOld = D3DXVECTOR3(-FADEROCKET.x, FADEROCKET.y, FADEROCKET.z);
					}
					else {
						m_bRocketMove = false;
						m_RocketPosOld = m_RocketPos; m_RocketposDiff = INITD3DXVECTOR3;
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
			}
			else if (m_bStgEnter) {//発射中アニメーション処理
				m_RocketposDiff = INITD3DXVECTOR3;

				if (m_nStgStartCnt < m_RocketRail.GetPointNum() * 10) {

					if (m_nStgStartCnt == 0 && m_RocketAnimCnt == 0) {
						RktAnimRt = 1.0f;
						m_RocketAnimCnt = 0;
					}

					if (m_RocketAnimCnt < ANIMCOUNT) m_RocketAnimCnt++;
					else if (m_RocketAnimCnt == ANIMCOUNT) m_nStgStartCnt++;
				}
					
				if (m_RocketAnimCnt == ANIMCOUNT) {
					m_RocketPosOld = m_RocketPos;
					m_RocketRotDiff = INITD3DXVECTOR3;

					D3DXVECTOR3 Pos = RNLib::Matrix().ConvMtxToPos(m_RocketRail.GetMtx((1.0f / (m_RocketRail.GetPointNum() * 10.0f)) * m_nStgStartCnt, true));
					D3DXVECTOR3 Rot = RNLib::Matrix().ConvMtxToRot(m_RocketRail.GetMtx((1.0f / (m_RocketRail.GetPointNum() * 10.0f)) * m_nStgStartCnt, true));

					if (m_Direction == RIGHT) {
						m_RocketPos = Pos + FADEROCKET;
						m_RocketRotOld = Rot + Rot3D(D3DX_PI * 0.5f,0.0f,0.0f);
					}
					else if (m_Direction == LEFT) {
						m_RocketPos = Pos3D(-Pos.x, Pos.y, Pos.z) + Pos3D(-FADEROCKET.x, FADEROCKET.y, FADEROCKET.z);
						m_RocketRotOld = Rot3D(Rot.x, -Rot.y, -Rot.z) + Rot3D(D3DX_PI * 0.5f, 0.0f, 0.0f);
					}
				}
				m_RocketposDiff = m_RocketPos - m_RocketPosOld;
			}

			//選択変更処理
			if (m_nStageSelect != m_nOldSelect) {
				m_RocketposDiff = m_RocketPos - m_RocketPosOld;

				if (!m_bRocketRot && !m_bStgEnter) {
					m_RocketRotOld = m_RocketRot;

					if (m_RocketposDiff.x > 0) {
						m_Direction = RIGHT;
						m_RocketRot = D3DXVECTOR3(0.0f, D3DX_PI, D3DX_PI * 0.5f);
					}
					else {
						m_Direction = LEFT;
						m_RocketRot = D3DXVECTOR3(D3DX_PI, 0.0f, D3DX_PI * 0.5f);
					}

					m_RocketRotDiff = m_RocketRot - m_RocketRotOld;
					if (m_bStageChange)
						m_bRocketRot = true;
				}
			}
		
			{//ロケット描画
				Matrix mtxRocket = RNLib::Matrix().ConvPosRotScaleToMtx(m_RocketPosOld + (m_RocketposDiff * RktAnimRt),m_RocketRotOld + (RotRate * m_RocketRotDiff), Scale3D(0.15f, 0.15f, 0.15f));
				RNLib::Model().Put(PRIORITY_OBJECT, m_RocketIdx, mtxRocket, false);
				
				float ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.1) + 1.0f);
				D3DXVECTOR3 TexPos = INITPOS3D;
				TexPos.x = TexPos.x + (float)(rand() % (int)3 - 1) * 0.5f;
				TexPos.y = -30.0f;

 				Matrix effMtx = RNLib::Matrix().MultiplyMtx(
					RNLib::Matrix().ConvPosRotToMtx(TexPos,Rot3D(0.0f,0.0f,D3DX_PI)),
					mtxRocket);

				D3DXVECTOR3 EffPos = RNLib::Matrix().ConvMtxToPos(effMtx);
				D3DXVECTOR3 EffRot = RNLib::Matrix().ConvMtxToRot(effMtx);

				Manager::EffectMgr()->ParticleCreate(m_EffTex, EffPos, D3DXVECTOR3(ScaleTex, ScaleTex, 0.0f), Color{ 255,85,0,255 }, CParticle::TYPE::TYPE_FLOATUP,60, EffRot,D3DXVECTOR3(10.0f,10.0f,0.0f),true,true);
			}

			//数字ブロックアニメーション処理
			float NumRate = RNLib::Ease().Easing(_RNC_Ease::TYPE::LINEAR, m_NumAnimCnt, ANIMCOUNT * 2);
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
			Matrix mtxBlock;
			if(!m_bStgEnter)
				mtxBlock = RNLib::Matrix().ConvPosRotScaleToMtx(UNSELECTBOX - poscor + nCnt * NUMPOSSELBOX + (SELBOXRATE * AnimRate), rot, INITSCALE3D * CountRate);
			else if(m_bStgEnter)
				mtxBlock = RNLib::Matrix().ConvPosRotScaleToMtx(UNSELECTBOX - poscor + nCnt * NUMPOSSELBOX + (SELBOXRATE * AnimRate), rot, INITSCALE3D);
			Matrix mtxNum = RNLib::Matrix().MultiplyMtx(
				RNLib::Matrix().ConvPosToMtx(numpos), 
				mtxBlock);

			//ブロック描画
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, mtxBlock, false)
				->SetCol(Color{ 243,191,63,255 });

			//数字テクスチャ描画
			if (m_bStageChange == false && m_nCnt == MAX_COUNT || m_bStgEnter) {
				if (!bStgRel)
					RNLib::Polygon3D().Put(PRIORITY_UI, mtxNum)
					->SetSize(5.0f, 5.0f)
					->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
				else
				{
					RNLib::Polygon3D().Put(PRIORITY_UI, mtxNum)
						->SetSize(5.0f, 5.0f)
						->SetTex(m_TexIdx[3]);

					int nStgCoin = Manager::StgEd()->GetStageCoin(m_nPlanetIdx, nCnt);
					Matrix mtxNum = RNLib::Matrix().MultiplyMtx(
						RNLib::Matrix().ConvPosToMtx(D3DXVECTOR3(numpos.x, numpos.y - 7.5f, numpos.z)),
						mtxBlock);

					RNLib::Text3D().Put(PRIORITY_UI, String("%d枚ひつよう", nStgCoin), _RNC_Text::ALIGNMENT::CENTER, -1, mtxNum)
						->SetSize(Size2D(1.5f, 1.5f))
						->SetCol(COLOR_WHITE)
						->SetZTest(false);
				}
			}
		}
		else {
			//数字ブロックアニメーション処理
			AnimRate = 1.0f - RNLib::Ease().Easing(_RNC_Ease::TYPE::OUT_SINE, m_AnimCnt[nCnt], ANIMCOUNT);
			if (m_AnimCnt[nCnt] > 0) m_AnimCnt[nCnt]--;

			// 非選択時	//ブロック描画
			RNLib::Model().Put(PRIORITY_OBJECT, m_SelIdx, SELECTBOX - poscor + nCnt * NUMPOSSELBOX - (SELBOXRATE * AnimRate), INITD3DXVECTOR3, INITSCALE3D * CountRate, false)
				->SetCol(Color{ 81,63,21,255 });

			//数字テクスチャ描画
			if (m_bStageChange == false && m_nCnt == MAX_COUNT) {
				numpos = D3DXVECTOR3(SELECTBOX.x - poscor.x + (nCnt * NUMPOSSELBOX.x), SELECTBOX.y, SELECTBOX.z - 5.0f);

				if (!bStgRel)
					RNLib::Polygon3D().Put(PRIORITY_UI, numpos - (SELBOXRATE * AnimRate), INITROT3D)
					->SetSize(5.0f, 5.0f)
					->SetCol(Color{ 85,85,85,255 })
					->SetTex(m_TexIdx[2], nCnt + 1, 8, 1);
				else
					RNLib::Polygon3D().Put(PRIORITY_UI, numpos - (SELBOXRATE * AnimRate), INITROT3D)
					->SetSize(5.0f, 5.0f)
					->SetCol(Color{ 85,85,85,255 })
					->SetTex(m_TexIdx[3]);
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
		RNLib::Memory().Alloc(&m_AnimCnt, nStage);
		m_RocketPos = UNSELECTBOX - poscor + NUMPOSROCKET;
		m_RocketposDiff = INITD3DXVECTOR3;
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
		m_bBackMode = false;

		TitleLogoInit(true);
	}
		break;
	case CMode_Title::TITLE_OUTSET:
	{
		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),60.0f,5,10,-1, };// 45
		FormShadow pShadow = { D3DXCOLOR(0.0f,0.0f,0.0f,1.0f),true, D3DXVECTOR3(6.0f,6.0f,0.0f) ,D3DXVECTOR2(4.0f,4.0f) };

		m_pMenu = CFontText::Create(CFontText::BOX_NORMAL_GRAY, D3DXVECTOR3(330.0f, 600.0f, 0.0f), D3DXVECTOR2(0.0f, 0.0f),
			"ボタンを押してねД", CFont::FONT_07NIKUMARU, &pFont, false, false, &pShadow);
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

		m_bStageSelect = false;

		FormFont pFont = { D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),65.0f,5,10,-1 };// 45
	}
		break;
	case CMode_Title::TITLE_NEXT:
		TextRelease(TEXT_ALL);
		break;
	}
}

//========================================
// ステージ解放処理
//========================================
void CMode_Title::StageRel(int nPlanet, int nStgMax)
{

	for (int nCnt = 0; nCnt < nStgMax; nCnt++) {
		bool bStgRel = Manager::StgEd()->GetStageRel(m_nPlanetIdx, nCnt);

		if (bStgRel)
		{
			int nStgCoin = Manager::StgEd()->GetStageCoin(nPlanet, nCnt);

			if (CCoin::GetNumAll() >= nStgCoin)
			{
				Manager::StgEd()->SetStageRel(nPlanet, nCnt, false);
			}
		}
	}
}

//========================================
// タイトルロゴの初期化
//========================================
void CMode_Title::TitleLogoInit(bool bSwitch)
{
	for (int nLogo = 0; nLogo < TITLE_LOGO_MAX; nLogo++) {

		m_TitleLogo.Logo[nLogo].color = Color{ 255,255,255,255 };
		m_TitleLogo.Logo[nLogo].angle = 0.0f;

		m_TitleLogo.Logo[nLogo].TexUV[0] = Pos2D(0.0f, 0.0f);
		m_TitleLogo.Logo[nLogo].TexUV[1] = Pos2D(1.0f, 0.0f);
		m_TitleLogo.Logo[nLogo].TexUV[2] = Pos2D(0.0f, 1.0f);
		m_TitleLogo.Logo[nLogo].TexUV[3] = Pos2D(1.0f, 1.0f);

		if (!bSwitch)
		{
			m_TitleLogo.Logo[nLogo].pos = INITD3DXVECTOR3;
			m_TitleLogo.Logo[nLogo].size = INITD3DXVECTOR2;
			m_TitleLogo.Logo[nLogo].TexIdx = 0;

			for (int nCnt = 0; nCnt < 4; nCnt++) {
				m_TitleLogo.Logo[nLogo].VtxPos[nCnt] = Pos2D(0.0f, 0.0f);
			}			
		}
		else
		{
			D3DXVECTOR3 CenterPos = m_TitleLogo.Logo[nLogo].pos;
			D3DXVECTOR2 size = m_TitleLogo.Logo[nLogo].size;

			m_TitleLogo.Logo[nLogo].VtxPos[0] = Pos2D(CenterPos.x - size.x, CenterPos.y - size.y);
			m_TitleLogo.Logo[nLogo].VtxPos[1] = Pos2D(CenterPos.x + size.x, CenterPos.y - size.y);
			m_TitleLogo.Logo[nLogo].VtxPos[2] = Pos2D(CenterPos.x - size.x, CenterPos.y + size.y);
			m_TitleLogo.Logo[nLogo].VtxPos[3] = Pos2D(CenterPos.x + size.x, CenterPos.y + size.y);

			if (!m_bStageSelect)
			{
				if (nLogo == LOGO_COS1 || nLogo == LOGO_COS2) {
					m_TitleLogo.Logo[nLogo].VtxPos[0].x = CenterPos.x + size.x;
					m_TitleLogo.Logo[nLogo].VtxPos[2].x = CenterPos.x + size.x;

					m_TitleLogo.Logo[nLogo].TexUV[1].x = 1.0f;
					m_TitleLogo.Logo[nLogo].TexUV[3].x = 1.0f;
				}
				else if (nLogo == LOGO_MOS1 || nLogo == LOGO_MOS2) {
					m_TitleLogo.Logo[nLogo].VtxPos[1].x = CenterPos.x - size.x;
					m_TitleLogo.Logo[nLogo].VtxPos[3].x = CenterPos.x - size.x;

					m_TitleLogo.Logo[nLogo].TexUV[0].x = 1.0f;
					m_TitleLogo.Logo[nLogo].TexUV[2].x = 1.0f;
				}
				else if (nLogo == LOGO_NUI) {
					m_TitleLogo.Logo[nLogo].VtxPos[0].y = CenterPos.y + size.y;
					m_TitleLogo.Logo[nLogo].VtxPos[1].y = CenterPos.y + size.y;
				}
				else if (nLogo == LOGO_TM) {
					m_TitleLogo.Logo[nLogo].color.a = 0;
				}
			}
			else
			{
				if(nLogo == LOGO_NUI)
				{
					// 対角線の長さと向き
					const float length = sqrtf(((size.x * 2) * (size.x * 2)) + ((size.y * 2) * (size.y * 2))) * 0.5f;
					const float vtxAngle = atan2f((size.x * 2), (size.y * 2));
					const float inverseVtxAngle = D3DX_PI - vtxAngle;

					// 頂点座標を設定
					{
						m_TitleLogo.Logo[nLogo].angle = -0.3f;
						CenterPos.y = m_TitleLogo.Logo[LOGO_NUI].InitPos.y + (size.y * 0.025f);

						m_TitleLogo.Logo[LOGO_NUI].pos.x = m_TitleLogo.Logo[LOGO_NUI].InitPos.x + 20;
						CenterPos = m_TitleLogo.Logo[LOGO_NUI].pos;
						float angle = m_TitleLogo.Logo[nLogo].angle;
						float resultAngle = angle - inverseVtxAngle;
						m_TitleLogo.Logo[nLogo].VtxPos[0].x = CenterPos.x + sinf(resultAngle) * length;
						m_TitleLogo.Logo[nLogo].VtxPos[0].y = CenterPos.y + cosf(resultAngle) * length;
						resultAngle = angle + inverseVtxAngle;
						m_TitleLogo.Logo[nLogo].VtxPos[1].x = CenterPos.x + sinf(resultAngle) * length;
						m_TitleLogo.Logo[nLogo].VtxPos[1].y = CenterPos.y + cosf(resultAngle) * length;
						resultAngle = angle - vtxAngle;
						m_TitleLogo.Logo[nLogo].VtxPos[2].x = CenterPos.x + sinf(resultAngle) * length;
						m_TitleLogo.Logo[nLogo].VtxPos[2].y = CenterPos.y + cosf(resultAngle) * length;
						resultAngle = angle + vtxAngle;
						m_TitleLogo.Logo[nLogo].VtxPos[3].x = CenterPos.x + sinf(resultAngle) * length;
						m_TitleLogo.Logo[nLogo].VtxPos[3].y = CenterPos.y + cosf(resultAngle) * length;
					}
				}

			}
		}
	}
	TitleAnima = ANIME_NUI;
	m_TitleLogo.nCntAnime = 0;
}

//========================================
// タイトルロゴの読込
//========================================
void CMode_Title::TitleLoad(void)
{
	int nCntLogo = 0;
	char aDataSearch[TXT_MAX];

	// ファイル読込
	FILE *pFile = fopen(TITLE_LOGO_FILE, "r");;

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
		else if (!strcmp(aDataSearch, "SetLogo"))
		{
			while (1)
			{
				fscanf(pFile, "%s", aDataSearch);	// 検索
				if (!strcmp(aDataSearch, "EndLogo")) {
					nCntLogo++;
					break;
				}
				else if (!strcmp(aDataSearch, "Pos")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%f", &m_TitleLogo.Logo[nCntLogo].pos.x);
					fscanf(pFile, "%f", &m_TitleLogo.Logo[nCntLogo].pos.y);
					fscanf(pFile, "%f", &m_TitleLogo.Logo[nCntLogo].pos.z);

					m_TitleLogo.Logo[nCntLogo].InitPos = m_TitleLogo.Logo[nCntLogo].pos;

				}
				else if (!strcmp(aDataSearch, "Size")) {
					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%f", &m_TitleLogo.Logo[nCntLogo].size.x);
					fscanf(pFile, "%f", &m_TitleLogo.Logo[nCntLogo].size.y);
				}
				else if (!strcmp(aDataSearch, "TexPath"))	{
					char aFileName[0xff];

					fscanf(pFile, "%s", &aDataSearch[0]);
					fscanf(pFile, "%s", &aFileName);		// ファイル名

					m_TitleLogo.Logo[nCntLogo].TexIdx = RNLib::Texture().Load(aFileName);
				}
			}
		}
	}
}

//========================================
// テキスト削除
//========================================
void CMode_Title::TextRelease(TEXT type) {

	// メニュー
	if (type == TEXT_MENU || type == TEXT_ALL) {
		if (m_pMenu != NULL) {
			m_pMenu->Uninit();
			m_pMenu = NULL;
		}
	}
}