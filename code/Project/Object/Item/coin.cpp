//========================================
// 
// コイン処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "coin.h"
#include "../../manager.h"
#include "../../Mode/mode_game.h"

int CCoin::s_NumAll = 0;
int CCoin::s_Num = 0;
//========================================
// コンストラクタ
//========================================
CCoin::CCoin() {

	Manager::StageObjectMgr()->AddList(this);
	m_ModelIdx = RNLib::Model().Load("data\\MODEL\\Item\\Coin.x");

	if (s_Num != 0)
		s_Num = 0;
}

//========================================
// デストラクタ
//========================================
CCoin::~CCoin() {

	Manager::StageObjectMgr()->SubList(this);
}

//========================================
// 更新処理
//========================================
void CCoin::Update(void) {
	m_rot.y -= 0.01f;

	RNLib::Model().Put(PRIORITY_OBJECT, m_ModelIdx, m_pos, m_rot, false)
		->SetOutLineIdx(8);

	CPlayer *pPlayer = CMode_Game::GetPlayer();
	CPlayer::Info *pInfo[2];
	pPlayer->GetInfo(pInfo[0],pInfo[1]);
	
	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		if (CGeometry::FindDistance(m_pos, pInfo[nCnt]->pos) <= 16)
		{
			Delete();
			s_Num++;
		}
	}
}