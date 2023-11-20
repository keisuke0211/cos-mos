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
	delete[] m_pOthColli;
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
// ブロックの当たり判定処理
//----------------------------
void CCollision::Block(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 表の世界のプレイヤー
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
			//	if (pSelfInfo->bJump == true)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
			//}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 裏の世界のプレイヤーならジャンプ可能
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
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
			// 左に当たった
			//*********************************
		case ROT::LEFT:
			// 位置・移動量修正
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			// 位置・移動量修正
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 埋まった
			//*********************************
		case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
	}
}

//----------------------------
// 穴埋めブロックの当たり判定処理
//----------------------------
void CCollision::FillBlock(SelfInfo *pSelfInfo, ROT Rot)
{
	//Death(&pSelfInfo->pos);
}

//----------------------------
// トランポリンの当たり判定処理
//----------------------------
void CCollision::Trampoline(SelfInfo *pSelfInfo, ColliInfo *pColli, CTrampoline *pTrampoline)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 表の世界のプレイヤー
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	if (pSelfInfo->posOld.y > pSelfInfo->pos.y)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//		pTrampoline->SetState(CTrampoline::STATE::UP_LAND);
			//		pTrampoline->SetSpringForce(pSelfInfo->fMaxHeight);
			//		pTrampoline->SetCount(CTrampoline::MAX_COUNT);
			//	}
			//
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
			//
			//	if (pTrampoline->GetState() == CTrampoline::STATE::DOWN_LAND) {
			//		//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
			//	}
			//}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 裏の世界のプレイヤーならジャンプ可能
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	if (pSelfInfo->posOld.y < pSelfInfo->pos.y)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//		pTrampoline->SetState(CTrampoline::STATE::DOWN_LAND);
			//		pTrampoline->SetSpringForce(pSelfInfo->fMaxHeight);
			//		pTrampoline->SetCount(CTrampoline::MAX_COUNT);
			//	}
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
			//
			//	if (pTrampoline->GetState() == CTrampoline::STATE::UP_LAND) {
			//		//SetTrampolineJump(pSelfInfo, pTrampoline->GetSpringForce());
			//	}
			//}
			break;
	}
}

//----------------------------
// トゲの当たり判定処理
//----------------------------
void CCollision::Spike(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	switch (pColli->Rot)
	{
		//上下どちらかに当たれば死亡
		case ROT::OVER:
		case ROT::UNDER:
			// 死亡処理
			//Death(&pSelfInfo->pos);
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:
			// 位置・移動量修正
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			// 位置・移動量修正
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;
	}
}

