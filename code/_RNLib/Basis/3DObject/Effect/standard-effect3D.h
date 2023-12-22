//========================================
// 
// 標準エフェクト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "effect3D_cylinder.h"

//****************************************
// クラス定義
//****************************************
// 標準エフェクト3Dクラス
class _RNC_StandardEffect3D {
public:
	//========== [[[ 関数宣言 ]]]
	_RNC_StandardEffect3D        ();
	~_RNC_StandardEffect3D       ();
	void SetPriority             (const UShort& priority) { m_priority = priority; }
	void CreateDustStormOnLanding(const Pos3D& pos, const Rot3D& rot, const Color& col, float force);

private:
	//========== [[[ 友達宣言 ]]]
	friend class C3DObject;

	//========== [[[ 関数宣言 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);

	//========== [[[ 変数宣言 ]]]
	UShort m_priority;
};