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
class CEffect_Model : public CObject {
public:

	// *** 関数 ***
	CEffect_Model();
	~CEffect_Model();

	/* 初期化	*/HRESULT Init(int nIdx, int nCount,D3DXVECTOR3 move);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 描画		*/void Draw(void);

	// -- 設定 ---------------------------------------------
	/* 色		*/void SetColor(const Color col) { m_col = col; }
	/* 位置		*/void SetPos(const D3DXVECTOR3 pos) { m_pos = pos; }
	/* 位置		*/void SetRot(const D3DXVECTOR3 rot) { m_rot = rot; }
	/* 位置		*/void SetScale(const D3DXVECTOR3 scale) { m_scale = scale; }

	// -- 所得 ---------------------------------------------
	/* 色		*/Color GetColor(void) { return m_col; }
	/* 位置		*/D3DXVECTOR3 GetPos(void) { return m_pos; }
	/* 位置		*/D3DXVECTOR3 GetScale(void) { return m_scale; }

private:

	void CollisionBound(void);

	// *** 静的変数 ***
	static int m_nNumAll;		// 総数

	// *** 変数 ***
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posold;	// 前回位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_spin;		// 回転量
	D3DXVECTOR3 m_scale;		// 拡縮
	Color m_col;				// 頂点カラー
	int m_nCount;
	int m_nCountMax;
	int m_nIdx;
};
