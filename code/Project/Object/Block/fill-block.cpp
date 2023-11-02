//========================================
// 
// ブロック (判定 無し)
// Author:KEISUKE OTONO
// 
//========================================
// *** fill-block.cpp ***
//========================================
#include "fill-block.h"
#include "../../main.h"

//パス読み込み
static const char* s_Path[4] = {
	"data\\MODEL\\fill-block_1x1.x",
	"data\\MODEL\\fill-block_2x2.x",
	"data\\MODEL\\fill-block_3x3.x",
	"data\\MODEL\\fill-block_4x4.x",
};

//========================================
// コンストラクタ
//========================================
CFillBlock::CFillBlock(void)
{
	Manager::BlockMgr()->AddList(this);

	m_type = TYPE::FILLBLOCK;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE;

	m_Info.nType = 0;
}

//========================================
// デストラクタ
//========================================
CFillBlock::~CFillBlock()
{

}

//========================================
// 初期化
//========================================
HRESULT CFillBlock::Init(FILL_TYPE type)
{
	m_Info.nType = 0;

	//モデル番号読み込み
	m_Info.nModelIdx = RNLib::Model().Load(s_Path[(int)type]);

	return S_OK;
}

//========================================
// 終了
//========================================
void CFillBlock::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CFillBlock::Update(void)
{
	// 過去の位置
	RNLib::Model().Put(m_pos, D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), m_Info.nModelIdx, false)
		->SetCol(m_color);
}



//========================================
// 描画
//========================================
void CFillBlock::Draw(void)
{

}