//========================================
// 
// エフェクト処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once
#include "../../_RNLib/RNlib.h"

//****************************************
// クラス
//****************************************
class CEffect : public CObject{
public:

	// 共通情報
	struct Info
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 scale;		// 移動量
		Color col;				// 頂点カラー
		int nCount;
		int nTex;
	};

	// *** 関数 ***
	CEffect();
	~CEffect();

	/* 初期化	*/HRESULT Init(int nTex);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* 色		*/void SetColor(const Color col) { m_Info.col = col; }
	/* 位置		*/void SetPos(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	/* 位置		*/void SetScale(const D3DXVECTOR3 scale) { m_Info.scale = scale; }

	// -- 所得 ---------------------------------------------
	/* 色		*/Color GetColor(void) { return m_Info.col; }
	/* 位置		*/D3DXVECTOR3 GetPos(void) { return m_Info.pos; }
	/* 位置		*/D3DXVECTOR3 GetScale(void) { return m_Info.scale; }

private:

	// *** 静的変数 ***
	static int m_nNumAll;		// 総数

	// *** 変数 ***
	Info m_Info;		// 共通情報
};
