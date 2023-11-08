//========================================
// 
// ブロック処理
// Author:KEISUKE OTONO
// 
//========================================
// *** block.cpp ***
//========================================
#include "block.h"
#include "../../main.h"

//========================================
// 静的変数
//========================================
int CBlock::m_nNumAll = 0;

//パス読み込み
static const char* s_Path[8] = {
	"data\\MODEL\\Block_Chocolate.x",
	"data\\MODEL\\WallDog\\WallDog_House.x",
};

//========================================
// コンストラクタ
//========================================
CBlock::CBlock(void)
{
	Manager::BlockMgr()->AddList(this);

	m_type   = TYPE::BLOCK;
	m_width  = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;
	m_Info.nID = m_nNumAll;
}

//========================================
// デストラクタ
//========================================
CBlock::~CBlock()
{

}

//========================================
// 初期化
//========================================
HRESULT CBlock::Init(BLOCK_TYPE type)
{
	m_Info.move = INITD3DXVECTOR3;
	m_Info.nType = 0;

	//モデル番号読み込み
	m_Info.nModelIdx = RNLib::Model().Load(s_Path[(int)type]);

	return S_OK;
}

//========================================
// 終了
//========================================
void CBlock::Uninit(void)
{
	
}

//========================================
// 更新
//========================================
void CBlock::Update(void)
{
	// 過去の位置
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_Info.nModelIdx, false)
		->SetOutLine(true)
		->SetCol(m_color);
}

//========================================
// 描画
//========================================
void CBlock::Draw(void)
{

}