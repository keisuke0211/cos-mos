//========================================
// 
// コイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "coin.h"
#include "../../manager.h"
#include "../../Mode/mode_game.h"
#include "../../stage.h"

int CCoin::s_NumAll = 0;
int CCoin::s_Num = 0;
int CCoin::s_CorrNumAll = 0; //今現在のステージのコイン総数

//========================================
// コンストラクタ
//========================================
CCoin::CCoin() {

	Manager::StageObjectMgr()->AddList(this);
	m_ModelIdx = RNLib::Model().Load("data\\MODEL\\Item\\Coin.x");

	if (s_Num != 0)
		s_Num = 0;

	m_nID = s_CorrNumAll++;

	//現在のワールド・ステージ番号を取得
	CStageEditor *pEd = Manager::StgEd();
	CInt planet = pEd->GetPlanetIdx();
	CInt stage = pEd->GetType()[planet].nStageIdx;

	//色設定（既に取得していたら半透明に
	m_color = Stage::GetCoinInfo(planet, stage, m_nID) ? Color{ 255,255,255, 100 } : COLOR_WHITE;
	m_outLineIdx = Stage::GetCoinInfo(planet, stage, m_nID) ? NONEDATA : 8;
}

//========================================
// デストラクタ
//========================================
CCoin::~CCoin() {

	Manager::StageObjectMgr()->SubList(this);
	s_CorrNumAll--;
}

//========================================
// 更新処理
//========================================
void CCoin::Update(void) {
	m_rot.y -= 0.01f;

	RNLib::Model().Put(PRIORITY_OBJECT, m_ModelIdx, m_pos, m_rot, false)
		->SetCol(m_color)
		->SetOutLineIdx(m_outLineIdx);

	for (int nCnt = 0; nCnt < CPlayer::NUM_PLAYER; nCnt++)
	{
		//プレイヤー情報取得
		CPlayer::Info *pInfo = CPlayer::GetInfo(nCnt);

		if (RNLib::Geometry().FindDistance(m_pos, pInfo->pos) <= 16)
		{
			Delete();
			s_Num++;

			//現在のワールド・ステージ番号を取得
			CStageEditor *pEd = Manager::StgEd();
			CInt planet = pEd->GetPlanetIdx();
			CInt stage = pEd->GetType()[planet].nStageIdx;
			
			//取得状況代入
			Stage::SetCoinInfo(planet, stage, m_nID, true);
		}
	}
}