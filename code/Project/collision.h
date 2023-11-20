#pragma once
//================================================================================================
// 
// 当たり判定処理[collision.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "Object/stage-object-manager.h"

//当たり判定クラス
class CCollision{
public:
	// 当たり判定の順番列挙
	enum class VEC {
		X = 0,	// Ⅹベクトル
		Y,		// Ｙベクトル
		MAX
	};

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
		Vector3D pos;		// 位置
		Vector3D posOld;	// 前回位置
		Vector3D minPos;	// 最小位置
		Vector3D maxPos;	// 最大位置
		Vector3D move;		// 移動量
		float	 fWidth;	// 幅
		float	 fHeight;	// 高さ
	};

	// 当たり判定情報
	struct ColliInfo
	{
		D3DXVECTOR3 pos;        // 位置
		D3DXVECTOR3 posOld;     // 前回位置
		D3DXVECTOR3 minPos;		// 最小位置
		D3DXVECTOR3 maxPos;		// 最大位置
		float		fWidth;		// 幅
		float		fHeight;	// 高さ
		ROT			Rot;		// 当たった方向
	};

	CCollision();
	~CCollision();

	//========================
	// 対象物の中にめり込んでいるかどうか判定
	//------------------------
	// 引数１	self	：自分の情報
	// 引数２	target	：対象の情報
	// 引数３	value	：ベクトル
	// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
	//========================
	ROT IsBoxCollider(SelfInfo& self, ColliInfo& target, VEC value);

	void Block(SelfInfo *pSelfInfo, ColliInfo *pColli);
	void FillBlock(SelfInfo *pSelfInfo, ROT ColliRot);
	void Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline);
	void Spike(SelfInfo *pSelfInfo, ColliInfo *pColli);
	void MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli);
	void Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli);
	void Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli);
	void Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli);
	void GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli);
	void Parts(SelfInfo *pSelfInfo, CParts *pParts);
	void Rocket(SelfInfo *pSelfInfo, CRocket *pRocket);
	void Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile);

	//メモリ開放
	void OthColliDelete(void);

private:
	ColliInfo *m_pOthColli;		// 他パーツの当たり判定情報

	void FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight);	// 上からの当たり判定による位置・移動量修正
	void FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight);	// 下からの当たり判定による位置・移動量修正
	void FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth);	// 左からの当たり判定による位置・移動量修正
	void FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth);	// 右からの当たり判定による位置・移動量修正
};