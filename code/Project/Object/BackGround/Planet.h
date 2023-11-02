//========================================
// 
// 惑星の処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// 惑星の情報構造体
class CPlanet : public CStageObject {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class STAR_TYPE {
		SATURN,
		SATURNx2,
		SATURNx4,
		SATURNx8,
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CPlanet();
	virtual        ~CPlanet();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void	SetModelIdx(int idx)	 { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void	SetCol(Color col)		 { m_col = col; }
	void    SetType(STAR_TYPE type) { m_Star_type = type; }
protected:

private:
	//========== [[[ 変数宣言 ]]]
	D3DXVECTOR3 m_posOld;		// 前の位置
	D3DXVECTOR3 m_posInit;		// 初期の位置
	D3DXVECTOR3 m_move;			// 移動量
	Color m_col;				// 色
	STAR_TYPE m_Star_type;		// 種類
	int m_moveCounter;			// 移動量のカウンター
	int ModelIdx;
};
