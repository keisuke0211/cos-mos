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

	static const int MAX_COUNT = 40;	// 最大カウント数
	static const int HIP_POS = 7;		// 尻の位置

	//========== [[[ 列挙型定義 ]]]
	enum class STATE {
		NONE = 0,		//乗っていない
		RETURN,			//戻る
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
	void   SetHead(D3DXVECTOR3 pos) { m_HeadPos = pos; }		//頭の位置設定
	void   SetHip(D3DXVECTOR3 pos) { m_HipPos = pos; }			//お尻の位置設定
	void   SetHeadHeight(int height) { m_nHeight = height; }	//頭の高さ
	void   SetReturn(bool bretrun) { m_bInversion = bretrun; }	//反転の設定
	STATE  GetState(void) { return m_state; }					//種類取得
	D3DXVECTOR3 GetHeadPos(void) { return m_HeadPos; }
	D3DXVECTOR3 GetHeadPosOid(void) { return m_HeadPosOid; }
	D3DXVECTOR3 GetBodyPos(void) { return m_BodyPos; }
	D3DXVECTOR3 GetHipPos(void) { return m_HipPos; }

protected:

private:

	//========== [[[ 変数宣言 ]]]
	STATE m_state;				// 種類
	int m_modelIdx[6];			// モデル番号
	D3DXVECTOR3 m_scale;		// 大きさ
	bool		m_bLand;		// 着地した？
	bool		m_bShrink;		// 縮む?
	bool		m_bInversion;	// 反転?
	int			m_nCntShrink;	// 縮むカウント
	float		m_fcurrenty;	// 現在のy座標
	int			m_nHeight;		// 高さ

	D3DXVECTOR3 m_HeadPos;		// 頭の位置
	D3DXVECTOR3 m_HeadPosOid;	// 頭の過去位置
	D3DXVECTOR3 m_BodyPos;		// 体の位置
	D3DXVECTOR3 m_HipPos;		// 尻の位置
};