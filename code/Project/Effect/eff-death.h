#pragma once
//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "../../_RNLib/RNlib.h"
#include "../collision.h"

// 死亡パーティクルクラス
class CEffect_Death : public CStageObject {
public:
	//種類
	enum class TYPE {
		NONE = 0,
		BALL, //ボール
		INK,  //インク
	};

	//ボールのサイズレベル
	enum class BALL_SIZE_LV {
		SMALL = 0, // サイズ小
		MIDIUM,    // サイズ中
		BIG,       // サイズ大(最大)
		MAX
	};

	//自己情報
	struct Info
	{
		//設定処理によって個別に設定される項目
		Vector3D move   = INITVECTOR3D; // 移動量
		Vector3D spin   = INITVECTOR3D; // 回転量
		float    size   = 0.0f;         // サイズ（半径）
		int      nLife  = NONEDATA;     // 寿命（不死身ならNONEDATA
		int      nIdx = NONEDATA;       // テクスチャ or モデル番号
		TYPE     type   = TYPE::NONE;   // 種類

		//ボール用
		BALL_SIZE_LV *pLv = NULL; // サイズレベル
		int nBallID = NONEDATA;   // ボール番号

		//インク用
		Pos2D InkSize = INITPOS2D;// インクサイズ

		//設定処理を通れば、皆共通で代入される項目
		int  ColliderInterval; // 特定の当たり判定を働かせないインターバル（全ての当たり判定に対してではない
		bool bDeath;           // 死亡フラグ
	};

	CEffect_Death();
	~CEffect_Death();

	void Uninit(void) { ; }
	void Update(void);

	//******************************
	//設定処理（自己情報を一括設定）
	//******************************
	void SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const TYPE type);

	//******************************
	//設定処理（自己情報を個別設定）
	//******************************
	void SetMove(const Vector3D move)     { m_Info.move = move; }
	void SetSpin(const Vector3D spin)     { m_Info.spin = spin; }
	void SetSize(const float size)        { m_Info.size = size; }
	void SetLife(const int nLife)         { m_Info.nLife = nLife; }
	void SetIdx(const int nIdx)           { m_Info.nIdx = nIdx; }
	void SetType(const TYPE type)         { m_Info.type = type; }

	//******************************
	//ボール用設定処理
	//******************************
	void SetBallSize(BALL_SIZE_LV Lv);

private:
	static CInt CREATE_INTERVAL = 30;  //生成インターバル
	static CInt FUSION_INTERVAL = 10;  //融合インターバル
	static CInt KICK_INTERVAL = 20;    //蹴飛ばしインターバル

	static CInt MAX_INK_LIFE = 600;    //インクの寿命

	static CFloat CREATE_SPREAD_POWER; //生成時の拡散力
	static CFloat PLAYER_COLLI_POWER;  //プレイヤーに当たったときの吹っ飛び力
	static CFloat PLAYER_KICK_POWER;   //プレイヤーのキック量
	static CFloat MOVE_X_CORRECT;      //Ⅹベクトルの移動補正係数
	static CFloat GRAVITY_POWER;       //重力加速度
	static CFloat BOUND_POWER;         //バウンド係数
	static const short BALL_ALPHA_DECREASE; //ボールのα値減少量（当たり判定でUnknownが出た際に使用
	static CFloat BALL_SIZE[(int)BALL_SIZE_LV::MAX];//ボールサイズを格納
	static       int   s_BallModelID[(int)BALL_SIZE_LV::MAX]; //ボールモデル番号を格納
	static const char *BALL_MODEL_PATH[(int)BALL_SIZE_LV::MAX];//ボールモデルパスを格納

	//種別更新処理
	void UpdateType_Ball(void);
	void UpdateType_Ink(void);

	//ボール合体処理
	void BallFusion(void);

	//情報更新処理
	CCollision::ROT StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type);
	void SetSelfInfo(CCollision::SelfInfo *pSelfInfo);
	void Move(CPlayer::VECTOL vec);
	void Spin(void);
	void PutPolygon(void);
	void PutModel(void);
	void Life(void);
	void Death(void);

	static int s_nNumAllBall;
	static int s_FusionSE;

	Info m_Info;
};