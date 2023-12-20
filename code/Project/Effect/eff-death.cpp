//========================================================
// 
// 死亡パーティクル処理
// Author:HIRASAWA SHION
// 
//========================================================
#include "eff-death.h"
#include "../main.h"
#include "../stage.h"

CFloat CEffect_Death::CREATE_SPREAD_POWER = -4.0f; //生成時の拡散力
CFloat CEffect_Death::PLAYER_COLLI_POWER = 1.0f;   //プレイヤーに当たったときの吹っ飛び力
CFloat CEffect_Death::PLAYER_KICK_POWER = 2.0f;   //プレイヤーのキック量
CFloat CEffect_Death::MOVE_X_CORRECT = 0.03f;      //Ⅹベクトルの移動補正係数
CFloat CEffect_Death::GRAVITY_POWER = 0.03f;       //重力加速度
CFloat CEffect_Death::BOUND_POWER = -0.9f;         //バウンド係数
const short CEffect_Death::BALL_ALPHA_DECREASE = 10;    //ボールのα値減少量（当たり判定でUnknownが出た際に使用
CFloat CEffect_Death::BALL_SIZE[(int)BALL_SIZE_LV::MAX] = { //ボールサイズを格納
	0.3f, 0.6f, 1.0f
};

int         CEffect_Death::s_nNumAllBall = 0;
int         CEffect_Death::s_FusionSE = NONEDATA;
int         CEffect_Death::s_BallModelID[(int)BALL_SIZE_LV::MAX] = { NONEDATA, NONEDATA ,NONEDATA }; //ボールモデル番号を格納
const char *CEffect_Death::BALL_MODEL_PATH[(int)BALL_SIZE_LV::MAX] = { //ボールモデルパスを格納
	"data\\MODEL\\Effect\\Ball_S.x",
	"data\\MODEL\\Effect\\Ball_M.x",
	"data\\MODEL\\Effect\\Ball_L.x"
};

//=======================================
// コンストラクタ
//=======================================
CEffect_Death::CEffect_Death()
{
	Manager::EffectMgr()->AddList(this);

	m_type = CStageObject::TYPE::DEATH_EFF;
}

//=======================================
// デストラクタ
//=======================================
CEffect_Death::~CEffect_Death()
{
	if (m_Info.type == TYPE::BALL) {
		//ボールサイズ用レベル開放
		if (m_Info.pLv != NULL) {
			delete m_Info.pLv;
			m_Info.pLv = NULL;
		}

		s_nNumAllBall--;
	}

	Manager::EffectMgr()->SubList(this);
}

//=======================================
// 設定処理
//=======================================
void CEffect_Death::SetInfo(const Vector3D pos, const Vector3D posOld, const Vector3D move, const Vector3D rot, const Vector3D spin, const float size, const Color color, const int nLife, const TYPE type)
{
	//基本情報設定
	m_pos = pos;        m_posOld    = posOld; m_Info.move = move;
	m_rot = rot;        m_Info.spin = spin;
	m_Info.size = size; m_color     = color;
	m_Info.type = type;
	m_Info.nLife = nLife;

	//生成してすぐプレイヤーに当たらないようにカウンター代入
	m_Info.ColliderInterval = CREATE_INTERVAL;
	m_Info.bDeath = false;

	switch (type)
	{
		//ボールの場合、拡散力を設定
		case TYPE::BALL:
		{
			m_Info.move.x = sinf(rot.z) * CREATE_SPREAD_POWER;
			m_Info.move.y = cosf(rot.z) * CREATE_SPREAD_POWER;
			m_Info.nBallID = s_nNumAllBall++;
			if (s_FusionSE == NONEDATA)
				s_FusionSE = RNLib::Sound().Load("data\\SOUND\\SE\\fusion.wav");

			for (int nCntID = 0; nCntID < (int)BALL_SIZE_LV::MAX; nCntID++)
			{
				if (s_BallModelID[nCntID] == NONEDATA)
					s_BallModelID[nCntID] = RNLib::Model().Load(BALL_MODEL_PATH[nCntID]);
			}
		}break;

		//インクの場合
		case TYPE::INK:
		{
			m_Info.nLife = MAX_INK_LIFE;        // ライフ設定
			m_Info.InkSize = Pos2D(size, size); // サイズ設定
			if (m_Info.nIdx == NONEDATA)
				m_Info.nIdx = RNLib::Texture().Load("data\\TEXTURE\\Effect\\ink001.png");
		}
	}

}

