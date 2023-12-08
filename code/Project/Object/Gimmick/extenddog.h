//========================================
// 
// 伸びる犬の処理
// Author:KOMURO HIROMU
// Arrange:HIRASAWA SHION
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
		NONE = 0,  //乗っていない
		RETURN,    //戻る
		DOWN_LAND, //作動中
		MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CExtenddog();
	virtual      ~CExtenddog();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void) { ; }

	// -- 設定 ---------------------------------------------
	void  SetState(STATE state)     { m_state = state; }      //種類設定
	void  SetScale(Pos3D scale)     { m_scale = scale; }      //拡縮設定
	void  SetShrink(bool shrink)    { m_bShrink = shrink; }   //伸縮設定
	void  SetHead(Pos3D pos)        { m_HeadPos = pos; }      //頭の位置設定
	void  SetHip(Pos3D pos)         { m_HipPos = pos; }       //お尻の位置設定
	void  SetHeadHeight(int height) { m_nHeight = height; }   //頭の高さ
	void  SetReturn(bool bretrun)   { m_bInversion = bretrun; }//反転の設定

	// -- 取得 ---------------------------------------------
	STATE GetState(void)      { return m_state; }      //種類
	Pos3D GetHeadPos(void)    { return m_HeadPos; }    //頭位置
	Pos3D GetHeadPosOld(void) { return m_HeadPosOld; } //頭位置（前回
	Pos3D GetBodyPos(void)    { return m_BodyPos; }    //体位置
	Pos3D GetHipPos(void)     { return m_HipPos; }     //お尻位置

private:
	//========== [[[ 関数宣言 ]]]
	void UpdateState_None(void);
	void UpdateState_Return(void);
	void UpdateState_DownLand(void);

	//========== [[[ 変数宣言 ]]]
	STATE m_state;      // 種類
	int   m_modelIdx[6];// モデル番号
	Pos3D m_scale;      // 大きさ
	bool  m_bLand;      // 着地した？
	bool  m_bShrink;    // 縮む?
	bool  m_bInversion; // 反転?
	int   m_nCntShrink; // 縮むカウント
	float m_fcurrenty;  // 現在のy座標
	int   m_nHeight;    // 高さ

	Pos3D m_HeadPos;    // 頭の位置
	Pos3D m_HeadPosOld; // 頭の過去位置
	Pos3D m_BodyPos;    // 体の位置
	Pos3D m_HipPos;     // 尻の位置
};