//========================================
// 
// 惑星の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "Planet.h"
#include "../../main.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| CPlanetクラス
//----------|---------------------------------------------------------------------
//================================================================================
static const int s_maxAnimeCounter = 120;		// アニメーションのカウンター最大
static const char* s_modelPaths[(int)CPlanet::STAR_TYPE::MAX] = {
	"data\\MODEL\\Saturn_Star.x",
	"data\\MODEL\\Saturn_2_Star.x",
	"data\\MODEL\\Saturn_4_Star.x",
	"data\\MODEL\\Saturn_8_Star.x",
};
static const D3DXVECTOR3 s_moveRot = D3DXVECTOR3(0.0005f, 0.0005f, 0.0f);
static const float s_PosOver = 100.0f;	

//========================================
// コンストラクタ
//========================================
CPlanet::CPlanet(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::BACKGROUND;	// 種類の設定
								// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 5;
	m_height = SIZE_OF_1_SQUARE * 5;

	// 各情報の初期化
	m_posInit = INITD3DXVECTOR3;
	m_rot = INITD3DXVECTOR3;
	m_move = INITD3DXVECTOR3;
	m_col = Color{ 255,255,255,255 };
	m_rot = INITD3DXVECTOR3;
	m_moveCounter = 0;
}

//========================================
// デストラクタ
//========================================
CPlanet::~CPlanet(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CPlanet::Init(void) {
	ModelIdx = RNLib::Model().Load(s_modelPaths[(int)m_Star_type]);

	m_posInit = m_pos;
}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CPlanet::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CPlanet::Update(void) {

	m_moveCounter++;	// カウンターを進める


	if (m_moveCounter % s_maxAnimeCounter == 0)
	{
		m_move = RNLib::Geometry().GetRandomVec() * 0.05f;
		m_move.z = 0.0f;
	}

	// 移動量を位置に増加
	m_pos += m_move;
	m_rot += s_moveRot;

	if (m_posInit.x + s_PosOver <= m_pos.x || m_posInit.x - s_PosOver >= m_pos.x)
	{
		m_move *= -1;
	}
	
	RNLib::Model().Put(PRIORITY_OBJECT, ModelIdx, m_pos, m_rot, false)
		->SetCol(m_col);
		
}

//========================================
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CPlanet::Draw(void) {


}