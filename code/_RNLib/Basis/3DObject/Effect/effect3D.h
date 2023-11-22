//========================================
// 
// エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../Mechanical/object-manager.h"

//****************************************
// クラス定義
//****************************************
// エフェクト3Dクラス
class CEffect3D {
public:
	//========== [[[ 構造体宣言 ]]]
	struct Circle {

	};

	//========== [[[ 関数宣言 ]]]
	CEffect3D();
	~CEffect3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void CreateCylinder(const Pos3D& pos, const UShort& divNum, const Circle& circleA, const Circle& circleB);
};