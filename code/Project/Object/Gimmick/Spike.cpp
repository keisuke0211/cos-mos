//========================================
// 
// 棘
// Author:KEISUKE OTONO
// 
//========================================
// *** spike.h ***
//========================================
#include "Spike.h"
#include "../../main.h"

//========================================
// コンストラクタ
//========================================
CSpike::CSpike(void)
{
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::SPIKE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 0.5f;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Spike.x");
}

//========================================
// デストラクタ
//========================================
CSpike::~CSpike()
{
	
}

//========================================
// 初期化
//========================================
HRESULT CSpike::Init(void)
{
	m_Info.move = INITD3DXVECTOR3;

	return S_OK;
}

//========================================
// 終了
//========================================
void CSpike::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CSpike::Update(void)
{
	// 過去の位置
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, m_rot, false)
		->SetOutLineIdx(true)
		->SetCol(m_color);
}

//========================================
// 描画
//========================================
void CSpike::Draw(void)
{

}