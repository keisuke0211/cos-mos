//================================================================================================
// 
// 当たり判定処理[collision.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "main.h"
#include "Character/player.h"
#include "collision.h"
#include "Sound/stage-sound-player.h"
#include "stage.h"

//=======================================
// コンストラクタ
//=======================================
CCollision::CCollision()
{

}

//=======================================
// デストラクタ
//=======================================
CCollision::~CCollision()
{

}

//========================
//距離を測る関数（判定するベクトル指定可能
//------------------------
//引数１・２  判定する座標など
//引数３～５  各ベクトルを使用するかどうか
//========================
float CCollision::Length(Pos3D& vec1, Pos3D& vec2, bool bXVec, bool bYVec, bool bZVec)
{
	Pos3D vectol = INITPOS3D;

	/*Xベクトル使用*/if (bXVec) vectol.x = vec1.x - vec2.x;
	/*Yベクトル使用*/if (bYVec) vectol.y = vec1.y - vec2.y;
	/*Zベクトル使用*/if (bZVec) vectol.z = vec1.z - vec2.z;

	//長さを返す
	return D3DXVec3Length(&vectol);
}

//========================
//2点の距離が、サイズの範囲内かどうかを判定（判定するベクトル指定可能
//------------------------
//引数１・２  判定する情報（位置とサイズ
//引数３・４  各ベクトルを使用するかどうか（サイズに奥行きは無いのでZベクトルは使用しない
//========================
bool CCollision::IsInRange(SelfInfo& self, ColliInfo& target, bool bXVec, bool bYVec, bool bUseSelfRadius, bool bUseTargetRadius)
{
	//２つの情報のサイズを格納
	Pos3D selfSize = INITPOS3D, targetSize = INITPOS3D;

	/*  幅代入  */ if (bXVec) { selfSize.x = bUseSelfRadius ? self.fRadius : self.fWidth;  targetSize.x = bUseTargetRadius ? target.fRadius : target.fWidth; }
	/* 高さ代入 */ if (bYVec) { selfSize.y = bUseSelfRadius ? self.fRadius : self.fHeight; targetSize.y = bUseTargetRadius ? target.fRadius : target.fHeight; }

	//位置・サイズの距離を取得
	CFloat PosLength = Length(self.pos, target.pos, bXVec, bYVec, false);
	CFloat SizeLength = Length(selfSize, targetSize, bXVec, bYVec, false);

	//範囲内に居るかどうか返す
	return PosLength < SizeLength;
}

