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
	enum class LOOKS_TYPE {
		// 見た目の種類
		SOIL_BLOCK					       = 0,		// 土ブロック
		SOIL_BLOCK_GRASSY			       = 1,		// 草の生えた土ブロック
		ANCIENT_STONE_BLOCK			       = 2,		// 古代の石ブロック
		ANCIENT_STONE_BLOCK_PTN_A	       = 3,		// 古代の石ブロックA
		ANCIENT_STONE_BLOCK_PTN_B	       = 4,		// 古代の石ブロックB
		ANCIENT_STONE_BLOCK_PTN_C	       = 5,		// 古代の石ブロックC
		ANCIENT_STONE_PILLAR_UP		       = 6,		// 柱の上
		ANCIENT_STONE_PILLAR_CENTER	       = 7,		// 柱の繋ぎ
		ANCIENT_STONE_PILLAR_DOWN	       = 8,		// 柱の根本
		LEAF_BLOCK	                       = 9,		// 草ブロック
		TREE_TRUNK	                       = 10,	// 木の幹
		TREE_TRUNK_ROOT                    = 11,	// 木の根元
		BAOBAB_TREE					       = 12,	// バオバブの木(背景用)
		CHEST                              = 13,	// 宝箱
		PILE_OF_COINS                      = 14,	// 山積みのコイン
		STONE_MONUMENT                     = 15,	// 謎の石碑
		ANCIENT_STONE_BRICK_BLOCK          = 16,	// 古代の石レンガブロック
		SOIL_AND_ANCIENT_STONE_BLOCK       = 17,	// 土＆古代の石ブロック(中間用)
		PALMTREE					       = 18,	// ヤシの木
		TREE_EYES_BLOCK				       = 19,	// 三つ目のブロック
		LEAF_BLOCK_NUTS                    = 20,	// 草ブロック(木の実)
		NUI_HOUSE_BLOCK                    = 21,	// ヌイの家ブロック
		ASPHALT_BLOCK				       = 22,	// アスファルトブロック
		ASPHALT_BLOCK_CRACKED		       = 23,	// ひび割れたアスファルトブロック
		IRON_BAR					       = 24,	// 鉄格子(当たり判定無し想定)
		IRON_BAR_1					       = 25,	// 鉄格子1
		IRON_BAR_2					       = 26,	// 鉄格子2
		IRON_BAR_3					       = 27,	// 鉄格子3
		MUD_BLOCK					       = 28,	// 泥ブロック
		SAND_AND_ASPHALT_BLOCK             = 29,	// 砂＆アスファルトブロック(中間用)
		SAND_BLOCK					       = 30,	// 砂ブロック
		TORCH						       = 31,	// 松明
		SNAKE						       = 32,	// 蛇の壁画
		LEAF_LARGE					       = 33,	// 緑色の葉っぱ(大)
		LEAF_SMALL					       = 34,	// 緑色の葉っぱ(小)
		DRY_LEAF_LARGE				       = 35,	// 茶色の葉っぱ(大)
		DRY_LEAF_SMALL				       = 36,	// 茶色の葉っぱ(小)
		STONE_DRAGON				       = 37,	// 龍の石像
		STONE_SWORD					       = 38,	// 剣の石像
		SMOOTH_CONCRETE_BLOCK              = 39,	// なめらかなコンクリートブロック
		SMOOTH_CONCRETE_BLOCK_CRACKED      = 40,	// なめらかなひび割れたコンクリートブロック
		CONCRETE_BLOCK                     = 41,	// コンクリートブロック
		BACK_SMOOTH_CONCRETE_BLOCK         = 42,	// (後ろ)なめらかなコンクリートブロック
		BACK_SMOOTH_CONCRETE_BLOCK_CRACKED = 43,	// (後ろ)なめらかなひび割れたコンクリートブロック
		BACK_CONCRETE_BLOCK                = 44,	// (後ろ)コンクリートブロック
		CORAL_REEF_1                       = 45,	// サンゴ礁1
		CORAL_REEF_2                       = 46,	// サンゴ礁2
		CORAL_REEF_3                       = 47,	// サンゴ礁3
		STONE_1                            = 48,	// 岩1
		STONE_2                            = 49,	// 岩2
		BUILDING_1                         = 50,	// 建物1
		BUILDING_2                         = 51,	// 建物2
		WOOD_FENCE						   = 52,	// 木の柵
		OLD_HOUSE						   = 53,	// 平屋
		POWER_POLE						   = 54,	// 電柱(電線なし)
		POWER_POLE_L					   = 55,	// 電柱(左側に電線)
		POWER_POLE_R					   = 56,	// 電柱(右側に電線)
		POWER_POLE_B					   = 57,	// 電柱(両方に電線)
		BUS_STOP						   = 58,	// バス停
		STREET_LIGHT					   = 59,	// 街灯
		SIGN							   = 60,	// 看板
		MAX,
	};
	enum class OTHER_TEXTURE {
		// その他テクスチャ
		EFFECT,
		MAX,
	};
	enum class OTHER_MODEL {
		// その他モデル
		SOIL_BLOCK_GRASSY,
		MAX,
	};
	enum class OTHER_SETUP3D {
		// その他セットアップ3D
		BAOBAB_TREE,
		CHEST,
		PALM_TREE,
		MAX,
	};
	enum class OTHER_MOTION3D {
		// その他モーション3D
		CHEST_STEPPED,
		PLAMTREE_SHAKE,
		MAX,
	};
	enum class OTHER_SOUND {
		// その他サウンド
		COIN,
		LIGHT_A,
		LIGHT_B,
		MAX,
	};
	enum class MODEL_TYPE {
		// モデルタイプ
		PUT,
		MATERIAL_MESH,
		MESH,
	};
	enum class SET_TYPE {
		// 設定タイプ
		FORWARD,
		BACKWARD,
		FORWARD_AND_BACKWARD,
	};

	//========== [[[ 関数宣言 ]]]
	static void Load(void);
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetCollision(void) { return LOOKS_DATAS[(int)m_looksType].isCollision; }
	LOOKS_TYPE GetLooksType(void) { return m_looksType; }

	//========== [[[ リアクション用関数宣言 ]]]
	bool IsGetOld(const int nRot) { return m_isHitOlds[nRot]; }

	void IsReaction_HitsRot(const int nRot) { m_isHits[nRot] = true; }
	void IsReaction_Move(bool bFrag)  { ; } //上で移動している
	void IsReaction_Land(bool bFrag)  { ; } //上に着地した瞬間
	void IsReaction_TakeOff(bool bFrag){ ;}//上からジャンプで離れた瞬間

	//========== [[[ 構造体定義 ]]]
	struct LooksData {
		const char* modelPath;
		const char* pasteTexPath;
		Color col;
		MODEL_TYPE modelType;
		SET_TYPE setType;
		float height;
		float depth;
		bool isCollision;
	};

private:
	//========== [[[ 定数宣言 ]]]
	static const LooksData LOOKS_DATAS[(int)LOOKS_TYPE::MAX];
	static const char* OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX];
	static const char* OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX];
	static const char* OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX];
	static const char* OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX];
	static const char* OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX];

	//========== [[[ 変数宣言 ]]]
	static UShort  m_num;
	static Scale2D m_eyescale;
	static short   m_modelIdxes[(int)LOOKS_TYPE::MAX];
	static short   m_pasteTexIdxes[(int)LOOKS_TYPE::MAX];
	static short   m_otherTexIdxes[(int)OTHER_TEXTURE::MAX];
	static short   m_otherModelIdxes[(int)OTHER_MODEL::MAX];
	static short   m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
	static short   m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
	static short   m_otherSoundIdxes[(int)OTHER_SOUND::MAX];
	CDoll3D*   m_doll;
	LOOKS_TYPE m_looksType;		// 種類
	bool       m_isHitOlds[6];
	bool       m_isHits[6];
	Pos3D      m_targetAddPos;
	Pos3D      m_oldAddPos;
	Pos3D      m_addPos;
	short      m_counter;
	short      m_counterMax;
	int		   m_nTexIdx;
};