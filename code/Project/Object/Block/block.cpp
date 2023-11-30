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
	"data\\MODEL\\StageObject\\SoilBlock.x",
	"data\\MODEL\\StageObject\\SoilBlock_Grassy.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternA.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternB.x",
	"data\\MODEL\\StageObject\\AncientStoneBlock_PatternC.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Up.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Center.x",
	"data\\MODEL\\StageObject\\AncientStonePillar_Down.x",
	"data\\MODEL\\StageObject\\LeafBlock\\Leaf.x",
	"data\\MODEL\\StageObject\\TreeTrunk.x",
	"data\\MODEL\\StageObject\\TreeTrunk_Root.x",
	"NONEDATA",
	"NONEDATA",
	"data\\MODEL\\coin.x",
	"data\\MODEL\\stone_monument.x",
	"data\\MODEL\\StageObject\\AncientStoneBrickBlock.x",
	"data\\MODEL\\StageObject\\SoilAndAncientStoneBlock.x",
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
};
const char* CBlock::OTHER_MOTION3D_PATHS[(int)OTHER_MOTION3D::MAX] = {
	"data\\MOTION\\ChestStepped.txt",
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
	switch (m_looksType) {
	case LOOKS_TYPE::BAOBAB_TREE: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::BAOBAB_TREE]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, -8.0f - (((int)fabsf(m_pos.x) % 20) * (m_pos.y / fabsf(m_pos.y))), 30.0f + ((int)fabsf(m_pos.x) % 20)));
		if (m_pos.y < 0.0f) {
			m_doll->SetRot(Rot3D(0.0f, 0.0f, D3DX_PI));
		}
	}break;
	case LOOKS_TYPE::CHEST: {
		m_doll = new CDoll3D(PRIORITY_OBJECT, m_otherSetUp3DlIdxes[(int)OTHER_SETUP3D::CHEST]);
		m_doll->SetPos(m_pos + Pos3D(0.0f, -8.0f, 0.0f));
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
		m_pos.z -= ((int)fabsf(m_pos.x + m_pos.y) % 20) * 0.5f;
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

	static const short outLineIdx = 5;
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
			}

			RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], CSound::CATEGORY::SE, false);
		}
		else {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				for (int cnt = 0; cnt < 3; cnt++)
					Manager::EffectMgr()->ParticleCreate(CPlayer::GetParticleIdx(CPlayer::PARTI_TEX::SWAP_PARTI00), m_pos, Scale3D(8.0f, 8.0f, 0.0f), Color{ 255,200,0,255 });

				RNLib::Sound().Play(m_otherSoundIdxes[(int)OTHER_SOUND::COIN], CSound::CATEGORY::SE, false);
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
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], CSound::CATEGORY::SE, false);
			}

			// 乗られたことがある
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTextureIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + CGeometry::GetRandomVec() * fRand() * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (UShort)(rand() % 40),(UShort)(100 + rand() % 80),(UShort)(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}
		else {
			// 乗られた瞬間
			if (!m_isHitOlds[(int)CCollision::ROT::UNDER] && m_isHits[(int)CCollision::ROT::UNDER]) {
				if (++m_counter == 1)
					RNLib::Sound().Play(m_otherSoundIdxes[rand() % 2 ? (int)OTHER_SOUND::LIGHT_A : (int)OTHER_SOUND::LIGHT_B], CSound::CATEGORY::SE, false);
			}

			// 乗られたことがある
			if (m_counter > 0) {
				if (++m_counter > 30) {
					m_counter = 30;
					if (rand() % 20 == 0) {
						Manager::EffectMgr()->ParticleCreate(m_otherTextureIdxes[(int)OTHER_TEXTURE::EFFECT], m_pos + CGeometry::GetRandomVec() * fRand() * 16.0f, Scale3D(8.0f, 8.0f, 0.0f), Color{ (UShort)(rand() % 40),(UShort)(100 + rand() % 80),(UShort)(100 + rand() % 80),255 }, CParticle::TYPE::TYPE_FIXED, 60 + rand() % 60);
					}
				}
			}
		}

		RNLib::Model().Put(PRIORITY_OBJECT, m_modelIdxes[(int)m_looksType], m_pos, m_pos.y > 0.0f ? Rot3D(0.0f, 0.0f, 0.0f) : Rot3D(0.0f, 0.0f, D3DX_PI), false)
			->SetCol(m_color)
			->SetOutLineIdx(m_isCollision ? outLineIdx : NONEDATA)
			->SetBrightnessOfEmissive((float)m_counter / 30);
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