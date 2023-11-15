//========================================
// 
// 動くブロック処理 [move-block.h]
// Author:HIROMU KOMURO
// 
//========================================
#include "../../manager.h"
#include "move-block.h"
#include "../../main.h"

#define ROT_MAG		(0.05f)	// 倍率
//========================================
// 静的変数
//========================================
int CMoveBlock::m_nNumAll = 0;

//========================================
// コンストラクタ
//========================================
CMoveBlock::CMoveBlock(void)
{
	Manager::BlockMgr()->AddList(this);

	m_type = TYPE::MOVE_BLOCK;	// 種類の設定

	// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 2.0f;
	m_height = SIZE_OF_1_SQUARE * 0.35f;

	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.posOld = INITD3DXVECTOR3;
	m_Info.move = INITD3DXVECTOR3;
	m_Info.rot = INITD3DXVECTOR3;
	m_Info.size = INITD3DXVECTOR3;
	m_Info.frefdef = 0.0f;
	m_Info.nType = 0;
	m_Info.nID = m_nNumAll;
	m_Info.bXReturn = false;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Lift.x");
	nModelIdx = RNLib::Model().Load("data\\MODEL\\Lift_Gear.x");
	m_nNumAll++;

	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//========================================
// デストラクタ
//========================================
CMoveBlock::~CMoveBlock()
{
	m_nNumAll--;
}

//========================================
// 初期化
//========================================
HRESULT CMoveBlock::Init(void)
{
	// 始点を位置に代入
	m_Info.pos = m_Info.m_posV;
	return S_OK;
}

//========================================
// 終了
//========================================
void CMoveBlock::Uninit(void)
{
	
}

//========================================
// 更新
//========================================
void CMoveBlock::Update(void)
{
	m_Info.posOld = m_Info.pos;	// 前回位置更新
	m_Info.pos += m_Info.move;	// 移動量の増加

	if (m_Info.move.y != 0.0f)
	{
		float fConv = m_Info.move.y;
		if (fConv < 0.0f)
			fConv -= 1.0f;
		else
			fConv += 1.0f;

		m_rot.z += fConv * ROT_MAG * -1;
	}

	if (m_Info.move.x != 0.0f)
	{
		float f = m_Info.move.x;
		if (f < 0.0f)
			f -= 1.0f;
		else
			f += 1.0f;

		m_rot.z += f * ROT_MAG * -1;
	}

	if (m_Info.bXReturn == false)
	{
		// xの移動量の反転
		if (m_Info.m_posV.x >= m_Info.pos.x || m_Info.m_posL.x <= m_Info.pos.x)
		{
			m_Info.move.x *= -1;
		}
	}
	else
	{
		// xの移動量の反転
		if (m_Info.m_posV.x <= m_Info.pos.x || m_Info.m_posL.x >= m_Info.pos.x)
		{
			m_Info.move.x *= -1;
		}
	}
	// yの移動量の反転
	if (m_Info.m_posV.y <= m_Info.pos.y || m_Info.m_posL.y >= m_Info.pos.y)
	{
		m_Info.move.y *= -1;
	}

	CStageObject::SetPos(m_Info.pos);
	RNLib::Model().Put(m_Info.pos, m_Info.rot, m_Info.nModelIdx)
		->SetCol(m_color);
	RNLib::Model().Put(m_Info.pos, m_rot, nModelIdx)
		->SetCol(m_color);
}

//========================================
// 描画
//========================================
void CMoveBlock::Draw(void)
{

}