//=======================================
//更新処理
//=======================================
void CEffect_Death::Update(void)
{
	//前回位置更新
	m_posOld = m_pos;

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
	//サイズ指定されていなかったら死亡フラグを立てて終了
	if (m_Info.pLv == NULL) {
		m_Info.bDeath = true;
		m_Info.size = 0.0f;
		return;
	}

	//生成されてからのカウンター減少
	if (m_Info.ColliderInterval != 0)
		m_Info.ColliderInterval--;

	//自己情報設定
	CCollision::SelfInfo SelfInfo;

	//当たり判定の情報を格納
	CCollision::ColliInfo colliInfo;
	CCollision::ROT ColliRot = CCollision::ROT::NONE;

	//プレイヤーの当たり判定
	if (m_Info.ColliderInterval == 0)
	{
		//プレイヤー取得
		CPlayer *pPlayer = Stage::GetPlayer();

		for (int nCntPlayer = 0; nCntPlayer < CPlayer::NUM_PLAYER; nCntPlayer++)
		{
			//プレイヤー情報反映
			CPlayer::Info *pInfo = pPlayer->GetInfo(nCntPlayer);

			//ゴールしている or 死んでいる
			if (pInfo->bGoal || pInfo->bRide || pInfo->deathCounter != 0 || pInfo->deathCounter2 != 0)continue;

			//当たり判定情報設定
			SetSelfInfo(&SelfInfo);
			colliInfo.pos = pInfo->pos;             colliInfo.posOld = pInfo->posOld;
			colliInfo.fWidth = CPlayer::SIZE_WIDTH; colliInfo.fHeight = CPlayer::SIZE_HEIGHT;

			//当たった方向を格納
			float fAngle = 0.0f;
			if (!CCollision::CircleToBoxCollider(SelfInfo, colliInfo, &fAngle)) continue;

			//位置修正
			float wh = D3DXVec2Length(&Pos2D(CPlayer::SIZE_WIDTH, CPlayer::SIZE_HEIGHT));
			CFloat SumSize = SelfInfo.fRadius * 0.5f + D3DXVec2Length(&Pos2D(CPlayer::SIZE_WIDTH, CPlayer::SIZE_HEIGHT));

			//移動ベクトル
			CFloat VecMove = D3DXVec3Length(&m_Info.move);
			float VecMovePower = VecMove * PLAYER_COLLI_POWER;
			m_Info.move.x = sinf(fAngle) * VecMovePower;
			m_Info.move.y = cosf(fAngle) * VecMovePower * 0.9f;

			//プレイヤーが地面にいる時蹴飛ばす
			if (pInfo->bGround &&
				(pInfo->side == CPlayer::WORLD_SIDE::FACE && m_Info.move.y <= 0.0f) ||
				(pInfo->side == CPlayer::WORLD_SIDE::BEHIND && m_Info.move.y >= 0.0f))
			{
				m_Info.move.x *= PLAYER_KICK_POWER;
				m_Info.move.y *= -PLAYER_KICK_POWER;
				m_Info.ColliderInterval = KICK_INTERVAL;
			}
			break;
		}
	}

	//当たり判定情報設定
	SetSelfInfo(&SelfInfo);

	for (int nCntVec = 0; nCntVec < (int)CPlayer::VECTOL::MAX; nCntVec++)
	{
		const CPlayer::VECTOL vec = (CPlayer::VECTOL)nCntVec;

		//移動処理
		Move(vec);

		//ステージオブジェクトとの当たり判定
		CStageObject::TYPE type = CStageObject::TYPE::NONE;
		ColliRot = StgObjCollider(&SelfInfo, &colliInfo, vec, type);
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
					case CCollision::ROT::UNDER: m_Info.move.y *= BOUND_POWER; break;

						//当たった方向が分からない
					case CCollision::ROT::UNKNOWN:
					{//α値を減少させ、０以下で死亡
						if (m_color.a <= BALL_ALPHA_DECREASE)
							m_Info.bDeath = true;
						else m_color.a -= BALL_ALPHA_DECREASE;
					}break;
				}break;

				//穴埋めブロックの判定
				//死亡
			case CStageObject::TYPE::FILLBLOCK: m_Info.bDeath = true; break;
		}
	}

	//ボール合体処理
	BallFusion();

	//移動ベクトルの角度を算出
	//CFloat fRot = atan2f(m_Info.move.x, -m_Info.move.y);
	//m_rot.z = fRot;
}