//----------------------------
// 上からの当たり判定による位置修正
//----------------------------
void CCollision::FixPos_OVER(float *pPosY, float fMaxPosY, float *pMoveY, float fHeight)
{
	// 位置・移動量修正
	*pPosY = fMaxPosY + fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// 下からの当たり判定による位置修正
//----------------------------
void CCollision::FixPos_UNDER(float *pPosY, float fMinPosY, float *pMoveY, float fHeight)
{
	// 位置・移動量修正
	*pPosY = fMinPosY - fHeight;
	*pMoveY = 0.0f;
}

//----------------------------
// 左からの当たり判定による位置修正
//----------------------------
void CCollision::FixPos_LEFT(float *pPosX, float fMinPosX, float *pMoveX, float fWidth)
{
	// 位置・移動量修正
	*pPosX = fMinPosX - fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
// 右からの当たり判定による位置修正
//----------------------------
void CCollision::FixPos_RIGHT(float *pPosX, float fMaxPosX, float *pMoveX, float fWidth)
{
	// 位置・移動量修正
	*pPosX = fMaxPosX + fWidth;
	*pMoveX = 0.0f;
}

//----------------------------
//上下どちらかに当たった（乗った）プレイヤーの設定処理
// 概要 ----------------------
//・着地SE再生(ジャンプ中のみ)
//・bGround = true
//・bJump = false
//・fMaxHeight = fMaxY
//----------------------------
void CCollision::LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY)
{
	if (pInfo == NULL) return;

	// 着地SE再生
	if (pInfo->bJump == true) {
		CPlayer::PlaySE(CPlayer::SE_LABEL::LANDING);
		
		// エフェクト
		Pos3D createPos = pInfo->pos;
		Rot3D createRot = INITROT3D;

		if (pInfo->pos.y > 0.0f) {
			createPos.y -= CPlayer::SIZE_HEIGHT * 0.5f;
		}
		else {
			createPos.y += CPlayer::SIZE_HEIGHT * 0.5f;
			createRot.x += D3DX_PI;
		}
		RNLib::StandardEffect3D().CreateDustStormOnLanding(createPos, createRot, Color{ 169,158,93,255 }, 20.0f);

		if (pInfo->swapWaitCounter == 0) {

			// モーション
			pInfo->landingCounter = 30;
			pInfo->doll->OverwriteMotion(CPlayer::GetMotion(pInfo->idx).landing);
		}
	}

	pInfo->bGround = true;	// 地面に接している
	pInfo->bJump = false;	// ジャンプ可能
	pInfo->fMaxHeight = fMaxY;// 最高Ｙ座標設定
}

//----------------------------
// ブロックの当たり判定処理
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::Info& Player, CBlock* pBlock, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			if (// 見た目の種類による当たり判定の除外
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR   && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 && pSelfInfo->pos.y < 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 && pSelfInfo->pos.y < 0.0f) ||
				false)
				break;

			// 着地
			if (pSelfInfo->pos.y > 0.0f && !Player.bGroundOld)
				StageSoundPlayer::PlayLandingSEIdx(pBlock->GetLooksType());

			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 表の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->maxPos.y);
			}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			if (// 見た目の種類による当たり判定の除外
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR   && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 && pSelfInfo->pos.y > 0.0f) ||
				(pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 && pSelfInfo->pos.y > 0.0f) ||
				false)
				break;

			// 着地
			if (pSelfInfo->pos.y < 0.0f && !Player.bGroundOld)
				StageSoundPlayer::PlayLandingSEIdx(pBlock->GetLooksType());

			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 表の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);
			}
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:	
			if (// 見た目の種類による当たり判定の除外
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				false)
				break;

			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			if (// 見た目の種類による当たり判定の除外
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				false)
				break;

			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// 埋まった
			//*********************************
		case ROT::UNKNOWN:
			if (// 見た目の種類による当たり判定の除外
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_1 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_2 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::IRON_BAR_3 ||
				pBlock->GetLooksType() == CBlock::LOOKS_TYPE::PILE_OF_COINS ||
				false)
				break;

			*pDeath = true;

			break;
	}
}

//----------------------------
// 穴埋めブロックの当たり判定処理
//----------------------------
void CCollision::FillBlock(SelfInfo *pSelfInfo, ROT Rot, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	//死亡処理
	*pDeath = true;
}

//----------------------------
// トランポリンの当たり判定処理
//----------------------------
void CCollision::Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 表の世界のプレイヤー
			if (pSide != NULL &&
				*pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//プレイヤー取得
				CPlayer *pPlayer = Stage::GetPlayer();
				CPlayer::Info *pInfo = pPlayer->GetInfo(*pSide);

				// 着地した
				if (pSelfInfo->posOld.y > pSelfInfo->pos.y)
					pTrampoline->SetBound(CTrampoline::STATE::UP_LAND, pInfo->fMaxHeight);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->maxPos.y);

				//トランポリンによる跳ね上げ処理
				if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND)
					pPlayer->SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
			}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 裏の世界のプレイヤーならジャンプ可能
			if (pSide != NULL &&
				*pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//プレイヤー取得
				CPlayer *pPlayer = Stage::GetPlayer();
				CPlayer::Info *pInfo = pPlayer->GetInfo(*pSide);

				// 着地した
				if (pSelfInfo->posOld.y < pSelfInfo->pos.y)
					pTrampoline->SetBound(CTrampoline::STATE::DOWN_LAND, pInfo->fMaxHeight);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);

				//トランポリンによる跳ね上げ処理
				if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND)
					pPlayer->SetTrampolineJump(pInfo, pTrampoline->GetSpringForce());
			}
			break;
	}
}

//----------------------------
// トゲの当たり判定処理
//----------------------------
void CCollision::Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	switch (pColli->Rot)
	{
		//*********************************
		//上下どちらかに当たれば死亡
		//*********************************
		case ROT::OVER:
		case ROT::UNDER: 
		case ROT::UNKNOWN: *pDeath = true;break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;
	}
}

