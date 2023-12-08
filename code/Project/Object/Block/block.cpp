//========================================
// 
// ブロック処理
// Author:KEISUKE OTONO
// 
//========================================
#include "block.h"
#include "../../main.h"
#include "../../collision.h"

//========================================
// 定数定義
//========================================
const char* CBlock::MODEL_PATHS[(int)LOOKS_TYPE::MAX] = {
	"data\\MODEL\\StageObject\\SoilBlock.x",						//[0]  土ブロック
	"data\\MODEL\\StageObject\\SoilBlock_Grassy.x",					//[1]  草の生えた土ブロック
	"data\\MODEL\\StageObject\\AncientStoneBlock.x",				//[2]  古代の石ブロック
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternA.x",		//[3]  古代の石ブロックA
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternB.x",		//[4]  古代の石ブロックB
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternC.x",		//[5]  古代の石ブロックC
	"data\\MODEL\\StageObject\\AncientStonePillar_Up.x",			//[6]  柱の上
	"data\\MODEL\\StageObject\\AncientStonePillar_Center.x",		//[7]  柱の繋ぎ
	"data\\MODEL\\StageObject\\AncientStonePillar_Down.x",			//[8]  柱の根本
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf.x",					//[9]  草ブロック
	"data\\MODEL\\StageObject\\TreeTrunk.x",						//[10] 木の幹
	"data\\MODEL\\StageObject\\TreeTrunk_Root.x",					//[11] 木の根元
	"NONEDATA",														//[12] バオバブの木(背景用)
	"NONEDATA",														//[13] 宝箱
	"data\\MODEL\\coin.x",											//[14] 山積みのコイン
	"data\\MODEL\\stone_monument.x",								//[15] 謎の石碑
	"data\\MODEL\\StageObject\\AncientStoneBrickBlock.x",			//[16] 古代の石レンガブロック
	"data\\MODEL\\StageObject\\SoilAndAncientStoneBlock.x",			//[17] 土＆古代の石ブロック(中間用)
	"NONEDATA",														//[18] ヤシの木
	"data\\MODEL\\Three-eyes_block.x",								//[19] 三つ目のブロック
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf_Nuts.x",				//[20] 草ブロック(木の実)
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternA.x",		//[21] ヌイの家ブロック
	"data\\MODEL\\StageObject\\AsphaltBlock.x",						//[22] アスファルトブロック
	"data\\MODEL\\StageObject\\AsphaltBlock_Cracked.x",				//[23] ひび割れたアスファルトブロック
	"data\\MODEL\\StageObject\\IronBar.x",							//[24] 鉄格子(当たり判定無し想定)
	"data\\MODEL\\StageObject\\IronBar1.x",							//[25] 鉄格子1
	"data\\MODEL\\StageObject\\IronBar2.x",							//[26] 鉄格子2
	"data\\MODEL\\StageObject\\IronBar3.x",							//[27] 鉄格子3
	"data\\MODEL\\StageObject\\MudBlock.x",							//[28] 泥ブロック
	"data\\MODEL\\StageObject\\SandAndAsphaltBlock.x",				//[29] 砂＆アスファルトブロック(中間用)
	"data\\MODEL\\StageObject\\SandBlock.x",						//[30] 砂ブロック
	"data\\MODEL\\Torch.x",											//[31] 松明
	"data\\MODEL\\Snake.x",											//[32] 蛇の壁画
	"data\\MODEL\\leaf_large.x",									//[33] 緑色の葉っぱ(大)
	"data\\MODEL\\leaf_small.x",									//[34] 緑色の葉っぱ(小)
	"data\\MODEL\\dry-leaf_large.x",								//[35] 茶色の葉っぱ(大)
	"data\\MODEL\\dry-leaf_small.x",								//[36] 茶色の葉っぱ(小)
	"data\\MODEL\\StoneDragon.x",									//[37] 龍の石像
	"data\\MODEL\\StoneSword.x",									//[38] 剣の石像
};
const char* CBlock::OTHER_TEXTURE_PATHS[(int)OTHER_TEXTURE::MAX] = {
	"data\\TEXTURE\\Effect\\effect000.jpg",
};
const char* CBlock::OTHER_MODEL_PATHS[(int)OTHER_MODEL::MAX] = {
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf_Inside.x",
};
const char* CBlock::OTHER_SETUP3D_PATHS[(int)OTHER_SETUP3D::MAX] = {
	"data\\SETUP\\BaobabTree.txt",
	"data\\SETUP\\Chest.txt",
	"data\\SETUP\\PalmTree.txt",
};
const char* CBlock::OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX] = {
	"data\\MOTION\\ChestStepped.txt",
	"data\\MOTION\\PalmTree_Shake.txt",
};
const char* CBlock::OTHER_SOUND_PATHS[(int)OTHER_SOUND::MAX] = {
	"data\\SOUND\\SE\\coin.wav",
	"data\\SOUND\\SE\\lightA.wav",
	"data\\SOUND\\SE\\lightB.wav",
};

