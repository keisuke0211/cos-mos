//========================================
// 
// マグマブロック処理
// Author:RYUKI FUJIWARA
// 
//========================================
// *** magma-block.cpp ***
//========================================
#include "magma-block.h"
#include "../../main.h"

#define COUNT (30)
//========================================
// 静的変数
//========================================
int CMagmaBlock::m_nNumAll = 0;

//========================================
// コンストラクタ
//========================================
CMagmaBlock::CMagmaBlock(void)
{
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::LAVA_BLOCK;
	m_width = SIZE_OF_1_SQUARE * 0.9f;
	m_height = SIZE_OF_1_SQUARE * 0.9f;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\maguma.x");
	m_Info.nID = m_nNumAll;
	m_Info.nCnt = 0;
}

//========================================
// デストラクタ
//========================================
CMagmaBlock::~CMagmaBlock()
{

}

//========================================
// 初期化
//========================================
HRESULT CMagmaBlock::Init(void)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;

	return S_OK;
}

//========================================
// 終了
//========================================
void CMagmaBlock::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CMagmaBlock::Update(void)
{
	m_Info.nCnt++;

	if ((m_Info.nCnt % COUNT) == 0)
	{
		int Tex = RNLib::Texture().Load("data\\TEXTURE\\maguma2.png");

		Manager::EffectMgr()->EffectCreate(Tex, D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z - SIZE_OF_1_SQUARE*0.5f), D3DXVECTOR3(16.0f, 16.0f, 0.0f), COLOR_WHITE,1200);
	}

	// 過去の位置
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
		->SetOutLineIdx(true)
		->SetCol(m_color);
}

//========================================
// 描画
//========================================
void CMagmaBlock::Draw(void)
{

}