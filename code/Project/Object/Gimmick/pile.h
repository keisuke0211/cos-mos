#pragma once
//================================================================================================
// 
// 杭処理[pile.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"

// 杭クラス
class CPile : public CStageObject
{
public:
	//最低幹数
	static const int MIN_TRUNK = 3;

	CPile();
	virtual      ~CPile();
	virtual void Init(void);
	virtual void Uninit(void);
	virtual void Update(void);

	//===============================
	//設定処理
	//引数に必要高さ（最低3）や配置座標を指定する
	//-------------------------------
	//引数１  pos：配置座標
	//引数２  NumTrunk：幹の数（最低３個 MIN_TRUNK）
	//引数３  TrunkHeight：幹のめり込み座標（めり込みなし = 0.0f）
	//===============================
	void Set(Pos3D pos, int NumTrunk, float TrunkHeight);

private:
	void SetModel(void);//モデル配置

	int m_BodyModelID;	//本体のモデル番号
	int m_TrunkModelID;	//幹部分のモデル番号
	Pos3D m_pos;		//本体座標
	float m_TrunkHeight;//幹座標
	int   m_NumTrunk;	//幹の数（最低３個）
};