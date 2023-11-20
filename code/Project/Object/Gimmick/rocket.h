//========================================
// 
// ロケット
// Author:KEISUKE OTONO
// 
//========================================
// *** rocket.h ***
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include "../stage-object.h"

//****************************************
// クラス
//****************************************
class CRocket : public CStageObject {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class ANIME_STATE {
		NONE = -1,// 何もなし
		RIDE,	  // 乗る
		FLY,	  // 飛ぶ
		MAX,
	};
	// 共通情報
	struct TexInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 rot;		// 向き
		Color col;				// 頂点カラー
		int TexIdx;				// テクスチャ番号
	};
	struct Info
	{
		TexInfo Firetex;	// 炎テクスチャ情報
		TexInfo Smoketex;	// 煙テクスチャ情報
		D3DXVECTOR3 move;		// 移動量
		D3DXCOLOR col;			// 頂点カラー
		ANIME_STATE Animstate;	// アニメーションの状態
		Scale3D scale;			// スケール
		Scale3D Maxscale;		// 最大スケール
		float SmallSpeed;		// 小さくなる速度
		float fScaleMag;		// スケールの倍率
		short nModelIdx;		// モデル番号
		int	nRideAnimeCounter;	// ライドアニメーションカウンター
		int	nFlyAnimeCounter;	// 飛行アニメーションカウンター
		int nEffectAnimCounter;	// エフェクトアニメーションカウント
		bool bEffect;			// エフェクトの発生
	};
	
	CRocket();
	~CRocket();

	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);
	/* 乗る状態の更新*/void UpdateState_Ride(void);
	/* 飛ぶ状態の更新*/void UpdateState_Fly(void);

	// -- 設定 ---------------------------------------------
	/* 色			*/void SetColor(const D3DXCOLOR col) { m_Info.col = col; }
	/* モデル番号	*/void SetModelIdx(const int nIdx) { m_Info.nModelIdx = nIdx; }
	/* アニメーションの状態の設定*/void SetState(const ANIME_STATE state) { m_Info.Animstate = state; }
	/* 乗る準備完了*/	static void ReadyRide(void) { s_bReady = true; }
	/* 乗ってる状態		*/void Ride(void);
	/*搭乗しているプレイヤー数リセット*/static void ResetCounter(void) { s_nCountPlayer = 0; }

	// -- 所得 ---------------------------------------------
	/* 色		*/D3DXCOLOR GetColor(void) { return m_Info.col; }
	/*位置		*/D3DXVECTOR3& GetFirePos(void) { return m_Info.Firetex.pos; }
	/*現在の搭乗者数*/static int GetCounter(void) { return s_nCountPlayer; }

private:
	// *** 変数 ***
	Info m_Info;				// 共通情報
	static int  s_nCountPlayer;	// プレイヤーのカウント
	static bool s_bReady;		// 乗る準備が整ったか

	static const int   s_AnimeMax;
	static const int   s_RideAnimeMax;
	static const int   s_RotAnimeMax;
	static const int   s_FadeModeCountMax;
	static const float s_RideAnimeMag;
	static const float s_RideAnimeShrink;
	static const float s_RotAdd;
	static const float s_MoveMag;
	static const float s_MoveAdd;
};