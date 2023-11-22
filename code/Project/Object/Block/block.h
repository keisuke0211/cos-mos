//========================================
// 
// ブロック処理
// Author:KEISUKE OTONO
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス
//****************************************
class CBlock : public CStageObject {
public:
	//========== [[[ 列挙型定義 ]]]
	// 見た目の種類
	enum class LOOKS_TYPE {
		SOIL_BLOCK = 0,
		SOIL_BLOCK_GRASSY = 1,
		ANCIENT_STONE_BLOCK = 2,
		ANCIENT_STONE_BLOCK_PTN_A = 3,
		ANCIENT_STONE_BLOCK_PTN_B = 4,
		ANCIENT_STONE_BLOCK_PTN_C = 5,
		ANCIENT_STONE_PILLAR_UP = 6,
		ANCIENT_STONE_PILLAR_CENTER = 7,
		ANCIENT_STONE_PILLAR_DOWN = 8,
		LEAF_BLOCK = 9,
		TREE_TRUNK = 10,
		TREE_TRUNK_ROOT = 11,
		MAX
	};

	// その他モデル
	enum class OTHER_MODEL {
		LEAF_INSIDE,
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	static void Load(void);
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetCollision(bool isCollision) { m_isCollision = isCollision; }
	bool& GetCollision(void) { return m_isCollision; }

	//========== [[[ リアクション用関数宣言 ]]]
	bool IsGetOld(const int nRot) { return m_isHitOlds[nRot]; }

	void IsReaction_HitsRot(int nRot) { m_isHits[nRot] = true; }
	void IsReaction_Move(bool bFrag)	{ ; }//上で移動している
	void IsReaction_Land(bool bFrag)	{ ; }//上に着地した瞬間
	void IsReaction_TakeOff(int nRot)   { ; }//上からジャンプで離れた瞬間

private:
	//========== [[[ 定数宣言 ]]]
	static const char* MODEL_PATHS[(int)LOOKS_TYPE::MAX];
	static const char* OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX];

	//========== [[[ 変数宣言 ]]]
	static short m_modelIdxes[(int)LOOKS_TYPE::MAX];
	static short m_otherModelIdxes[(int)OTHER_MODEL::MAX];
	LOOKS_TYPE m_looksType;		// 種類
	bool       m_isCollision;
	bool       m_isHits   [6];
	bool       m_isHitOlds[6];
	Pos3D      m_targetAddPos;
	Pos3D      m_oldAddPos;
	Pos3D      m_addPos;
	short      m_counter;
	short      m_counterMax;
};