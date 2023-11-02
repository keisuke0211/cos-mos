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

	// 共通情報
	struct Info
	{
		int nType;				// 種類
		short nModelIdx;		// モデル番号
	};

	// 穴埋めブロック種類
	enum class FILL_TYPE
	{
		FILL_1x1 = 0,
		FILL_2x2,
		FILL_3x3,
		FILL_4x4,
		FILL_MAX,
	};

	// *** 関数 ***
	CFillBlock();
	~CFillBlock();

	/* 初期化	*/HRESULT Init(FILL_TYPE type);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* モデル番号	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	// -- 所得 ---------------------------------------------

private:

	// *** 変数 ***
	Info m_Info;		// 共通情報

};