//----------------------------
// 移動床の当たり判定処理
//----------------------------
void CCollision::MoveBlock(SelfInfo *pSelfInfo, CMoveBlock *pMoveBlock, ColliInfo *pColli)
{
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 位置・移動量修正
			FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 表の世界のプレイヤーの場合
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	pSelfInfo->move = pMoveBlock->GetMove();
			//	if (pSelfInfo->bJump == true)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
			//}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 位置・移動量修正
			FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

			// 裏の世界のプレイヤーならジャンプ可能
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	pSelfInfo->pos = pMoveBlock->GetMove();
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
			// 左に当たった
			//*********************************
		case ROT::LEFT:
			// 位置・移動量修正
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			// 位置・移動量修正
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

		case ROT::UNKNOWN:
			// 移動床 -> プレイヤーへの当たり判定処理を実行
			const D3DXVECTOR3 BlockPos = pMoveBlock->GetPos();
			const D3DXVECTOR3 BlockPosOld = pMoveBlock->GetPosOld();
			const float fWidth = pMoveBlock->GetWidth() * 0.5f;
			const float fHeight = pMoveBlock->GetHeight() * 0.5f;

			// 移動床からの当たり判定
			for (int nCntVec = 0; nCntVec < (int)VEC::MAX; nCntVec++)
			{
				// プレイヤーのどの方向に当たっているか
				//pColli->Rot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pSelfInfo->pos, pSelfInfo->posOld, pColli->fWidth, pColli->fHeight, (VEC)nCntVec);

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
void CCollision::Meteor(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	// 死亡処理
	//Death(&pSelfInfo->pos);
}

//----------------------------
// レーザーの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Laser(SelfInfo *pSelfInfo, CRoadTripLaser *pRoadTripLaser, ColliInfo *pColli, ColliInfo *pOthColli)
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
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// 表の世界のプレイヤーの場合
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
				//{
				//	pSelfInfo->move = pRoadTripLaser->GetMove();
				//	if (pSelfInfo->bJump == true)
				//	{// 着地した
				//	 // SE再生
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

				// 表の世界のプレイヤーの場合
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
				//{
				//	pSelfInfo->move = pRoadTripLaser->GetMove();
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
				// 左に当たった
				//*********************************
			case ROT::LEFT:
				// 位置・移動量修正
				FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:
				// 位置・移動量修正
				FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN:
				// レーザー -> プレイヤーへの当たり判定処理を実行
				const D3DXVECTOR3 BlockPos = pRoadTripLaser->GetPos();
				const D3DXVECTOR3 BlockPosOld = pRoadTripLaser->GetPosOld();
				const float fWidth = pRoadTripLaser->GetWidth() * 0.5f;
				const float fHeight = pRoadTripLaser->GetHeight() * 0.5f;

				// 移動床からの当たり判定
				for (int nCntVec = 0; nCntVec < (int)VEC::MAX; nCntVec++)
				{
					// プレイヤーのどの方向に当たっているか
					//pColli->Rot = IsBoxCollider(BlockPos, BlockPosOld, fWidth, fHeight, pSelfInfo->pos, pSelfInfo->posOld, pColli->fWidth, pColli->fHeight, (VEC)nCntVec);

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
		//Death(&pSelfInfo->pos);
	}
}

//----------------------------
// ヌイの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::Dog(SelfInfo *pSelfInfo, CExtenddog *pExtenddog, ColliInfo *pColli, ColliInfo *pOthColli)
{
	// 当たった方向ごとに処理を切り替え
	CExtenddog::STATE State = pExtenddog->GetState();

	// ハウス
	{
		switch (pColli->Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pColli->maxPos.y, &pSelfInfo->move.y, pColli->fHeight);

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
				FixPos_UNDER(&pSelfInfo->pos.y, pColli->minPos.y, &pSelfInfo->move.y, pColli->fHeight);

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
			case ROT::LEFT:
				// 位置・移動量修正
				FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
				break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:
				// 位置・移動量修正
				FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fHeight);
				break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}

	// 頭
	{
		CExtenddog::STATE state = pExtenddog->GetState();
		switch (pOthColli[0].Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[0].maxPos.y, &pSelfInfo->move.y, pOthColli[0].fHeight);

				// 表の世界のプレイヤーの場合
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE){
				//	if (pSelfInfo->bJump == true)
				//	{// 着地した
				//	 // SE再生
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[0].maxPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[0].minPos.y, &pSelfInfo->move.y, pOthColli[0].fHeight);

				// 表の世界のプレイヤーの場合
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE){
				//	if (pSelfInfo->bJump == true)
				//	{// 着地した
				//	 // SE再生
				//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
				//	}
				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[0].maxPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:
				// 位置・移動量修正
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[0].minPos.x, &pSelfInfo->move.x, pOthColli[0].fWidth);
				break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:

				if (state == CExtenddog::STATE::NONE) {
					// 位置・移動量修正
					FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[0].maxPos.x, &pSelfInfo->move.x, pOthColli[0].fWidth);
				}
				break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: break;

		}
	}

	// 体
	{
		switch (pOthColli[1].Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[1].maxPos.y, &pSelfInfo->move.y, pOthColli[1].fHeight);

				// 表の世界のプレイヤー
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {
				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[1].maxPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[1].minPos.y, &pSelfInfo->move.y, pOthColli[1].fHeight);

				// 裏の世界のプレイヤーならジャンプ可能
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {
				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[1].minPos.y;// 最高Ｙ座標設定
				//}
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:
				// 位置・移動量修正
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[1].minPos.x, &pSelfInfo->move.x, pOthColli[1].fWidth);
				break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:
				// 位置・移動量修正
				FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[1].maxPos.x, &pSelfInfo->move.x, pOthColli[1].fWidth);
				break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}

	// 尻
	{
		switch (pOthColli[2].Rot)
		{
			//*********************************
			// 上に当たった
			//*********************************
			case ROT::OVER:
				// 位置・移動量修正
				FixPos_OVER(&pSelfInfo->pos.y, pOthColli[2].maxPos.y, &pSelfInfo->move.y, pOthColli[2].fHeight);

				// 表の世界のプレイヤー
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE) {

				//	if (pSelfInfo->bJump == true)
				//		//s_SE.pSound->Play(//s_SE.dog[0], CSound::CATEGORY::SE, false);

				//	pSelfInfo->bGround = true;	// 地面に接している
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[2].maxPos.y;// 最高Ｙ座標設定
				//}

				//pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
				//pSelfInfo->bExtendDog = true;
				break;

				//*********************************
				// 下に当たった
				//*********************************
			case ROT::UNDER:
				// 位置・移動量修正
				FixPos_UNDER(&pSelfInfo->pos.y, pOthColli[2].minPos.y, &pSelfInfo->move.y, pOthColli[2].fHeight);

				// 裏の世界のプレイヤーならジャンプ可能
				//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND) {

				//	if (pSelfInfo->bJump == true)
				//		//s_SE.pSound->Play(//s_SE.dog[0], CSound::CATEGORY::SE, false);

				//	pSelfInfo->bGround = true;
				//	pSelfInfo->bJump = false;	// ジャンプ可能
				//	pSelfInfo->fMaxHeight = pOthColli[2].minPos.y;// 最高Ｙ座標設定
				//}

				//pExtenddog->SetState(CExtenddog::STATE::DOWN_LAND);
				//pSelfInfo->bExtendDog = true;
				break;

				//*********************************
				// 左に当たった
				//*********************************
			case ROT::LEFT:
				// 位置・移動量修正
				FixPos_LEFT(&pSelfInfo->pos.x, pOthColli[2].minPos.x, &pSelfInfo->move.x, pOthColli[2].fWidth);
				break;

				//*********************************
				// 右に当たった
				//*********************************
			case ROT::RIGHT:
				// 位置・移動量修正
				FixPos_RIGHT(&pSelfInfo->pos.x, pOthColli[2].maxPos.x, &pSelfInfo->move.x, pOthColli[2].fWidth);
				break;

				//*********************************
				// 埋まった
				//*********************************
			case ROT::UNKNOWN: break; //Death(&pSelfInfo->pos);
		}
	}
}

