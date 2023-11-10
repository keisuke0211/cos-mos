//========================================
// 
// 隕石エフェクト処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"

//****************************************
// クラス
//****************************************
class CEffect_Meteor : public CObject {
public:

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		Scale3D scale;			// 大きさ
		Color col;				// 頂点カラー
		int nCountMax;
	};

	// *** 関数 ***
	CEffect_Meteor();
	~CEffect_Meteor();

	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* 位置		*/void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }


private:

	// *** 静的変数 ***
	static int m_nNumAll;		// 総数
	static int s_nModelIdx[3];	// モデル

	// *** 変数 ***
	Info m_Info[10];		// 共通情報
	D3DXVECTOR3 m_pos;		// 位置
	int m_nCount;

};
