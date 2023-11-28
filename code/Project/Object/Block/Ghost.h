//========================================
// 
// 幽霊の処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// 幽霊の情報構造体
class CGhost : public CStageObject {
public:

	CGhost();
	virtual		~CGhost();

	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	static CGhost *Create(D3DXVECTOR3 pos,int nplayernumber);	// 生成処理

	void Delete(void) { CStageObject::Delete(); }		// 削除処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; };		// 位置の設定
	void SetPlayer(int playernumber) { m_nPlayerNumber = playernumber; };	// プレイヤー番号

protected:

private:
	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 位置
	int m_nPlayerNumber;	// プレイヤー番号
	int m_TexIdx[2];		// テクスチャ
	int m_nMagCnt;			// 倍率カウント
	Scale2D m_Scale;		// スケール

};