//========================================
// 変数定義
//========================================
UShort CBlock::m_num = 0;
Scale2D CBlock::m_eyescale = Scale2D(5.0f, 5.0f);
short CBlock::m_modelIdxes[(int)LOOKS_TYPE::MAX];
short CBlock::m_otherTextureIdxes[(int)OTHER_TEXTURE::MAX];
short CBlock::m_otherModelIdxes[(int)OTHER_MODEL::MAX];
short CBlock::m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::MAX];
short CBlock::m_otherMotion3DIdxes[(int)OTHER_MOTION3D::MAX];
short CBlock::m_otherSoundIdxes[(int)OTHER_SOUND::MAX];

//========================================
// 読み込み処理
//========================================
void CBlock::Load(void) {

	// テクスチャの読み込み
	for (int cnt = 0; cnt < (int)OTHER_TEXTURE::MAX; cnt++) {
		m_otherTextureIdxes[cnt] = RNLib::Texture().Load(OTHER_TEXTURE_PATHS[cnt]);
	}

	// モデルの読み込み
	for (int cnt = 0; cnt < (int)LOOKS_TYPE::MAX; cnt++) {
		m_modelIdxes[cnt] = RNLib::Model().Load(MODEL_PATHS[cnt]);
	}
	for (int cnt = 0; cnt < (int)OTHER_MODEL::MAX; cnt++) {
		m_otherModelIdxes[cnt] = RNLib::Model().Load(OTHER_MODEL_PATHS[cnt]);
	}

	// セットアップ3D読み込み
	for (int cnt = 0; cnt < (int)OTHER_SETUP3D::MAX; cnt++) {
		m_otherSetUp3DlIdxes[cnt] = RNLib::SetUp3D().Load(OTHER_SETUP3D_PATHS[cnt]);
	}

	// モーション3D読み込み
	for (int cnt = 0; cnt < (int)OTHER_MOTION3D::MAX; cnt++) {
		m_otherMotion3DIdxes[cnt] = RNLib::Motion3D().Load(OTHER_MOTION3D_PATHS[cnt]);
	}

	// サウンド読み込み
	for (int cnt = 0; cnt < (int)OTHER_SOUND::MAX; cnt++) {
		m_otherSoundIdxes[cnt] = RNLib::Sound().Load(OTHER_SOUND_PATHS[cnt]);
	}
}

//========================================
// コンストラクタ
//========================================
CBlock::CBlock(void) {

	Manager::StageObjectMgr()->AddList(this);
	for (int cnt = 0; cnt < 6; cnt++) {
		m_isHits[cnt] =
		m_isHitOlds[cnt] = false;
	}
	m_isCollision  = true;
	m_doll         = NULL;
	m_type         = TYPE::BLOCK;
	m_width        = SIZE_OF_1_SQUARE;
	m_height       = SIZE_OF_1_SQUARE;
	m_targetAddPos = INITPOS3D;
	m_oldAddPos    = INITPOS3D;
	m_addPos       = INITPOS3D;
	m_counter      = 0;
	m_counterMax   = 0;
	m_nTexIdx = RNLib::Texture().Load("data\\TEXTURE\\Eye.png");
	m_num++;
}

//========================================
// デストラクタ
//========================================
CBlock::~CBlock() {

	CMemory::Release(&m_doll);
	m_num--;
}