//=======================================
//【インク】更新処理
//=======================================
void CEffect_Death::UpdateType_Ink(void)
{
	CFloat Rate = RNLib::Ease().Easing(_RNC_Ease::TYPE::IN_SINE, m_Info.nLife, MAX_INK_LIFE);
	m_color.a = 255 * Rate;

	//インクが垂れるようにサイズを拡大
	m_Info.InkSize.y = m_Info.size + m_Info.size * 0.5f * (1.0f - Rate);

	//重力方向に緩やかに移動
	m_pos.y += m_pos.y >= 0.0f ?
		-0.01f :
		0.01f;

	RNLib::Text2D().PutDebugLog(String("ライフ:%d  透明度:%d  高さ:%.2f  割合:%.2f", m_Info.nLife, m_color.a, m_Info.InkSize.y, Rate));

	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, INITROT3D)
		->SetSize(m_Info.InkSize)
		->SetCol(m_color)
		->SetZTest(false)
		->SetTex(m_Info.nIdx);

	//透明度が０なら死亡
	if (m_color.a <= 0) m_Info.bDeath = true;
}

//=======================================
//［ステージオブジェクト当たり判定］情報更新処理
//=======================================
CCollision::ROT CEffect_Death::StgObjCollider(CCollision::SelfInfo *pSelfInfo, CCollision::ColliInfo *pColliInfo, CPlayer::VECTOL vec, CStageObject::TYPE& type)
{
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

		//当たった方向を格納
		if (!CCollision::CircleToBoxCollider(*pSelfInfo, *pColliInfo, NULL)) continue;
		
		ColliRot = CCollision::IsBoxToBoxCollider(*pSelfInfo, *pColliInfo, vec);
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
			m_pos.x += m_Info.move.x;

			//移動量を減衰させる
			m_Info.move.x += (0.0f - m_Info.move.x) * MOVE_X_CORRECT;

			if (fabsf(m_Info.move.x) <= 0.01f)
				m_Info.move.x = 0.0f;
			break;

			//Ｙベクトル移動
		case CPlayer::VECTOL::Y:
			m_pos.y += m_Info.move.y;

			//表の世界にいる
			if (m_pos.y > 0.0f)
				m_Info.move.y -= GRAVITY_POWER;
			//裏の世界にいる
			else m_Info.move.y += GRAVITY_POWER;

			if (fabsf(m_Info.move.y) <= 0.01f)
				m_Info.move.y = 0.0f;
			break;
	}
}

//=======================================
//［回転］情報更新処理
//=======================================
void CEffect_Death::Spin(void)
{
	m_rot += m_Info.spin;
}

//=======================================
//［寿命］情報更新処理
//=======================================
void CEffect_Death::Life(void)
{
	//寿命が存在し、尽きた
	if (m_Info.nLife > 0 && --m_Info.nLife <= 0)
		m_Info.bDeath = true;

#ifndef DEBUG
	if (RNLib::Input().GetKeyTrigger(DIK_O))
		m_Info.bDeath = true;
#endif // DEBUG
}

//=======================================
//ポリゴン配置処理
//=======================================
void CEffect_Death::PutPolygon(void)
{
	RNLib::Polygon3D().Put(PRIORITY_EFFECT, m_pos, m_rot)
		->SetTex(m_Info.nIdx)
		->SetCol(m_color)
		->SetSize(m_Info.size, m_Info.size)
		->SetZTest(false);
}

//=======================================
//モデル配置処理
//=======================================
void CEffect_Death::PutModel(void)
{
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nIdx, m_pos, m_rot)
		->SetCol(m_color);
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
	pSelfInfo->pos = m_pos; 
	pSelfInfo->posOld = m_posOld;
	pSelfInfo->move = m_Info.move;
	pSelfInfo->fRadius = pSelfInfo->fWidth = pSelfInfo->fHeight = CStageObject::SIZE_OF_1_SQUARE * m_Info.size * 0.5f;
}

