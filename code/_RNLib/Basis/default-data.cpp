//========================================
// 
// デフォルトデータの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]デフォルトデータクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 定数定義
//****************************************
const char* CDefaultData::SOUND_PATH[(int)SOUND::MAX] = {
	"RNData\\Sound\\TestBGM.wav",
	"RNData\\Sound\\TestSE.wav",
};
const char* CDefaultData::MODEL_PATH[(int)MODEL::MAX] = {
	"RNData\\Model\\Sphere.x",
	"RNData\\Model\\Observer.x",
	"RNData\\Model\\Prufen\\Head.x",
	"RNData\\Model\\Prufen\\Body.x",
	"RNData\\Model\\Prufen\\Limb_Up.x",
	"RNData\\Model\\Prufen\\Limb_Down.x",
	"RNData\\Model\\Prufen\\Limb_Left.x",
	"RNData\\Model\\Prufen\\Limb_Right.x",
};

//========================================
// コンストラクタ
//========================================
CDefaultData::CDefaultData(void) {

	for (int cntSound = 0; cntSound < (int)SOUND::MAX; m_soundIdxes[cntSound] = NONEDATA, cntSound++);
	for (int cntModel = 0; cntModel < (int)MODEL::MAX; m_modelIdxes[cntModel] = NONEDATA, cntModel++);
}

//========================================
// デストラクタ
//========================================
CDefaultData::~CDefaultData(void) {

}

//========================================
// 初期化処理
//========================================
void CDefaultData::Init(void) {

	// サウンド読み込み
	for (int cntSound = 0; cntSound < (int)SOUND::MAX; cntSound++)
		m_soundIdxes[cntSound] = RNLib::Sound().Load(SOUND_PATH[cntSound]);

	// モデル読み込み
	for (int cntModel = 0; cntModel < (int)MODEL::MAX; cntModel++)
		m_modelIdxes[cntModel] = RNLib::Model().Load(MODEL_PATH[cntModel]);
}

//========================================
// 終了処理
//========================================
void CDefaultData::Uninit(void) {

}

//========================================
// 更新処理
//========================================
void CDefaultData::Update(void) {

}