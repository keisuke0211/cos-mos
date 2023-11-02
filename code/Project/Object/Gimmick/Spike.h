//========================================
// 
// 棘
// Author:KEISUKE OTONO
// 
//========================================
// *** spike.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス
//****************************************
class CSpike : public CStageObject {
public:

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 move;		// 移動量
		short nModelIdx;		// モデル番号
	};

	// *** 関数 ***
	CSpike();
	~CSpike();

	/* 初期化	*/HRESULT Init(void);
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