//******************************
//ボール用設定処理
//******************************
void CEffect_Death::SetBallSize(BALL_SIZE_LV Lv)
{
	//サイズがまだ指定されていない
	if (m_Info.pLv == NULL)
		m_Info.pLv = new BALL_SIZE_LV;

	//サイズ設定
	*m_Info.pLv = Lv;
	m_Info.size = BALL_SIZE[(int)Lv];
	m_Info.nIdx = s_BallModelID[(int)Lv];
}

//******************************
//ボール合体処理
//他のボールと当たった場合、自分は消す
//******************************
void CEffect_Death::BallFusion(void)
{
	if (m_Info.ColliderInterval != 0) return;

	CObject* obj = NULL;
	while (Manager::EffectMgr()->ListLoop(&obj))
	{
		// 取得したオブジェクトをキャスト
		CStageObject* stageObj = (CStageObject*)obj;

		//死亡エフェクト以外は判定しない
		const CStageObject::TYPE type = stageObj->GetType();
		if (type != CStageObject::TYPE::DEATH_EFF) continue;

		//死亡エフェクトにキャスト
		//ボール以外は無視
		CEffect_Death *pEff = (CEffect_Death *)stageObj;
		if (pEff->m_Info.type != CEffect_Death::TYPE::BALL || 
			pEff->m_Info.ColliderInterval != 0 ||
			pEff->m_Info.nBallID == m_Info.nBallID) continue;

		//距離の差を取得
		const Pos3D PosDiff = m_pos - pEff->m_pos;
		CFloat PosDiffLen = D3DXVec3Length(&PosDiff);

		//サイズの合計値よりも離れている
		if (PosDiffLen > m_Info.size * CStageObject::SIZE_OF_1_SQUARE) continue;

		//どちらかのサイズレベルが最大
		if (*m_Info.pLv == BALL_SIZE_LV::BIG || *pEff->m_Info.pLv == BALL_SIZE_LV::BIG)
		{
			//両方に死亡フラグを立てる
			m_Info.bDeath = pEff->m_Info.bDeath = true;

			//ジュワ～とするパーティクル
			CInt NUM_PARTI = 16;
			CInt PARTI_TEX = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Circle_005.png");
			const Pos3D PopPos = m_pos + PosDiff * 0.5f;

			for (int nCntParti = 0; nCntParti < NUM_PARTI; nCntParti++)
			{
				//現在地からの拡散量
				const Pos3D Spread(
					(rand() % 2000 - 1000.0f) / 100.0f,
					(rand() % 2000 - 1000.0f) / 100.0f,
					0.0f);

				//エフェクト生成
				Manager::EffectMgr()->ParticleCreate(PARTI_TEX, PopPos + Spread, INITSCALE3D * (rand() % 700) / 100.0f, m_color,
													 CParticle::TYPE::TYPE_FLOATUP, 1000);
			}
		}
		else
		{
			//サイズレベルの合計値を取得
			BALL_SIZE_LV SumSizeLv = (BALL_SIZE_LV)((int)*m_Info.pLv + (int)*pEff->m_Info.pLv);

			//合計値が小・中の場合、一段階上げる
			if(SumSizeLv == BALL_SIZE_LV::SMALL || SumSizeLv == BALL_SIZE_LV::MIDIUM)
				*pEff->m_Info.pLv = (BALL_SIZE_LV)((int)SumSizeLv + 1);

			//それ以外は【大】に設定
			else *pEff->m_Info.pLv = BALL_SIZE_LV::BIG;

			//*******************************
			// 自分は消す
			//*******************************
			m_Info.size = 0.0f;
			m_Info.bDeath = true;

			//*******************************
			//相手は情報を変える
			//*******************************
			pEff->m_Info.size = BALL_SIZE[(int)*pEff->m_Info.pLv];
			pEff->m_posOld = pEff->m_pos;
			pEff->m_pos = m_pos + PosDiff * 0.5f; //2つのボールの中間に位置設定
			pEff->m_pos.y += 5.0f * pEff->m_Info.size;
			pEff->m_Info.move = INITVECTOR3D;
			pEff->m_Info.ColliderInterval = FUSION_INTERVAL;
			pEff->UpdateType_Ball();
			pEff->m_Info.nIdx = s_BallModelID[(int)*pEff->m_Info.pLv];
		}

		//SE再生
		RNLib::Sound().Play(s_FusionSE, _RNC_Sound::CATEGORY::SE, 1.0f, false);
		break;
	}
}