//========================================
// 
// デモオブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNmain.h"
#include "../Basis/Mechanical/object.h"

//****************************************
// クラス定義
//****************************************
// デモオブジェクトクラス
class CDemoObject : public CObject {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class TYPE { PLAYER, ZONE, MAX, };

	//========== [[[ 関数宣言 ]]]
	CDemoObject(const TYPE& type);
	virtual ~CDemoObject();
	void  Update (void) = 0;
	TYPE& GetType(void)             { return m_type; }
	void  SetType(const TYPE& type) { m_type = type; }

protected:
	//========== [[[ 変数宣言 ]]]
	TYPE m_type;
};