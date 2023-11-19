//========================================
// 
// 波打つ光の処理
// Author:KOMURO HIROMU
// 
//========================================
#include "ShiningWave.h"
#include "../../main.h"
//================================================================================
//----------|---------------------------------------------------------------------
//==========| CShiningWaveクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================
static const 	int		 s_nNumShining = 30;	// 光の総数
static const D3DXVECTOR3 s_ShiningSummonPos =	// 光の出現する位置
D3DXVECTOR3(-120.0f, 120.0f, 200.0f);

//========================================
// コンストラクタ
//========================================
CShiningWave::CShiningWave(void) {
	Manager::StageObjectMgr()->AddList(this);

	m_type = TYPE::BACKGROUND;	// 種類の設定
								// 大きさの設定
	m_width = SIZE_OF_1_SQUARE * 5;
	m_height = SIZE_OF_1_SQUARE * 5;

	// 各情報の初期化
}

//========================================
// デストラクタ
//========================================
CShiningWave::~CShiningWave(void) {

}

//========================================
// 初期化処理
// Author:KOMURO HIROMU
//========================================
void CShiningWave::Init(void) {
	//ModelIdx = RNLib::Model().Load(s_modelPaths[(int)m_Star_type]);
	//RNLib::Texture().Load();

	
}

//========================================
// 終了処理
// Author:KOMURO HIROMU
//========================================
void CShiningWave::Uninit(void) {

}

//========================================
// 更新処理
// Author:KOMURO HIROMU
//========================================
void CShiningWave::Update(void) {

	/*for (int nCntShin = 0; nCntShin < s_nNumShining; nCntShin++)
	{
		nShining = nCntShin % 12;
		RNLib::Polygon3D().Put(D3DXVECTOR3(s_ShiningSummonPos.x + (20.0f * nShining), s_ShiningSummonPos.x + (20.0f * nShining), m_pos.z), m_rot, false)
			->SetLighting(false)
			->SetSize(10.0f, 10.0f);
	}*/
}

//========================================
// 描画処理
// Author:KOMURO HIROMU
//========================================
void CShiningWave::Draw(void) {


}