//========================================
// 初期化
//========================================
HRESULT CBlock::Init(LOOKS_TYPE looksType) {
	m_looksType = looksType;

	// ドールの生成
	const float correctHalf = (-8.0f * (m_pos.y / fabsf(m_pos.y)));
	switch (m_looksType) {
	case LOOKS_TYPE::BAOBAB_TREE: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::BAOBAB_TREE]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, correctHalf - (((int)fabsf(m_pos.x) % 20) * (m_pos.y / fabsf(m_pos.y))), 30.0f + ((int)fabsf(m_pos.x) % 20)));
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	case LOOKS_TYPE::CHEST: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::CHEST]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, correctHalf, 0.0f));
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	case LOOKS_TYPE::PALMTREE: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::PALM_TREE]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, correctHalf, 0.0f));
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	}

	// 位置調整
	switch (m_looksType) {
	case LOOKS_TYPE::SOIL_BLOCK					:
	case LOOKS_TYPE::SOIL_BLOCK_GRASSY			:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK		:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_A	:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_B	:
	case LOOKS_TYPE::ANCIENT_STONE_BLOCK_PTN_C	:
	case LOOKS_TYPE::LEAF_BLOCK					:
	case LOOKS_TYPE::LEAF_LARGE					:
	case LOOKS_TYPE::LEAF_SMALL					:
	case LOOKS_TYPE::DRY_LEAF_LARGE				:
	case LOOKS_TYPE::DRY_LEAF_SMALL				:
		m_pos.z -= ((int)fabsf(m_pos.x + m_pos.y) % 20) * 0.5f;
		break;
	case LOOKS_TYPE::TORCH						:
	case LOOKS_TYPE::SNAKE						:
	case LOOKS_TYPE::STONE_DRAGON				:
	case LOOKS_TYPE::STONE_SWORD				:
		m_pos.z += 30.0f + ((int)fabsf(m_pos.x) % 20);
		break;
	case LOOKS_TYPE::PILE_OF_COINS:
		m_pos.y -= (m_pos.y / fabsf(m_pos.y)) * SIZE_OF_1_SQUARE * 0.5f;
		break;
	}

	return S_OK;
}

//========================================
// 終了
//========================================
void CBlock::Uninit(void) {
	
}

