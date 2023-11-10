//========================================
// 
// 背景エディットの読み込み
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../main.h"

//****************************************
// クラス定義
//****************************************
class CBGEditor {

public:

	enum class WORLD_SIDE {
		NONE = 0,
		UP,			//上
		DOWN,		//下
		MAX,
	};

	//プレイヤー情報
	struct INFO
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 spin;
		Color		col;
		char		ModelPath[128];
		WORLD_SIDE  side;
	};

	// *** 関数宣言 ***
	CBGEditor();
	~CBGEditor();

	static void Load(const char* filepath);	//背景読込処理

private:

	static INFO m_Info;
};