//----------------------------
// 移動床の当たり判定処理
//----------------------------
void CCollision::MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 表の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->maxPos.y);
				pSelfInfo->move = pMoveBlock->GetMove();
			}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

			// 裏の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);
				pSelfInfo->move = pMoveBlock->GetMove();
			}
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:	FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

		case ROT::UNKNOWN:
			// 移動床からの当たり判定
			for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
			{
				SelfInfo self;						ColliInfo colliInfo;
				self.pos = pColli->pos;				colliInfo.pos = pSelfInfo->pos;
				self.posOld = pColli->posOld;		colliInfo.posOld = pSelfInfo->posOld;
				self.fWidth = pColli->fWidth;		colliInfo.fWidth = pSelfInfo->fWidth;
				self.fHeight = pColli->fHeight;		colliInfo.fHeight = pSelfInfo->fHeight;

				// プレイヤーのどの方向に当たっているか
				pColli->Rot = IsBoxToBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

				// それでも当たらないなら、スキップ
				if (pColli->Rot == ROT::NONE || pColli->Rot == ROT::UNKNOWN) continue;

				// 当たった方向（上下・左右）を反転する
				{
					// 当たった方向をint型に変換
					const int nRot = (int)pColli->Rot;
					pColli->Rot = (ROT)(nRot - 1 + 2 * (nRot % 2));
				}

				// もう一度当たり判定
				MoveBlock(pSelfInfo, pMoveBlock, pColli);
			}
			break;
	}
}

//----------------------------
// 隕石の当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// 死亡処理
	*pDeath = true;
}

//----------------------------
// レーザーの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// 本体
	{
		// 当たった方向ごとに処理を切り替え
		switch (pColli->Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 表の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->maxPos.y);
					pSelfInfo->move = pRoadTripLaser->GetMove();
				}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 裏の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->minPos.y);
					pSelfInfo->move = pRoadTripLaser->GetMove();
				}
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN:
				// 移動床からの当たり判定
				for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
				{
					SelfInfo self;						ColliInfo colliInfo;
					self.pos = pColli->pos;				colliInfo.pos = pSelfInfo->pos;
					self.posOld = pColli->posOld;		colliInfo.posOld = pSelfInfo->posOld;
					self.fWidth = pColli->fWidth;		colliInfo.fWidth = pSelfInfo->fWidth;
					self.fHeight = pColli->fHeight;		colliInfo.fHeight = pSelfInfo->fHeight;

					// プレイヤーのどの方向に当たっているか
					pColli->Rot = IsBoxToBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

					// それでも当たらないなら、スキップ
					if (pColli->Rot == ROT::NONE || pColli->Rot == ROT::UNKNOWN) continue;

					// 当たった方向（上下・左右）を反転する
					{
						// 当たった方向をint型に変換
						const int nRot = (int)pColli->Rot;
						pColli->Rot = (ROT)(nRot - 1 + 2 * (nRot % 2));
					}

					// もう一度当たり判定
					Laser(pSelfInfo, pRoadTripLaser, pColli, pOthColli);
				}
				break;
		}
	}

	// レーザー
	{
		// 死亡処理
		//*pDeath = true;
	}
}

//----------------------------
// ヌイの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// ハウス
	{
		switch (pColli->Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 表の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 裏の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->minPos.y);
				}
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: *pDeath = true; break;
		}
	}

	//別の当たり判定のサイズを取得（通常通りなら【頭・体・足】の3つになる
	const int SIZE = sizeof pOthColli / sizeof ColliInfo;
	for (int nCntColli = 0; nCntColli < SIZE; nCntColli++)
	{
		switch (pOthColli[nCntColli].Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[nCntColli].maxPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 裏の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[nCntColli].minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 裏の世界のプレイヤー
				if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
				{
					//プレイヤー取得
					CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->minPos.y);
				}
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[nCntColli].minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[nCntColli].maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth); break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: *pDeath = true; break;
		}
	}
}

//----------------------------
// ゴールゲートの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli, CObject *obj, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	if (pSide == NULL) return;

	//プレイヤー取得
	CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

	if (!pInfo->bGoal)
	{
		//オブジェクトをキャスト
		CGoalGate *GoalGateObj = dynamic_cast<CGoalGate*>(obj);
		GoalGateObj->SetEntry(true);

		pInfo->bGoal = true;
		for (int ParCnt = 0; ParCnt < 8; ParCnt++)
		{
			Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::GOAL_EFFECT), pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
		}
	}
}

