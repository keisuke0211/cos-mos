//================================================================================================
// 
// 当たり判定処理[collision.cpp]
// Author:Hirasawa Shion
// 
//================================================================================================
#include "main.h"
#include "Character/player.h"
#include "collision.h"

//=======================================
// コンストラクタ
//=======================================
CCollision::CCollision()
{
	m_pOthColli = NULL;
}

//=======================================
// デストラクタ
//=======================================
CCollision::~CCollision()
{
	OthColliDelete();
}

//----------------------------
// 他の当たり判定情報のメモリ解放
//----------------------------
void CCollision::OthColliDelete(void)
{
	if (m_pOthColli == NULL) return;
	delete m_pOthColli;
	m_pOthColli = NULL;
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
//----------------------------
void CCollision::LandPlayerOption(CPlayer::Info *pInfo, const float fMaxY)
{
	if (pInfo == NULL) return;

	// 着地SE再生
	if (pInfo->bJump == true)
		CPlayer::PlaySE(CPlayer::SE_LABEL::LANDING);

	pInfo->bGround = true;	// 地面に接している
	pInfo->bJump = false;	// ジャンプ可能
	pInfo->fMaxHeight = fMaxY;// 最高Ｙ座標設定
}

//----------------------------
// ブロックの当たり判定処理
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide)
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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

			// 表の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);
			}
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:	FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:	FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pSelfInfo->fWidth);	break;

			//*********************************
			// 埋まった
			//*********************************
		case ROT::UNKNOWN: break; CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
	}
}

//----------------------------
// 穴埋めブロックの当たり判定処理
//----------------------------
void CCollision::FillBlock(SelfInfo *pSelfInfo, ROT Rot, CPlayer::WORLD_SIDE *pSide)
{
	//死亡処理
	CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
}

//----------------------------
// トランポリンの当たり判定処理
//----------------------------
void CCollision::Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline, CPlayer::WORLD_SIDE *pSide)
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				if (pSelfInfo->posOld.y > pSelfInfo->pos.y)
				{// 着地した
					pTrampoline->SetState(CTrampoline::STATE::UP_LAND);
					pTrampoline->SetSpringForce(pInfo->fMaxHeight);
					pTrampoline->SetCount(CTrampoline::MAX_COUNT);
				}

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->maxPos.y);

				if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND) {
					//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
				}
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				if (pSelfInfo->posOld.y < pSelfInfo->pos.y)
				{// 着地した
					pTrampoline->SetState(CTrampoline::STATE::DOWN_LAND);
					pTrampoline->SetSpringForce(pInfo->fMaxHeight);
					pTrampoline->SetCount(CTrampoline::MAX_COUNT);
				}

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);

				if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND) {
					//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
				}
			}
			break;
	}
}

//----------------------------
// トゲの当たり判定処理
//----------------------------
void CCollision::Spike(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide)
{
	switch (pColli->Rot)
	{
		//上下どちらかに当たれば死亡
		case ROT::OVER:
		case ROT::UNDER:
			// 死亡処理
			CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
			break;

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
void CCollision::MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide)
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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
				pColli->Rot = IsBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

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
void CCollision::Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide)
{
	// 死亡処理
	CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
}

//----------------------------
// レーザーの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide)
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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					pColli->Rot = IsBoxCollider(self, colliInfo, (CPlayer::VECTOL)nCntVec);

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
		CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
	}
}

//----------------------------
// ヌイの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli, CPlayer::WORLD_SIDE *pSide)
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
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
				//	if (pSelfInfo->bJump == true)
				//	{// 着地した
				//	 // SE再生
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pSelfInfo->fHeight);

				// 裏の世界のプレイヤーならジャンプ可能
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
				//	if (pSelfInfo->bJump == true)
				//	{// 着地した
				//	 // SE再生
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
				//}
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
			case ROT::UNKNOWN: CMode_Game::GetPlayer()->Death(&pSelfInfo->pos); break;
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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
					CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

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
			case ROT::UNKNOWN: CMode_Game::GetPlayer()->Death(&pSelfInfo->pos); break;
		}
	}
}

