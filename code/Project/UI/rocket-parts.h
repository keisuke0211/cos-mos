//========================================
// 
// ロケットパーツUIの処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
//****************************************
// クラス定義
//****************************************
// 惑星の情報構造体
class CRocketPartsUI{
public:

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE = 0,
		OBTAIN,
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CRocketPartsUI();
	virtual        ~CRocketPartsUI();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static CRocketPartsUI *Create(void);

	static void SetRocketstg(bool bRocketStg) { m_bRocketStg = bRocketStg; }
	static bool GetRocketstg(void) { return m_bRocketStg; }

protected:

private:
	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_pos;			// 位置
	Scale2D		m_scale;		// 拡縮
	STATE		m_state[2];		// 状態
	int			m_TexIdx;
	int			m_colorA;
	int			m_num;
	static bool	m_bRocketStg;	//ロケットのあるステージ？
};