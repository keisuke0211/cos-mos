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

	//杭をめり込ませる最低高さ
	static const float CAVEIN_DIFF_HEIGHT;

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
	//引数３  TrunkHeight：幹のめり込み係数（めり込みなし = 0.0f）
	//===============================
	void Set(Pos3D pos, int NumTrunk, float TrunkHeight);

	//===============================
	//めり込み量反映処理（Cave in  ==  へこむ
	//===============================
	void CaveInTrunkHeight(float fCaveInHeight);

	//===============================
	//めり込み修正量取得
	//===============================
	float GetCorrHeight(void) { return m_fEvenTrunkCorrHeight; }

	//===============================
	//本体座標取得
	//===============================
	Pos3D GetPilePos(void) { return m_PilePos; }

	//===============================
	//当たり判定情報取得
	//引数１　中心位置
	//引数２　中心位置（前回
	//引数３　高さ
	//===============================
	void GetCollisionInfo(Pos3D& CenterPos, Pos3D& CenterPosOld, float& height);

	//===============================
	//全体のめり込み量を初期化
	//===============================
	static void ResetTrunkHeightAll(void);

private:
	void SetOld(float fCaveInHeight);	//前回情報を保存
	void PutModel(void);//モデル配置

	int m_TrunkModelID;	//幹部分のモデル番号
	Pos3D m_PilePos;	//本体座標
	int   m_NumTrunk;	//幹の数（最低３個）
	float m_fEvenTrunkCorrHeight;//幹の数が偶数だった場合の補正値（奇数なら0.0f
	float m_TrunkHeight;	//幹座標
	float m_TrunkHeightOld;	//前回の幹座標
	float m_StartTrunkHeight;

	//エフェクト
	int m_nTex[2];			//テクスチャ番号
};