//========================================
// 更新
//========================================
void CBlock::Update(void) {

	static const short outLineIdx = 10;
	m_num;

	switch (m_looksType) {
	case LOOKS_TYPE::LEAF_BLOCK: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::CHEST: {

		if (m_pos.y > 0.0f) {
			// ジャンプで離れた瞬間
			if (m_isHitOlds[(int)CCollision::ROT::OVER] && !m_isHits[(int)CCollision::ROT::OVER] && CMode_Game::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::FACE)->bJump) {
				m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::CHEST_STEPPED]);
			}
		}
		else {
			// ジャンプで離れた瞬間
			if (m_isHitOlds[(int)CCollision::ROT::UNDER] && !m_isHits[(int)CCollision::ROT::UNDER] && CMode_Game::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::BEHIND)->bJump) {
				m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::CHEST_STEPPED]);
			}
		}
	}break;
	case LOOKS_TYPE::PILE_OF_COINS: {

		if (m_pos.y > 0.0f) {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::OVER] && m_isHits[(int)CCollision::ROT::OVER]) {
				for (int cnt = 0; cnt < 3; cnt++)
					Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::SWAP_PARTI00), m_pos, Scale3D(8.0f,8.0f,0.0f), Color{ 255,200,0,255 });
				RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], CSound::CATEGORY::SE, 1.0f, false);
			}
		}
		else {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				for (int cnt = 0; cnt < 3; cnt++)
					Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::SWAP_PARTI00), m_pos, Scale3D(8.0f, 8.0f, 0.0f), Color{ 255,200,0,255 });

				RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], CSound::CATEGORY::SE, 1.0f, false);
			}
		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::STONE_MONUMENT: {

		if (m_pos.y > 0.0f) {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::OVER] && m_isHits[(int)CCollision::ROT::OVER]) {
				if (++m_counter == 1)
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], CSound::CATEGORY::SE, 1.0f, false);
			}

			// 乗られたことがある
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTextureIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + CGeometry::GetRandomVec() * fRand() * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (rand() % 40),(100 + rand() % 80),(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}
		else {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				if (++m_counter == 1)
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], CSound::CATEGORY::SE, 1.0f, false);
			}

			// 乗られたことがある
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTextureIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + CGeometry::GetRandomVec() * fRand() * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (rand() % 40),(100 + rand() % 80),(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA)
			->SetBrightnessOfEmissive((float)m_counter / 30);
	}break;
	case LOOKS_TYPE::PALMTREE: {
		if (++m_counter > 240)
		{
			m_doll->SetMotion(m_otherMotion3DIdxes[(int)OTHER_MOTION3D::PLAMTREE_SHAKE]);
			m_counter = 0;
		}
	}break;
	case LOOKS_TYPE::TREE_EYES_BLOCK: {
		D3DXVECTOR3 pos;
		if (m_pos.y > 0)
		{
			pos = CMode_Game::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::FACE)->pos;
			float fatan = -CGeometry::FindAngleXY(m_pos, pos);

			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x - 4.5f, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x + 4.5f, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
		}
		else
		{
			pos = CMode_Game::GetPlayer()->GetInfo(CPlayer::WORLD_SIDE::BEHIND)->pos;
			float fatan = -CGeometry::FindAngleXY(m_pos, pos);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x, m_pos.y - 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x - 4.5f, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);
			RNLib::Polygon3D().Put(PRIORITY_OBJECT, D3DXVECTOR3(m_pos.x + 4.5f, m_pos.y + 4.0f, m_pos.z - 10.0f), D3DXVECTOR3(0.0f, 0.0f, fatan), false)
				->SetSize(m_eyescale)
				->SetTex(m_nTexIdx);

		}

		

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);

	}break;
	case LOOKS_TYPE::LEAF_BLOCK_NUTS: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::TORCH: {
		// 炎のエフェクト
		float m_ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.4) + 1.0f);

		D3DXVECTOR3 m_TexPos = D3DXVECTOR3(m_pos.x + (float)(rand() % (int)m_width - m_width * 0.5), m_pos.y > 0.0f ? m_pos.y + 15.0f : m_pos.y - 15.0f, m_pos.z);

		Manager::EffectMgr()->ParticleCreate(RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_steam_000.png"), m_TexPos, D3DXVECTOR3(m_ScaleTex, m_ScaleTex, 0.0f), Color{ 255,50,0,255 }, CParticle::TYPE::TYPE_FLOATUP, 200, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI));

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::SNAKE: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f,m_pos.z) , m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::LEAF_LARGE: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::LEAF_SMALL: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::DRY_LEAF_LARGE: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::DRY_LEAF_SMALL: {
		if (--m_counter <= 0) {
			m_counterMax = 60 + rand() % 60;
			m_counter = m_counterMax;
			m_oldAddPos = m_addPos;
			m_targetAddPos = CGeometry::GetRandomVec() * (1.0f + fRand());
		}

		float rate = CEase::Easing(CEase::TYPE::INOUT_SINE, m_counter, m_counterMax);
		m_addPos = (m_oldAddPos * rate) + (m_targetAddPos * (1.0f - rate));

		RNLib::Model().Put(PRIORITY_OBJECT, m_otherModelIdxes[(int)OTHER_MODEL::LEAF_INSIDE], m_pos - m_addPos * 0.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos + m_addPos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y + 5.0f : m_pos.y - 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::STONE_DRAGON: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	case LOOKS_TYPE::STONE_SWORD: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], D3DXVECTOR3(m_pos.x, m_pos.y > 0.0f ? m_pos.y - 5.0f : m_pos.y + 5.0f, m_pos.z), m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	default: {
		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y >= 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA);
	}break;
	}

	// 過去の当たりフラグ保存
	for (int cnt = 0; cnt < 6; cnt++) {
		m_isHitOlds[cnt] = m_isHits[cnt];
		m_isHits[cnt] = false;
	}
}

//========================================
// 描画
//========================================
void CBlock::Draw(void) {

}