//----------------------------
// ゴールゲートの当たり判定処理
// Author:KEISUKE OTONO
//----------------------------
void CCollision::GoalGate(SelfInfo *pSelfInfo, ColliInfo *pColli)
{
	//if (!pSelfInfo->bGoal)
	//{
	//	pSelfInfo->bGoal = true;
	//
	//	const int ParTex = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Hit_002.png");
	//
	//	for (int ParCnt = 0; ParCnt < 8; ParCnt++)
	//	{
	//		Manager::EffectMgr()->ParticleCreate(ParTex, pSelfInfo->pos, INIT_EFFECT_SCALE * 0.5f, Color{ 245,255,0,255 });
	//	}
	//}
}

//----------------------------
// パーツの当たり判定処理
//----------------------------
void CCollision::Parts(SelfInfo *pSelfInfo, CParts *pParts)
{
	//if (!pParts->GetDisp()) return;

	//// 取得したので描画OFF
	//pParts->DispSwitch(false);

	//// 取得数増加
	//CPlayer::s_nNumGetParts++;

	//// 取得した数が全体数と同じなら、ロケット乗車可能
	//if (CPlayer::s_nNumGetParts == CParts::GetNumAll()) CPlayer::s_bRideRocket = true;
}

//----------------------------
// ロケットの当たり判定処理
//----------------------------
void CCollision::Rocket(SelfInfo *pSelfInfo, CRocket *pRocket)
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
void CCollision::Pile(SelfInfo *pSelfInfo, ColliInfo *pColli, CPile *pPile)
{
	// 当たった方向ごとに処理を切り替え
	switch (pColli->Rot)
	{
		//*********************************
		// 上に当たった
		//*********************************
		case ROT::OVER:
			// 表の世界のプレイヤー
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::FACE)
			//{
			//	if (pSelfInfo->bJump == true)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}

			//	//ある程度の高さから落下してきた
			//	if (pSelfInfo->fMaxHeight - pColli->maxPos.y >= CPile::CAVEIN_DIFF_HEIGHT)
			//	{
			//		pPile->CaveInTrunkHeight(pColli->maxPos.y - pSelfInfo->pos.y - CPlayer::SIZE_HEIGHT);
			//	}

			//	pSelfInfo->move.y = 0.0f;
			//	pSelfInfo->bLandPile = true;// 乗った
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->maxPos.y;// 最高Ｙ座標設定
			//}
			break;

			//*********************************
			// 下に当たった
			//*********************************
		case ROT::UNDER:
			// 裏の世界のプレイヤー
			//if (pSelfInfo->side == CPlayer::WORLD_SIDE::BEHIND)
			//{
			//	if (pSelfInfo->bJump == true)
			//	{// 着地した
			//	 // SE再生
			//		//s_SE.pSound->Play(//s_SE.landing, CSound::CATEGORY::SE, false);
			//	}

			//	//ある程度の高さから落下してきた
			//	if (pSelfInfo->fMaxHeight - pColli->minPos.y <= -CPile::CAVEIN_DIFF_HEIGHT)
			//	{
			//		pPile->CaveInTrunkHeight(pColli->minPos.y - pSelfInfo->pos.y + CPlayer::SIZE_HEIGHT);
			//	}

			//	pSelfInfo->move.y = 0.0f;
			//	pSelfInfo->bLandPile = true;// 乗った
			//	pSelfInfo->bGround = true;	// 地面に接している
			//	pSelfInfo->bJump = false;	// ジャンプ可能
			//	pSelfInfo->fMaxHeight = pColli->minPos.y;// 最高Ｙ座標設定
			//}
			break;

			//*********************************
			// 左に当たった
			//*********************************
		case ROT::LEFT:
			// 位置・移動量修正
			FixPos_LEFT(&pSelfInfo->pos.x, pColli->minPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 右に当たった
			//*********************************
		case ROT::RIGHT:
			// 位置・移動量修正
			FixPos_RIGHT(&pSelfInfo->pos.x, pColli->maxPos.x, &pSelfInfo->move.x, pColli->fWidth);
			break;

			//*********************************
			// 埋まった
			//*********************************
		case ROT::UNKNOWN: break;// if (s_nSwapInterval != 0)	Death(&pSelfInfo->pos);
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
CCollision::ROT CCollision::IsBoxCollider(SelfInfo& self, ColliInfo& target, VEC value)
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
		switch (value)
		{
			case VEC::X:
				// 前回は左からめり込んでいない（今はめり込んだ
				if (OLD_MAXPOS.x <= TARGET_MinPosOld.x)			return ROT::LEFT;

				// 前回は右からめり込んでいない（今はめり込んだ
				else if (OLD_MINPOS.x >= TARGET_MaxPosOld.x)	return ROT::RIGHT;

				//対象が左に動いている
				else if (target.posOld.x > target.pos.x) return ROT::RIGHT;

				//対象が右に動いている
				else if (target.posOld.x < target.pos.x) return ROT::LEFT;
				break;

			case VEC::Y:
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