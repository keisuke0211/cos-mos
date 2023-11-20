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
		SOIL_BLOCK,
		SOIL_BLOCK_GRASSY,
		ANCIENT_STONE_BLOCK,
		ANCIENT_STONE_BLOCK_PTN_A,
		ANCIENT_STONE_BLOCK_PTN_B,
		ANCIENT_STONE_BLOCK_PTN_C,
		ANCIENT_STONE_PILLAR_UP,
		ANCIENT_STONE_PILLAR_CENTER,
		ANCIENT_STONE_PILLAR_DOWN,
		MAX,
	};

	//========== [[[ 構造体定義 ]]]
	// 共通情報
	struct Info {
		LOOKS_TYPE looksType;	// 種類
		short      modelIdx;	// モデル番号
	};

	//========== [[[ 関数宣言 ]]]
	CBlock();
	~CBlock();
	HRESULT Init(LOOKS_TYPE type);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	//========== [[[ 定数宣言 ]]]
	static const char* MODEL_PATHS[(int)LOOKS_TYPE::MAX];

	//========== [[[ 変数宣言 ]]]
	Info m_Info;

};