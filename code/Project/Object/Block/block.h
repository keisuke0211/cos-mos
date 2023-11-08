//========================================
// 
// ブロック処理
// Author:KEISUKE OTONO
// 
//========================================
// *** block.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス
//****************************************
class CBlock : public CStageObject {
public:

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 move;		// 移動量
		int nType;				// 種類
		short nModelIdx;		// モデル番号
		int nID;				// ID
	};

	// 穴埋めブロック種類
	enum class BLOCK_TYPE
	{
		BLOCK_NORMAL = 0,
		BLOCK_NUI,
		BLOCK_MAX
	};

	// *** 関数 ***
	CBlock();
	~CBlock();

	/* 初期化	*/HRESULT Init(BLOCK_TYPE type);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* モデル番号	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }

	// -- 所得 ---------------------------------------------

private:

	// *** 静的変数 ***
	static int m_nNumAll;		// 総数

	// *** 変数 ***
	Info m_Info;		// 共通情報

};