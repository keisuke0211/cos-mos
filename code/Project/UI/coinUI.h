//========================================
// 
// コインUIの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
//****************************************
// クラス定義
//****************************************
// 惑星の情報構造体
class CCoinUI {
public:

	//========== [[[ 関数宣言 ]]]
	CCoinUI();
	virtual        ~CCoinUI();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static CCoinUI *Create(void);

protected:

private:
	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_pos;			//位置
	Scale2D		m_scale;		//拡縮
	int			m_TexIdx[2];	//テクスチャ番号　
	int			m_num;
};
