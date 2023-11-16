//========================================
// 
// ブロック(判定 無し)
// Author:KEISUKE OTONO
// 
//========================================
// *** fill-block.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス
//****************************************
class CFillBlock : public CStageObject {
public:
	// 穴埋めブロック種類
	enum class BLOCKTYPE
	{
		TYPE_1x1 = 0,
		TYPE_1x2,
		TYPE_1x3,
		TYPE_2x1,
		TYPE_2x2,
		TYPE_3x1,
		TYPE_3x3,
		TYPE_4x4,
		TYPE_MAX,
	};

	// 共通情報
	struct Info
	{
		BLOCKTYPE Type;// 種類
		short nModelIdx;// モデル番号
	};

	// *** 関数 ***
	CFillBlock();
	~CFillBlock();
	/* 初期化	*/HRESULT Init(BLOCKTYPE type);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);

	// -- 設定 ---------------------------------------------
	/* モデル番号	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	// -- 取得 ---------------------------------------------

private:
	//サイズの種類
	enum class SIZE
	{
		WIDTH = 0,	//幅
		HEIGHT,		//高さ
		MAX
	};

	// *** 変数 ***
	Info m_Info;		// 共通情報
	static const char* s_TypePath[(int)CFillBlock::BLOCKTYPE::TYPE_MAX];
	static const float s_Size[(int)CFillBlock::BLOCKTYPE::TYPE_MAX][(int)CFillBlock::SIZE::MAX];
};