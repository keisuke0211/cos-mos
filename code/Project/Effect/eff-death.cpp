//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"

const float CEffect_Death::CREATE_SPREAD_POWER = -8.0f; //生成時の拡散力
const float CEffect_Death::PLAYER_COLLI_POWER = 1.0f;   //プレイヤーに当たったときの吹っ飛び力
const float CEffect_Death::MOVE_X_CORRECT = 0.01f;      //Ⅹベクトルの移動補正係数
const float CEffect_Death::GRAVITY_POWER = 0.03f;       //重力加速度
const float CEffect_Death::BOUND_POWER = -0.7f;         //バウンド係数
const short CEffect_Death::BALL_ALPHA_DECREASE = 10;    //ボールのα値減少量（当たり判定でUnknownが出た際に使用

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
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const Vector2D size, const Color color, const int nLife, const int nIdx, const TYPE type)
{
	//基本情報設定
	m_Info.pos = pos;   m_Info.posOld = posOld; m_Info.move = move;
	m_Info.rot = rot;   m_Info.spin   = spin;
	m_Info.size = size; m_Info.color  = color;
	m_Info.nIdx = nIdx; m_Info.type   = type;
	m_Info.nLife = nLife;

	//生成してすぐプレイヤーに当たらないようにカウンター代入
	m_Info.CreateCounter = CREATE_COUNTER;
	m_Info.bDeath = false;

	if (type == TYPE::BALL)
	{//ボールの場合、拡散力を設定
		m_Info.move.x = sinf(rot.z) * CREATE_SPREAD_POWER;
		m_Info.move.y = cosf(rot.z) * CREATE_SPREAD_POWER;
	}
}

//=======================================
//更新処理
//=======================================
void CEffect_Death::Update(void)
{
	//前回位置更新
	m_Info.posOld = m_Info.pos;

	//種類別更新・配置処理
	switch (m_Info.type)
	{
		case TYPE::BALL:UpdateType_Ball(); PutModel();   break;//ボール
		case TYPE::INK: UpdateType_Ink();  PutPolygon(); break;//インク
	}

	//［寿命］共通更新処理
	Life();

	//［死亡］共通更新処理
	Death();
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
				const float fRot = atan2f(-PosDiff.x, -PosDiff.y);

				const Pos3D PlayerMove = colliInfo.pos - colliInfo.posOld;

				//移動量設定
				switch (vec)
				{
					case CPlayer::VECTOL::X:m_Info.move.x = sinf(fRot) * PLAYER_COLLI_POWER + PlayerMove.x;break;
					case CPlayer::VECTOL::Y:m_Info.move.y = cosf(fRot) * PLAYER_COLLI_POWER + PlayerMove.y * 0.7f;break;
				}
			}
		}

		//ステージオブジェクトとの当たり判定
		CStageObject::TYPE type = CStageObject::TYPE::NONE;
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec, type);
		if (ColliRot != CCollision::ROT::NONE)
		{
			switch (type)
			{
					//ブロックの判定
				case CStageObject::TYPE::BLOCK:
					//当たった方向による処理
					switch (ColliRot)
					{
						//左右に当たった場合
						case CCollision::ROT::LEFT:
						case CCollision::ROT::RIGHT: m_Info.move.x *= BOUND_POWER; break;

						//上下に当たった場合
						//落下速度がほぼ０なら０に設定
						//違うなら速度を弱めつつ、バウンド
						case CCollision::ROT::OVER:
						case CCollision::ROT::UNDER: m_Info.move.y *= BOUND_POWER;break;

						//当たった方向が分からない
						case CCollision::ROT::UNKNOWN:
						{//α値を減少させ、０以下で死亡
							if (m_Info.color.a <= BALL_ALPHA_DECREASE)
								m_Info.bDeath = true;
							else m_Info.color.a -= BALL_ALPHA_DECREASE;
						}
					}break;

					//穴埋めブロックの判定
					//死亡
				case CStageObject::TYPE::FILLBLOCK: m_Info.bDeath = true;break;
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

		//プレイヤー情報反映
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
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type)
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

		type = stageObj->GetType();

		//ブロック以外は判定しない
		if (type != CStageObject::TYPE::BLOCK &&
			type != CStageObject::TYPE::FILLBLOCK) continue;

		//情報反映
		pColliInfo->pos    = stageObj->GetPos();   pColliInfo->posOld = stageObj->GetPosOld();
		pColliInfo->fWidth = stageObj->GetWidth() * 0.5f;
		pColliInfo->fHeight = stageObj->GetHeight() * 0.5f;

		//当たり判定
		ColliRot = CCollision::IsBoxCollider(*pSelfInfo, *pColliInfo, vec);

		//当たったら終了
		if (ColliRot != CCollision::ROT::NONE)
			break;
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
			m_Info.move.x += (0.0f - m_Info.move.x) * MOVE_X_CORRECT;
			break;

			//Ｙベクトル移動
		case CPlayer::VECTOL::Y:
			m_Info.pos.y += m_Info.move.y;

			//表の世界にいる
			if (m_Info.pos.y > 0.0f)
				m_Info.move.y -= GRAVITY_POWER;
			//裏の世界にいる
			else m_Info.move.y += GRAVITY_POWER;
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
	if (m_Info.nLife > 0 && --m_Info.nLife <= 0)
		m_Info.bDeath = true;

	if(RNLib::Input().GetKeyTrigger(DIK_O))
		m_Info.bDeath = true;
}

//=======================================
//ポリゴン配置処理
//=======================================
void CEffect_Death::PutPolygon(void)
{
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_Info.pos, m_Info.rot)
		->SetTex(m_Info.nIdx)
		->SetCol(m_Info.color)
		->SetSize(m_Info.size.x, m_Info.size.y)
		->SetZTest(false);
}

//=======================================
//モデル配置処理
//=======================================
void CEffect_Death::PutModel(void)
{
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nIdx, m_Info.pos, m_Info.rot, Vector3D(m_Info.size.x, m_Info.size.y, (m_Info.size.x + m_Info.size.y) * 0.5f))
		->SetZTest(false)
		->SetCol(m_Info.color);
}

//=======================================
//［死亡］情報更新処理
//=======================================
void CEffect_Death::Death(void)
{
	//死亡フラグが立っている
	if(m_Info.bDeath)
		CObject::Delete();
}

//=======================================
//自身の当たり判定情報設定
//=======================================
void CEffect_Death::SetSelfInfo(CCollision::SelfInfo *pSelfInfo)
{
	//自分の情報を反映
	pSelfInfo->pos = m_Info.pos; pSelfInfo->posOld = m_Info.posOld;
	pSelfInfo->move = m_Info.move;
	pSelfInfo->fWidth = CStageObject::SIZE_OF_1_SQUARE * m_Info.size.x * 0.5f;
	pSelfInfo->fHeight = CStageObject::SIZE_OF_1_SQUARE * m_Info.size.y * 0.5f;
}