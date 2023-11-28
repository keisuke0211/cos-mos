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

	m_Info.CreateCounter = CREATE_COUNTER;

	if (type == TYPE::BALL)
	{
		m_Info.move.x = sinf(rot.z) * -8.0f;
		m_Info.move.y = cosf(rot.z) * -8.0f;
	}
}

//=======================================
//更新処理
//=======================================
void CEffect_Death::Update(void)
{
	//前回位置更新
	m_Info.posOld = m_Info.pos;

	//種類別更新処理
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); break;
		case TYPE::INK: UpdateType_Ink();  break;
	}
	
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, INITVECTOR3D)
		->SetBillboard(true)
		->SetCol(INITCOLOR)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);

	Pos3D rot = m_Info.rot;
	rot.x = rot.z;
	rot.y = rot.z;
	rot.z = 0.0f;

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nTex)
		//->SetBillboard(true)
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
	//生成されてからのカウンター減少
	if (m_Info.CreateCounter != 0)
		m_Info.CreateCounter--;

	//自己情報設定
	CCollision::SelfInfo SelfInfo;

	//当たり判定の情報を格納
	CCollision::ColliInfo colliInfo;
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
	{
		const CPlayer::VECTOL vec = (CPlayer::VECTOL)nCntVec;

		//移動処理
		Move(vec);

		//プレイヤーの当たり判定
		if (m_Info.CreateCounter == 0)
		{
			ColliRot = PlayerCollider(&SelfInfo, &colliInfo, vec);
			if (ColliRot != CCollision::ROT::NONE)
			{
				const Pos3D PosDiff = colliInfo.pos - SelfInfo.pos;

				//プレイヤーまでの角度を取得
				const float fRot = atan2f(PosDiff.x, -PosDiff.y);

				//移動量設定
				m_Info.move.x = sinf(fRot) * 3.0f;
				m_Info.move.y = cosf(fRot) * 3.0f;
			}
		}

		//ステージオブジェクトとの当たり判定
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec);
		if (ColliRot != CCollision::ROT::NONE)
		{
			switch (ColliRot)
			{
				case CCollision::ROT::OVER:
				case CCollision::ROT::UNDER: m_Info.move.y *= -0.7f; break;
				case CCollision::ROT::LEFT:
				case CCollision::ROT::RIGHT: m_Info.move.x *= -0.7f; break;
			}
		}
	}

	//移動ベクトルの角度を算出
	const float fRot = atan2f(m_Info.move.x, -m_Info.move.y);
	m_Info.rot.z = fRot;

	//回転処理
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
CCollision::ROT CEffect_Death::PlayerCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec)
{
	//自分の情報を反映
	SetSelfInfo(pSelfInfo);

	//当たった方向を格納
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//プレイヤー取得
	CPlayer *pPlayer = CMode_Game::GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
	{
		//プレイヤー情報反映
		CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);
		//ゴールしている or 死んでいる
		if (pInfo->bGoal || pInfo->bRide || pInfo->deathCounter != 0 || pInfo->deathCounter2 != 0)continue;
		pColliInfo->pos = pInfo->pos;             pColliInfo->posOld  = pInfo->posOld;
		pColliInfo->fWidth = CPlayer::SIZE_WIDTH; pColliInfo->fHeight = CPlayer::SIZE_HEIGHT;

		//位置の差分やサイズの対角線を取得
		const float fposDiffLength = D3DXVec3Length(&(pColliInfo->pos - pSelfInfo->pos));
		const float fSizeLength = D3DXVec2Length(&D3DXVECTOR2(pSelfInfo->fWidth + pColliInfo->fWidth, pSelfInfo->fHeight + pColliInfo->fHeight));

		//サイズの対角線より距離が離れている
		if (fposDiffLength > fSizeLength)
			continue;

		//当たり判定
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//当たったら終了
		if (ColliRot != CCollision::ROT::NONE)break;
	}

	//結果を返す
	return ColliRot;
}

//=======================================
//［ステージオブジェクト当たり判定］情報更新処理
//=======================================
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec)
{
	//自分の情報を反映
	SetSelfInfo(pSelfInfo);

	//当たった方向を格納
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	CObject* obj = NULL;
	while (Manager::StageObjectMgr()->ListLoop(&obj)) 
	{
		// 取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		CStageObject::TYPE type = stageObj->GetType();

		//ブロック以外は判定しない
		if (stageObj->GetType() != CStageObject::TYPE::BLOCK) continue;

		//情報反映
		pColliInfo->pos    = stageObj->GetPos();   pColliInfo->posOld = stageObj->GetPosOld();
		pColliInfo->fWidth = stageObj->GetWidth() * 0.5f;
		pColliInfo->fHeight = stageObj->GetHeight() * 0.5f;

		//当たり判定
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//当たったら終了
		if (ColliRot != CCollision::ROT::NONE)break;
	}

	//結果を返す
	return ColliRot;
}

//=======================================
//［移動］情報更新処理
//=======================================
void CEffect_Death::Move(CPlayer::VECTOL vec)
{
	//移動
	switch (vec)
	{
			//Ⅹベクトル移動
		case CPlayer::VECTOL::X:
			m_Info.pos.x += m_Info.move.x;

			//移動量を減衰させる
			m_Info.move.x += (0.0f - m_Info.move.x) * 0.01f;
			break;

			//Ｙベクトル移動
		case CPlayer::VECTOL::Y:
			m_Info.pos.y += m_Info.move.y;

			//表の世界にいる
			if (m_Info.pos.y > 0.0f)
				m_Info.move.y -= 0.03f;
			//裏の世界にいる
			else m_Info.move.y += 0.03f;
			break;
	}
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

	if(RNLib::Input().GetKeyTrigger(DIK_O))
		CObject::Delete();
}

//=======================================
//自身の当たり判定情報設定
//=======================================
void CEffect_Death::SetSelfInfo(CCollision::SelfInfo *pSelfInfo)
{
	//自分の情報を反映
	pSelfInfo->pos = m_Info.pos;    pSelfInfo->posOld = m_Info.posOld; pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = m_Info.size.x * 0.5f;
	pSelfInfo->fHeight = m_Info.size.y * 0.5f;
}