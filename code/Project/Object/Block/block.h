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
		SOIL_BLOCK					 = 0,	// 土ブロック
		SOIL_BLOCK_GRASSY			 = 1,	// 草の生えた土ブロック
		ANCIENT_STONE_BLOCK			 = 2,	// 古代の石ブロック
		ANCIENT_STONE_BLOCK_PTN_A	 = 3,	// 古代の石ブロックA
		ANCIENT_STONE_BLOCK_PTN_B	 = 4,	// 古代の石ブロックB
		ANCIENT_STONE_BLOCK_PTN_C	 = 5,	// 古代の石ブロックC
		ANCIENT_STONE_PILLAR_UP		 = 6,	// 柱の上
		ANCIENT_STONE_PILLAR_CENTER	 = 7,	// 柱の繋ぎ
		ANCIENT_STONE_PILLAR_DOWN	 = 8,	// 柱の根本
		LEAF_BLOCK	                 = 9,	// 草ブロック
		TREE_TRUNK	                 = 10,	// 木の幹
		TREE_TRUNK_ROOT              = 11,	// 木の根元
		BAOBAB_TREE					 = 12,	// バオバブの木(背景用)
		CHEST                        = 13,	// 宝箱
		PILE_OF_COINS                = 14,	// 山積みのコイン
		STONE_MONUMENT               = 15,	// 謎の石碑
		ANCIENT_STONE_BRICK_BLOCK    = 16,	// 古代の石レンガブロック
		SOIL_AND_ANCIENT_STONE_BLOCK = 17,	// 土＆古代の石ブロック(中間用)
		PALMTREE					 = 18,	// ヤシの木
		TREE_EYES_BLOCK				 = 19,	// 三つ目のブロック
		LEAF_BLOCK_NUTS              = 20,	// 草ブロック(木の実)
		MAX
	};

	// その他テクスチャ
	enum class OTHER_TEXTURE {
		EFFECT,
		MAX,
	};

	// その他モデル
	enum class OTHER_MODEL {
		LEAF_INSIDE,
		MAX,
	};

	// その他セットアップ3D
	enum class OTHER_SETUP3D {
		BAOBAB_TREE,
		CHEST,
		PALM_TREE,
		MAX,
	};

	// その他モーション3D
	enum class OTHER_MOTION3D {
		CHEST_STEPPED,
		PLAMTREE_SHAKE,
		MAX,
	};

	// その他サウンド
	enum class OTHER_SOUND {
		COIN,
		LIGHT_A,
		LIGHT_B,
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

	void IsReaction_HitsRot(const int nRot) { m_isHits[nRot] = true; }
	void IsReaction_Move(bool bFrag)  { ; } //上で移動している
	void IsReaction_Land(bool bFrag)  { ; } //上に着地した瞬間
	void IsReaction_TakeOff(bool bFrag){ ;}//上からジャンプで離れた瞬間

private:
	//========== [[[ 定数宣言 ]]]
	static const char* MODEL_PATHS[(int)LOOKS_TYPE::MAX];
	static const char* OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX];
	static const char* OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX];
	static const char* OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX];
	static const char* OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX];
	static const char* OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX];
	//========== [[[ 変数宣言 ]]]
	static UShort m_num;
	static Scale2D m_eyescale;
	static short m_modelIdxes[(int)LOOKS_TYPE::MAX];
	static short m_otherTextureIdxes[(int)OTHER_TEXTURE::MAX];
	static short m_otherModelIdxes[(int)OTHER_MODEL::MAX];
	static short m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
	static short m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
	static short m_otherSoundIdxes[(int)OTHER_SOUND::MAX];
	CDoll3D*   m_doll;
	LOOKS_TYPE m_looksType;		// 種類
	bool       m_isCollision;
	bool       m_isHitOlds[6];
	bool       m_isHits[6];
	Pos3D      m_targetAddPos;
	Pos3D      m_oldAddPos;
	Pos3D      m_addPos;
	short      m_counter;
	short      m_counterMax;
	int		   m_nTexIdx;
};