//----------------------------
// パーツの当たり判定処理
//----------------------------
void CCollision::Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	if (!pParts->GetDisp()) return;

	// 取得数増加・描画OFF
	pParts->DispSwitch(false);
	pParts->GetParts();

	// 取得した数が全体数と同じなら、ロケット乗車可能
	if (CParts::IsReady()) CRocket::ReadyRide();
}

//----------------------------
// ロケットの当たり判定処理
//----------------------------
void CCollision::Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	RNLib::Text2D().PutDebugLog("ロケットに当たっている");

	CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);
	if (!pRocket->GetReady() && !pInfo->bRide) return;

	// ロケットに搭乗
	if (!CPlayer::IsKeyConfigTrigger(pInfo->idx, *pSide, CPlayer::KEY_CONFIG::JUMP) ||
		pInfo->nRideInterval != 0) return;

	pInfo->bRide = true;
	pInfo->nRideInterval = CRocket::RIDE_ONOFF_INTERVAL;
	pRocket->RideOn();
	
	CInt ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	{
		Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	}
}

//----------------------------
// 杭の当たり判定処理
//----------------------------
void CCollision::Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide, bool *pDeath)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上下に当たった
		//*********************************
		case ROT::OVER:
		case ROT::UNDER:
			if (pSide != NULL)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);

				// 表の世界のプレイヤー
				if (*pSide == CPlayer::WORLD_SIDE::FACE)
				{
					//ある程度の高さから落下してきた
					if (pInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
						pPile->CaveInTrunkHeight(pColli->maxPos.y - pSelfInfo->pos.y - CPlayer::SIZE_HEIGHT);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->maxPos.y);
				}

				// 裏の世界のプレイヤー
				else
				{
					//ある程度の高さから落下してきた
					if (pInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
						pPile->CaveInTrunkHeight(pColli->minPos.y - pSelfInfo->pos.y + CPlayer::SIZE_HEIGHT);

					//プレイヤーオプション設定
					LandPlayerOption(pInfo, pColli->minPos.y);
				}

				pInfo->bLandPile = true;// 乗った
				pInfo->move.y = 0.0f;
			}
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:
			// 位置・移動量修正
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			// 位置・移動量修正
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);
			break;

			//*********************************
			// 埋まった
			//*********************************
		case CCollision::ROT::UNKNOWN:
		{
			if (pSide == NULL) return;

			if (CPlayer::GetSwapInterval() != 0) *pDeath = true;
			else if(pColli->vec == (int)CPlayer::VECTOL::Y)
			{
				//当たった方向を適当に上にして修正
				pColli->Rot = ROT::OVER;
				Pile(pSelfInfo, pColli, pPile, pSide, pDeath);

				//プレイヤー取得
				//CPlayer::Info *pInfo = Stage::GetPlayer()->GetInfo(*pSide);
				//
				//if (*pSide == CPlayer::WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
				//else 									pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
				//
				//pInfo->move.y = 0.0f;
				//pInfo->bLandPile = true;// 乗った
				//pInfo->bGround = true;	// 地面に接している
				//pInfo->bJump = false;	// ジャンプ可能
			}
		}break;
	}
}

