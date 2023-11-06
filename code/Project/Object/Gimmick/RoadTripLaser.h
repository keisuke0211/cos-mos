//========================================
// 
// 往復するレーザーの処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// クラス定義
//****************************************
// レーザーの情報構造体
class CRoadTripLaser : public CStageObject {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class ROT {
		UP,
		DOWN,
		LEFT,
		RIGHT,
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CRoadTripLaser();
	virtual        ~CRoadTripLaser();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void	SetCol(Color col) { m_col = col; }
	void	SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void	SetRefdef(const float refdef) { m_frefdef = refdef; }
protected:

private:
	//========== [[[ 変数宣言 ]]]
	D3DXMATRIX mtx;				// マトリックス
	D3DXVECTOR3 m_posOld;		// 前の位置
	D3DXVECTOR3 m_refPos;		// 位置(過去)
	D3DXVECTOR3 m_move;			// 移動量
	Color m_col;				// 色
	float m_frefdef;			// 反射する距離
	float m_fGroundDis;			// 地面までの距離
	int ModelIdx;
};
