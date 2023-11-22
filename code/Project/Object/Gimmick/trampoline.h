//========================================
// 
// トランポリンの処理
// Author:RYUKI FUJIWARA
// 
//========================================
#pragma once

#include "../stage-object.h"

//****************************************
// クラス定義
//****************************************
// モードの情報構造体
class CTrampoline : public CStageObject {
public:
	//========== [[[ 定数定義 ]]]
	static const int MAX_COUNT = 10;

	//========== [[[ 列挙型定義 ]]]
	//トランポリンの状態列挙
	enum class STATE {
		NONE = -1,	//乗っていない
		UP_LAND,	//上に着地
		DOWN_LAND,	//下に着地
		MAX,
	};

	//パーツ名列挙
	enum Parts
	{
		Parts_BASE = 0,	//土台
		Parts_MASH,		//キノコ
		Parts_SPRING,	//ばね
		Parts_EYE,		//目玉
		Parts_MAX,
	};

	//========== [[[ 関数宣言 ]]]
	CTrampoline();
	virtual        ~CTrampoline();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void   SetState(STATE state) { m_state = state; }	//種類設定
	STATE  GetState(void) { return m_state; }			//種類取得
	void SetCount(int nCount) { m_nSpringCounter = nCount; }

	void SetSpringForce(float fForce) { m_fSpringForce = fForce; }
	float GetSpringForce(void) { return m_fSpringForce; }

	//---------------------------------------------
	//バウンド設定
	//Author:HIRASAWA SHION
	//---------------------------
	//引数１　state： どっちの方向に着地したか
	//引数２　fForce：どれだけバウンドさせるか（最高Ｙ座標を代入）
	//---------------------------------------------
	void SetBound(STATE state, float fForce) { SetState(state); SetSpringForce(fForce); SetCount(MAX_COUNT); }

private:
	//========== [[[ 関数宣言 ]]]
	void PutModel(void);

	//========== [[[ 変数宣言 ]]]
	STATE       m_state;		// 種類
	D3DXVECTOR3 *m_pSpringPos;	// ばねの位置
	float       m_fSpringForce;
	float		m_fJamp;		// 差分
	int         m_modelIdx[Parts_MAX];	// モデル番号
	int			m_nSpringCounter;		// ばねのカウンター
};