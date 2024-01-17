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

	static void	Collision(D3DXVECTOR3 pos,D3DXVECTOR3 rot,D3DXVECTOR3 Laserpos,D3DXVECTOR2 Lasersize);
	void	SetModelIdx(int idx) { ModelIdx = idx; }
	void	SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void	SetPosInfo(const D3DXVECTOR3 posV, const D3DXVECTOR3 posL) { m_posV = posV, m_posL = posL; }	// 位置の各種設定
	void	SetCol(Color col) { m_col = col; }
	void	SetMove(const D3DXVECTOR3 move) { m_move = move; }
	void	SetLaserSize(const D3DXVECTOR2 size) { m_LaserSize = size; }
	void	SetXMoveReturn(const bool xReturn) { m_bXMoveReturn = xReturn; }

	D3DXVECTOR3 GetBlockpos(void) { return m_pos; }
	D3DXVECTOR3 GetLaserPos(void) { return m_LaserPos; }
	D3DXVECTOR2	GetLaserSize(void) { return m_LaserSize; }
	D3DXVECTOR3 GetMove(void) { return m_move; }


protected:

private:
	//========== [[[ 変数宣言 ]]]
	D3DXMATRIX mtx;				// マトリックス
	D3DXVECTOR3 m_posV;			// 始点位置
	D3DXVECTOR3 m_posL;			// 終点位置
	D3DXVECTOR3 m_move;			// 移動量
	Color m_col;				// 色
	float m_fGroundDis;			// 地面までの距離
	D3DXVECTOR3 m_LaserPos;		// レーザーの位置
	D3DXVECTOR2 m_LaserSize;	// レーザーの大きさ
	int ModelIdx;
	bool m_bXMoveReturn;			// xの移動の反転
};
