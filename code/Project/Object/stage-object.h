//========================================
// 
// ステージオブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// クラス定義
//****************************************
class CStageObject : public CObject {
public:
	//========== [[[ 定数定義 ]]]
	static const float SIZE_OF_1_SQUARE;
	static const Pos3D INVERSEVECTOR3;

	//========== [[[ 列挙型定義 ]]]
	enum class TYPE {
		NONE       = -1,//何もなし
		BLOCK      = 0,	//ブロック
		FILLBLOCK  = 1,	//穴埋めブロック
		TRAMPOLINE = 2,	//トランポリン
		SPIKE	   = 3,	//トゲ
		MOVE_BLOCK = 4,	//移動ブロック
		METEOR	   = 5,	//隕石
		LASER      = 6,	//レーザー
		EXTEND_DOG = 7,	//ヌイ
		GOALGATE   = 8, //ゴールゲート
		PARTS      = 9,	//ロケットパーツ
		ROCKET	   = 10,//ロケット本体
		PILE       = 11,//杭
		BACKGROUND = 12,//背景
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CStageObject();
	virtual ~CStageObject();
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	TYPE GetType(void) { return m_type; }
	void SetPos(const D3DXVECTOR3& pos) { m_pos = m_posOld = pos; }
	D3DXVECTOR3& GetPos(void) { return m_pos; }
	D3DXVECTOR3& GetPosOld(void) { return m_posOld; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	D3DXVECTOR3& GetRot(void) { return m_rot; }
	void SetColor(Color color) { m_color = color; }
	float& GetWidth(void) { return m_width; }
	float& GetHeight(void) { return m_height; }

protected:
	//========== [[[ 変数宣言 ]]]
	TYPE        m_type;
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_posOld;
	D3DXVECTOR3 m_rot;
	Color       m_color;
	float       m_width;
	float       m_height;
};