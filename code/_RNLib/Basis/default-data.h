//========================================
// 
// デフォルトデータの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// デフォルトデータクラス
class CDefaultData {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class SOUND { TEST_BGM, TEST_SE, MAX, };
	enum class MODEL { SPHERE, OBSERVER, PRUFEN_HEAD, PRUFEN_BODY, PRUFEN_LIMB_UP, PRUFEN_LIMB_DOWN, PRUFEN_LIMB_LEFT, PRUFEN_LIMB_RIGHT, MAX, };

	//========== [[[ 関数宣言 ]]]
	CDefaultData();
	~CDefaultData();
	void Init(void);
	void Uninit(void);
	void Update(void);
	short& GetSoundIdx(const SOUND& sound) { return m_soundIdxes[(int)sound]; }
	short& GetModelIdx(const MODEL& model) { return m_modelIdxes[(int)model]; }

private:
	//========== [[[ 定数宣言 ]]]
	static const char* SOUND_PATH[(int)SOUND::MAX];
	static const char* MODEL_PATH[(int)MODEL::MAX];

	//========== [[[ 変数宣言 ]]]
	short m_soundIdxes[(int)SOUND::MAX];
	short m_modelIdxes[(int)MODEL::MAX];
};
