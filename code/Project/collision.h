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
		Pos2D minPos;   // 最小位置
		Pos2D maxPos;   // 最大位置
		Pos2D minPosOld;// 前回最小位置
		Pos2D maxPosOld;// 前回最大位置
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
		Pos2D minPos;   // 最小位置
		Pos2D maxPos;   // 最大位置
		Pos2D minPosOld;// 前回最小位置
		Pos2D maxPosOld;// 前回最大位置
		float fWidth;   // 幅
		float fHeight;  // 高さ
		float fRadius;  // 半径
		ROT   Rot;      // 当たった方向
		int   vec;      // ベクトル
	};

	CCollision();
	~CCollision();

	//========================
	//距離を測る関数（判定するベクトル指定可能
	//------------------------
	//引数１・２  判定する座標など
	//引数３～５  各ベクトルを使用するかどうか
	//========================
	static float Length(Pos3D& vec1, Pos3D& vec2, bool bXVec, bool bYVec, bool bZVec);

	//========================
	//2点の距離が、サイズの範囲内かどうかを判定（判定するベクトル指定可能
	//------------------------
	//引数１・２  判定する情報（位置とサイズ
	//引数３・４  各ベクトルを使用するかどうか（サイズに奥行きは無いのでZベクトルは使用しない
	//引数５・６  各情報の半径を使用するかどうか
	//引数７      スワップインターバル中かどうか
	//========================
	static bool IsInRange(SelfInfo& self, ColliInfo& target, bool bXVec, bool bYVec, bool bUseSelfRadius = false, bool bUseTargetRadius = false, int ForPlayerInterval = 0);

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
	// 引数３	pAngle	：当たった方向を返す
	// 返り値	対象物にめりこんでいるか返す  true:めり込んだ  false:めり込んでいない
	//========================
	static bool CircleToBoxCollider(SelfInfo& self, ColliInfo& target, float *pAngle);

	static void Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::Info& Player, CBlock* pBlock, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli,CObject *obj, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);
	static void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide = NULL, bool *pDeath = NULL);

private:

	//上下左右の当たり判定による位置・移動量修正
	static void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight);
	static void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);
	static void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);
	static void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);

	//上下どちらかに当たった（乗った）プレイヤーの設定処理
	static void LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY);

	//最小最大位置設定処理
	static void SetMinMaxPos(SelfInfo& self);
	static void SetMinMaxPos(ColliInfo& colli);

	bool IsEqual(float& f11, float& f12, float& f21, float& f22);
};