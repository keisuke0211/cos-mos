//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

//=======================================
// コンストラクタ
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);
}

//=======================================
// デストラクタ
//=======================================
CEffect_Death::~CEffect_Death()
{
	Manager::EffectMgr()->SubList(this);
}

//=======================================
// 設定処理
//=======================================
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int nLife, const int nTex, const TYPE type)
{
	m_Info.pos = pos;   m_Info.posOld = posOld; m_Info.move = move;
	m_Info.rot = rot;   m_Info.spin   = spin;
	m_Info.size = size; m_Info.color  = color;
	m_Info.nTex = nTex; m_Info.type   = type;
	m_Info.nLife = nLife;

	if (type == TYPE::BALL)
	{
		m_Info.move.x = sinf(rot.z) * -2.0f;
		m_Info.move.y = cosf(rot.z) * -2.0f;
	}
}

//=======================================
//更新処理
//=======================================
void CEffect_Death::Update(void)
{
	//種類別更新処理
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); break;
		case TYPE::INK: UpdateType_Ink();  break;
	}
	
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, INITVECTOR3D)
		->SetTex(m_Info.nTex)
		->SetBillboard(true)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	//［寿命］共通更新処理
	Life();
}

//=======================================
//【ボール】更新処理
//=======================================
void CEffect_Death::UpdateType_Ball(void)
{
	//自己情報設定
	CCollision::SelfInfo SelfInfo;

	//当たり判定の情報を格納
	CCollision::ColliInfo colliInfo;
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//プレイヤーの当たり判定
	ColliRot = PlayerCollider(&SelfInfo, &colliInfo);
	if (ColliRot != CCollision::ROT::NONE)
	{
		//プレイヤーまでの角度を取得
		const float fRot = atan2f(powf(SelfInfo.pos.x + colliInfo.pos.x, 2.0f),
								  powf(SelfInfo.pos.y + colliInfo.pos.y, 2.0f));

		//移動量設定
		m_Info.move.x = sinf(fRot) * -3.0f;
		m_Info.move.y = cosf(fRot) * -3.0f;
	}

	//ステージオブジェクトとの当たり判定
	ColliRot = StgObjCollider(&SelfInfo, &colliInfo);
	if (ColliRot != CCollision::ROT::NONE)
	{
		switch (ColliRot)
		{
			case CCollision::ROT::OVER:
			case CCollision::ROT::UNDER: m_Info.move.y *= -0.7f;break;
			case CCollision::ROT::LEFT:
			case CCollision::ROT::RIGHT: m_Info.move.x *= -0.7f;break;
		}
	}

	//移動・回転処理
	Move();
	Spin();
}

//=======================================
//【インク】更新処理
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{
	
}

//=======================================
//［プレイヤー当たり判定］情報更新処理
//=======================================
CCollision::ROT CEffect_Death::PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo)
{
	//自分の情報を反映
	pSelfInfo->pos    = m_Info.pos;    pSelfInfo->posOld  = m_Info.posOld; pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = m_Info.size.x; pSelfInfo->fHeight = m_Info.size.y;

	//当たった方向を格納
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//プレイヤー取得
	CPlayer *pPlayer = CMode_Game::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
	{
		//プレイヤー情報反映
		CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);
		pColliInfo->pos    = pInfo->pos;          pColliInfo->posOld  = pInfo->posOld;
		pColliInfo->fWidth = CPlayer::SIZE_WIDTH; pColliInfo->fHeight = CPlayer::SIZE_HEIGHT;

		//位置の差分やサイズの対角線を取得
		const float fposDiffLength = D3DXVec3Length(&(pColliInfo->pos - pSelfInfo->pos));
		const float fSizeLength = D3DXVec2Length(&D3DXVECTOR2(pSelfInfo->fWidth + pColliInfo->fWidth, pSelfInfo->fHeight + pColliInfo->fHeight));

		//サイズの対角線より距離が離れている
		if (fposDiffLength > fSizeLength)
			continue;

		for (int nCntRot = 0; nCntRot < (int)CPlayer::VECTOL::MAX; nCntRot++)
		{
			//当たり判定
			ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, (CPlayer::VECTOL)nCntRot);

			//当たったら終了
			if (ColliRot != CCollision::ROT::NONE)break;
		}
	}

	//結果を返す
	return ColliRot;
}

//=======================================
//［ステージオブジェクト当たり判定］情報更新処理
//=======================================
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo)
{
	//自分の情報を反映
	pSelfInfo->pos    = m_Info.pos;    pSelfInfo->posOld  = m_Info.posOld; pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = m_Info.size.x; pSelfInfo->fHeight = m_Info.size.y;

	//当たった方向を格納
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) 
	{
		// 取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		//ブロック以外は判定しない
		if (stageObj->GetType() != CStageObject::TYPE::BLOCK) continue;

		//情報反映
		pColliInfo->pos    = stageObj->GetPos();   pColliInfo->posOld = stageObj->GetPosOld();
		pColliInfo->fWidth = stageObj->GetWidth(); pColliInfo->fHeight = stageObj->GetHeight();

		for (int nCntRot = 0; nCntRot < (int)CPlayer::VECTOL::MAX; nCntRot++)
		{
			//当たり判定
			ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, (CPlayer::VECTOL)nCntRot);

			//当たったら終了
			if (ColliRot != CCollision::ROT::NONE)break;
		}
	}

	//結果を返す
	return ColliRot;
}

//=======================================
//［移動］情報更新処理
//=======================================
void CEffect_Death::Move(void)
{
	//移動する
	m_Info.pos += m_Info.move;

	//横の移動量を減衰させる
	m_Info.move.x += (0.0f - m_Info.move.x) * 0.01f;
	
	//表の世界にいる
	if (m_Info.pos.y > 0.0f) 
		 m_Info.move.y -= 0.03f;
	//裏の世界にいる
	else m_Info.move.y += 0.03f;
}

//=======================================
//［回転］情報更新処理
//=======================================
void CEffect_Death::Spin(void)
{
	m_Info.rot += m_Info.spin;
}

//=======================================
//［寿命］情報更新処理
//=======================================
void CEffect_Death::Life(void)
{
	//寿命が存在し、尽きた
	if (m_Info.nLife > 0 &&	--m_Info.nLife <= 0)
		CObject::Delete();
}