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
	void  SetElasticity(bool Elast) { m_bElasticity = Elast; }//伸縮設定
	void  SetHead(Pos3D pos)        { m_HeadPos = pos; }      //頭の位置設定
	void  SetHip(Pos3D pos)         { m_HipPos = pos; }       //お尻の位置設定
	void  SetHeadHeightMin(CInt heightMin) { m_nHeightMin = heightMin; } //頭の高さ（最低
	void  SetHeadHeightMax(CInt heightMax) { m_nHeightMax = heightMax; } //頭の高さ（最高
	void  SetReturn(bool bReturn)   { m_bInversion = bReturn; }//反転の設定

	// -- 取得 ---------------------------------------------
	STATE GetState(void)      { return m_state; }      //種類
	STATE GetStateOld(void)	  { return m_stateOld; }   //過去の種類
	Pos3D GetHeadPos(void)    { return m_HeadPos; }    //頭位置
	Pos3D GetBodyPos(void)    { return m_BodyPos; }    //体位置
	Pos3D GetBodyPosOld(void) { return m_BodyPosOld; } //体位置（前回
	Pos3D GetHipPos(void)     { return m_HipPos; }     //お尻位置
	bool  IsInversion(void)   { return m_bInversion; } //反転してるかどうか
	float GetColliHeight(void); //当たり判定用の高さ取得
	int   GetHipRot(void); //お尻の方向を取得

private:
	//頭・お尻用サイズ
	static const float HEAD_HIP_SIZE;

	//========== [[[ 列挙型定義 ]]]
	enum class Parts
	{
		Hip = 0, // お尻
		Body,    // 体
		Head,    // 頭
		Max
	};
	//========== [[[ 関数宣言 ]]]
	void UpdateState_None(void);
	void UpdateState_Return(void);

	//========== [[[ 変数宣言 ]]]
	int   m_modelIdx[(int)Parts::Max]; // モデル番号
	static const char *MODEL_PATH[(int)Parts::Max];//モデルパス
	STATE m_state;       // 種類
	STATE m_stateOld;	 // 過去の種類
	Pos3D m_scale;       // 大きさ
	bool  m_bElasticity; // 触って縮むか伸びるか
	bool  m_bInversion;  // 反転
	int   m_nCntShrink;  // 縮むカウント
	int   m_nHeightMin;  // 伸びる高さ（最低
	int   m_nHeightMax;  // 伸びる高さ（最高

	Pos3D m_HeadPos;     // 頭の位置
	Pos3D m_BodyPos;     // 体の位置
	Pos3D m_BodyPosOld;  // 体の過去位置
	Pos3D m_HipPos;      // 尻の位置

	Pos3D m_StartBodyPos;// 体の初期位置
	float m_StartHeight; // 高さの初期位置
};