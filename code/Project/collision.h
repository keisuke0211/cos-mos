#pragma once
//================================================================================================
// 
// 当たり判定処理[collision.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "Object/stage-object-manager.h"
#include "Character\player.h"

//当たり判定クラス
class CCollision{
public:
	// 当たり判定が働いた方向
	enum class ROT {
		NONE = 0,	// 何もなし
		OVER,		// 上
		UNDER,		// 下
		LEFT,		// 左
		RIGHT,		// 右
		UNKNOWN,	// 当たっているけど方向が分からない（当たられる側が動いている可能性アリ
		MAX
	};

	// 当たり判定を行うものの情報
	struct SelfInfo
	{
		Pos3D pos;	    // 位置
		Pos3D posOld;   // 前回位置
		Pos3D minPos;   // 最小位置
		Pos3D maxPos;   // 最大位置
		Pos3D minPosOld;// 前回最小位置
		Pos3D maxPosOld;// 前回最大位置
		Pos3D move;     // 移動量
		float fWidth;   // 幅
		float fHeight;  // 高さ
		float fRadius;  // 半径
	};

	// 当たり判定情報
	struct ColliInfo
	{
		Pos3D pos;      // 位置
		Pos3D posOld;   // 前回位置
		Pos3D minPos;   // 最小位置
		Pos3D maxPos;   // 最大位置
		Pos3D minPosOld;// 前回最小位置
		Pos3D maxPosOld;// 前回最大位置
		float fWidth;   // 幅
		float fHeight;  // 高さ
		float fRadius;  // 半径
		ROT   Rot;      // 当たった方向
	};

	CCollision();
	~CCollision();

	//========================
	// 矩形と矩形の当たり判定
	//------------------------
	// 引数１	self	：自分の情報
	// 引数２	target	：対象の情報
	// 引数３	value	：ベクトル
	// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
	//========================
	static ROT IsBoxToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec);

	//========================
	// 円と矩形の当たり判定
	//------------------------
	// 引数１	self	：自分の情報
	// 引数２	target	：対象の情報
	// 引数３	value	：ベクトル
	// 引数４	pAngle	：当たった方向を返す
	// 返り値	対象物にめりこんでいるか返す  true:めり込んだ  false:めり込んでいない
	//========================
	static bool CircleToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec, float *pAngle);

	void Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli,CObject *obj, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);

	//メモリ開放
	void OthColliDelete(void);

private:
	// 他パーツの当たり判定情報
	ColliInfo *m_pOthColli;

	//上下左右の当たり判定による位置・移動量修正
	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight);
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);

	//上下どちらかに当たった（乗った）プレイヤーの設定処理
	void LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY);

	//最小最大位置設定処理
	static void SetMinMaxPos(SelfInfo& self);
	static void SetMinMaxPos(ColliInfo& colli);
};