//========================
// 矩形と矩形の当たり判定
//------------------------
// 引数１	self	：自分の情報
// 引数２	target	：対象の情報
// 引数３	value	：ベクトル
// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
//========================
CCollision::ROT CCollision::IsBoxToBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec)
{
	// 自分の現在と前回の最小・最大位置
	SetMinMaxPos(self);

	// 対象の現在と前回の最小・最大位置
	SetMinMaxPos(target);

	// 衝突ベクトルで処理分け
	switch (vec)
	{
		case CPlayer::VECTOL::X:
			if (RNLib::Number().GetIsLs(self.minPos.y, target.maxPos.y) && RNLib::Number().GetIsLs(target.minPos.y, self.maxPos.y))
			{// Y範囲内
				const bool isLeft  = RNLib::Number().GetIsGtOrEq(self.maxPos.x, target.minPos.x);
				const bool isRight = RNLib::Number().GetIsLsOrEq(self.minPos.x, target.maxPos.x);

				if (isLeft && (RNLib::Number().GetIsLsOrEq(self.maxPosOld.x, target.minPosOld.x) || RNLib::Number().GetIsLsOrEq(self.posOld.x, target.minPosOld.x)))
					return ROT::LEFT;

				if (isRight && (RNLib::Number().GetIsGtOrEq(self.minPosOld.x, target.maxPosOld.x) || RNLib::Number().GetIsGtOrEq(self.posOld.x, target.maxPosOld.x)))
					return ROT::RIGHT;

				if (isLeft && isRight)
					return ROT::UNKNOWN;
			}
			break;

		case CPlayer::VECTOL::Y:
			if (RNLib::Number().GetIsLs(self.minPos.x, target.maxPos.x) && RNLib::Number().GetIsLs(target.minPos.x, self.maxPos.x))
			{// X範囲内
				const bool isUnder = RNLib::Number().GetIsGtOrEq(self.maxPos.y, target.minPos.y);
				const bool isOver =  RNLib::Number().GetIsLsOrEq(self.minPos.y, target.maxPos.y);

				if (isUnder && (RNLib::Number().GetIsLsOrEq(self.maxPosOld.y, target.minPosOld.y) || RNLib::Number().GetIsLsOrEq(self.posOld.y, target.minPosOld.y)))
					return ROT::UNDER;

				if (isOver && (RNLib::Number().GetIsGtOrEq(self.minPosOld.y, target.maxPosOld.y) || RNLib::Number().GetIsGtOrEq(self.posOld.y, target.maxPosOld.y)))
					return ROT::OVER;

				if (isUnder && isOver)
					return ROT::UNKNOWN;
			}
			break;
	}

	// 当たらなかった
	return ROT::NONE;
}

//========================
// 円と矩形の当たり判定
//------------------------
// 引数１	self	：自分の情報
// 引数２	target	：対象の情報
// 引数３	pAngle	：当たった方向を返す
// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
//========================
bool CCollision::CircleToBoxCollider(SelfInfo& self, ColliInfo& target, float *pAngle)
{
	// 対象の現在と前回の最小・最大位置
	SetMinMaxPos(target);

	//距離が、自分の半径＋対象の対角線の長さより大きい
	if (IsInRange(self, target, true, true, true, false)) return false;

	//自分から対象までの角度
	const Pos3D PosDiff = self.pos - target.pos;
	CFloat Angle = atan2f(-PosDiff.x, -PosDiff.y);
	const Pos2D NearPos = Pos2D(sinf(Angle) * self.fRadius + self.pos.x,
								cosf(Angle) * self.fRadius + self.pos.y);

	//対象の中に、自分から対象までの最短ポイントがめり込んでいる
	if (target.minPos.x <= NearPos.x && NearPos.x <= target.maxPos.x &&
		target.minPos.y <= NearPos.y && NearPos.y <= target.maxPos.y)
	{
		//当たった方向を代入する
		if (pAngle != NULL)
		{
			//対象から自分までの角度
			*pAngle = atan2f(PosDiff.x, PosDiff.y);
		}

		//当たった
		return true;
	}

	//当たらなかった
	return false;
}

//========================
//最小最大位置設定処理
//========================
void CCollision::SetMinMaxPos(SelfInfo& self)
{
	// 自分の現在と前回の最小・最大位置
	self.minPos = Pos2D(self.pos.x - self.fWidth, self.pos.y - self.fHeight);
	self.maxPos = Pos2D(self.pos.x + self.fWidth, self.pos.y + self.fHeight);
	self.minPosOld = Pos2D(self.posOld.x - self.fWidth, self.posOld.y - self.fHeight);
	self.maxPosOld = Pos2D(self.posOld.x + self.fWidth, self.posOld.y + self.fHeight);
}

//========================
//最小最大位置設定処理
//========================
void CCollision::SetMinMaxPos(ColliInfo& colli)
{
	colli.minPos = Pos2D(colli.pos.x - colli.fWidth, colli.pos.y - colli.fHeight);
	colli.maxPos = Pos2D(colli.pos.x + colli.fWidth, colli.pos.y + colli.fHeight);
	colli.minPosOld = Pos2D(colli.posOld.x - colli.fWidth, colli.posOld.y - colli.fHeight);
	colli.maxPosOld = Pos2D(colli.posOld.x + colli.fWidth, colli.posOld.y + colli.fHeight);
}