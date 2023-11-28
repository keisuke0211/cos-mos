//========================================
// 
// ミスの処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"
#include "../Object/stage-object.h"

//****************************************
// クラス定義
//****************************************
// ミスの情報構造体
class CMiss : public CStageObject{
public:

	CMiss();
	virtual		~CMiss();

	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void Delete(void) { CStageObject::Delete(); }	// 削除処理
	static CMiss *Create();	// 生成処理

protected:

private:
	int m_TexIdx;		// テクスチャ
	int m_nMagCnt;		// 倍率カウント
	Scale2D m_Scale;	// スケール

};