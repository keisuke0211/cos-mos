//========================================
// 
// 伸びる犬の処理
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// モードの情報構造体
class CExtenddog : public CStageObject {
public:

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE = 0,		//乗っていない
		UP_LAND,		//上に着地
		DOWN_LAND,		//下に着地
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CExtenddog();
	virtual        ~CExtenddog();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void   SetState(STATE state) { m_state = state; }			//種類設定
	void   SetScale(D3DXVECTOR3 scale) { m_scale = scale; }		//拡縮設定
	void   SetShrink(bool shrink) { m_bShrink = shrink; }		//伸縮設定
	void   SetHead(float headposx) { m_fHeadposx = headposx; }	//頭の位置設定
	void   SetHip(float hipposx) { m_fHipposx = hipposx; }		//お尻の位置設定
	STATE  GetState(void) { return m_state; }					//種類取得

protected:

private:

	//========== [[[ 関数宣言 ]]]
	void   Collision(void);	//当たり判定処理

	//========== [[[ 変数宣言 ]]]
	STATE m_state;			// 種類
	int m_modelIdx[5];		// モデル番号
	D3DXVECTOR3 m_scale;	// 大きさ
	bool		m_bLand;	// 着地した？
	bool		m_bShrink;	// 縮む?
	int			m_nCntShrink;		// 縮むカウント
	float		m_fcurrenty;		// 現在のy座標
	float		m_fHeadposx;		// 頭の位置
	float		m_fHipposx;			// お尻の位置
};