//----------------------------
// ゴールゲートの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli, CPlayer::WORLD_SIDE *pSide)
{
	if (pSide == NULL) return;

	//プレイヤー取得
	CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

	if (!pInfo->bGoal)
	{
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
void CCollision::Parts(SelfInfo *pSelfInfo, CParts *pParts, CPlayer::WORLD_SIDE *pSide)
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
void CCollision::Rocket(SelfInfo *pSelfInfo, CRocket *pRocket, CPlayer::WORLD_SIDE *pSide)
{
	//const int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");

	//if (!CPlayer::s_bRideRocket) return;

	// ロケットに搭乗
	//pSelfInfo->bRide = true;
	//pRocket->Ride();
	//
	//for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	//{
	//	Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	//}
}

//----------------------------
// 杭の当たり判定処理
//----------------------------
void CCollision::Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile, CPlayer::WORLD_SIDE *pSide)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 表の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::FACE)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				//ある程度の高さから落下してきた
				if (pInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
				{
					pPile->CaveInTrunkHeight(pColli->maxPos.y - pSelfInfo->pos.y - CPlayer::SIZE_HEIGHT);
				}

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->maxPos.y);
				pInfo->bLandPile = true;// 乗った
				pInfo->move.y = 0.0f;
			}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 裏の世界のプレイヤー
			if (pSide != NULL && *pSide == CPlayer::WORLD_SIDE::BEHIND)
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				//ある程度の高さから落下してきた
				if (pInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
				{
					pPile->CaveInTrunkHeight(pColli->minPos.y - pSelfInfo->pos.y + CPlayer::SIZE_HEIGHT);
				}

				//プレイヤーオプション設定
				LandPlayerOption(pInfo, pColli->minPos.y);
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

			if (CPlayer::GetSwapInterval() != 0) CMode_Game::GetPlayer()->Death(&pSelfInfo->pos);
			else
			{
				//プレイヤー取得
				CPlayer::Info *pInfo = CMode_Game::GetPlayer()->GetInfo(*pSide);

				if (*pSide == CPlayer::WORLD_SIDE::FACE)pInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
				else 									pInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定

				pInfo->move.y = 0.0f;
				pInfo->bLandPile = true;// 乗った
				pInfo->bGround = true;	// 地面に接している
				pInfo->bJump = false;	// ジャンプ可能
			}
		}break;
	}
}

//========================
// 対象物の中にめり込んでいるかどうか判定
//------------------------
// 引数１	self	：自分の情報
// 引数２	target	：対象の情報
// 引数３	value	：ベクトル
// 返り値	対象物にめりこんでいる方向を返す（NONEなら当たっていない
//========================
CCollision::ROT CCollision::IsBoxCollider(SelfInfo& self, ColliInfo& target, CPlayer::VECTOL vec)
{
	// 自分の現在の最小・最大位置
	self.minPos = Pos3D(self.pos.x - self.fWidth, self.pos.y - self.fHeight, 0.0f);
	self.maxPos = Pos3D(self.pos.x + self.fWidth, self.pos.y + self.fHeight, 0.0f);

	// 対象の現在の最小・最大位置
	target.minPos = Pos3D(target.pos.x - target.fWidth, target.pos.y - target.fHeight, 0.0f);
	target.maxPos = Pos3D(target.pos.x + target.fWidth, target.pos.y + target.fHeight, 0.0f);

	// めり込んでいるか判定
	if (self.minPos.x < target.maxPos.x && target.minPos.x < self.maxPos.x &&
		self.minPos.y < target.maxPos.y && target.minPos.y < self.maxPos.y)
	{
		// 自分の過去の最小・最大位置
		const Pos3D OLD_MINPOS = Pos3D(self.posOld.x - self.fWidth, self.posOld.y - self.fHeight, 0.0f);
		const Pos3D OLD_MAXPOS = Pos3D(self.posOld.x + self.fWidth, self.posOld.y + self.fHeight, 0.0f);

		// 対象の前回の最小・最大位置
		const Pos3D TARGET_MinPosOld = Pos3D(target.posOld.x - target.fWidth, target.posOld.y - target.fHeight, 0.0f);
		const Pos3D TARGET_MaxPosOld = Pos3D(target.posOld.x + target.fWidth, target.posOld.y + target.fHeight, 0.0f);

		// 衝突ベクトルで処理分け
		switch (vec)
		{
			case CPlayer::VECTOL::X:
				// 前回は左からめり込んでいない（今はめり込んだ
				if (OLD_MAXPOS.x <= TARGET_MinPosOld.x)			return ROT::LEFT;

				// 前回は右からめり込んでいない（今はめり込んだ
				else if (OLD_MINPOS.x >= TARGET_MaxPosOld.x)	return ROT::RIGHT;

				//対象が左に動いている
				else if (target.posOld.x > target.pos.x) return ROT::RIGHT;

				//対象が右に動いている
				else if (target.posOld.x < target.pos.x) return ROT::LEFT;
				break;

			case CPlayer::VECTOL::Y:
				// 前回は上からめり込んでいない（今はめり込んだ
				if (OLD_MINPOS.y >= TARGET_MaxPosOld.y)			return ROT::OVER;

				// 前回は下からめり込んでいない（今はめり込んだ
				else if (OLD_MAXPOS.y <= TARGET_MinPosOld.y)	return ROT::UNDER;

				//対象が上に動いている
				else if (target.posOld.y < target.pos.y) return ROT::OVER;

				//対象が下に動いている
				else if (target.posOld.y > target.pos.y) return ROT::UNDER;
				break;
		}

		// 当たった方向が分からない
		return ROT::UNKNOWN;
	}

	// 当たらなかった
	return ROT::NONE;
}