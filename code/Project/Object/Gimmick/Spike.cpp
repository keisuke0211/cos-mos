//========================================
// 
// 棘
// Author:KEISUKE OTONO
// 
//========================================
// *** spike.h ***
//========================================
#include "Spike.h"
#include "../../main.h"

#define MAX_COUNT (30 + (rand() % 30))	//カウント
#define RAND_TEX  (rand() % 2)			//テクスチャランダム
//========================================
// コンストラクタ
//========================================
CSpike::CSpike(void)
{
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::SPIKE;
	m_width = SIZE_OF_1_SQUARE;
	m_height = SIZE_OF_1_SQUARE * 0.5f;

	m_Info.move = INITD3DXVECTOR3;
	m_Info.nModelIdx = RNLib::Model().Load("data\\MODEL\\Spike.x");

	m_nCnt = MAX_COUNT;
	m_nTex[0] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\Bubble.png");
	m_nTex[1] = RNLib::Texture().Load("data\\TEXTURE\\Effect\\eff_Smoke_001.png");
}

//========================================
// デストラクタ
//========================================
CSpike::~CSpike()
{
	
}

//========================================
// 初期化
//========================================
HRESULT CSpike::Init(void)
{
	m_Info.move = INITD3DXVECTOR3;

	return S_OK;
}

//========================================
// 終了
//========================================
void CSpike::Uninit(void)
{

}

//========================================
// 更新
//========================================
void CSpike::Update(void)
{
	// 過去の位置
	RNLib::Model().Put(PRIORITY_OBJECT, m_Info.nModelIdx, m_pos, m_rot, false)
		->SetOutLineIdx(true)
		->SetCol(m_color);

	if (m_nCnt > 0)
		m_nCnt--;
	else
	{
		m_nCnt = MAX_COUNT;
		float ScaleTex = (float)(rand() % (int)(INIT_EFFECT_SCALE.x * 0.2) + 1.0f);

		D3DXVECTOR3 TexPos;
		Rot3D rot = m_rot;

		if(m_rot.z == D3DX_PI || m_rot.z == 0)
			TexPos = D3DXVECTOR3(m_pos.x + (float)(rand() % (int)m_width - m_width * 0.5), m_pos.y, m_pos.z);
		else {
			rot.z = m_rot.z + D3DX_PI;
			TexPos = D3DXVECTOR3(m_pos.x, m_pos.y + (float)(rand() % (int)m_height - m_height * 0.5), m_pos.z);

		}

		Manager::EffectMgr()->ParticleCreate(m_nTex[RAND_TEX], TexPos,D3DXVECTOR3(ScaleTex, ScaleTex,0.0f), Color{ 255,0,255,255 }, CParticle::TYPE::TYPE_FLOATUP, 300,rot,D3DXVECTOR3(40.0f,40.0f,0.0f));
	}
}

//========================================
// 描画
//========================================
void CSpike::Draw(void)
{

}