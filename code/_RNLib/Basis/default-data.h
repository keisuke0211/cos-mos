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
class _RNC_DefaultData {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class SOUND    { TEST_BGM, TEST_SE, MAX, };
	enum class TEXTURE  { TILE, GRID, STRIPE, BORDER, SMOKE, MAX };
	enum class MODEL    { CUBE, SPHERE, DOME_UP, DOME_DOWN, LINEAR_LIGHT, OBSERVER, MONITOR, PRUFEN_HEAD, PRUFEN_BODY, PRUFEN_LIMB_UP, PRUFEN_LIMB_DOWN, PRUFEN_LIMB_LEFT, PRUFEN_LIMB_RIGHT, MAX, };
	enum class SETUP3D  { CAPSULE, MAX };
	enum class MOTION3D { CAPSULE_STRETCH, MAX };

	//========== [[[ 関数宣言 ]]]
	_RNC_DefaultData     ();
	~_RNC_DefaultData    ();
	void   Init          (void);
	void   Uninit        (void);
	void   Update        (void);
	short& GetSoundIdx   (const SOUND&    sound   ) { return m_soundIdxes   [(int)sound   ]; }
	short& GetTextureIdx (const TEXTURE&  texture ) { return m_textureIdxes [(int)texture ]; }
	short& GetModelIdx   (const MODEL&    model   ) { return m_modelIdxes   [(int)model   ]; }
	short& GetSetUp3DIdx (const SETUP3D&  setUp3D ) { return m_setUp3DIdxes [(int)setUp3D ]; }
	short& GetMotion3DIdx(const MOTION3D& motion3D) { return m_motion3DIdxes[(int)motion3D]; }

private:
	//========== [[[ 定数宣言 ]]]
	static const char* SOUND_PATHS   [(int)SOUND   ::MAX];
	static const char* TEXTURE_PATHS [(int)TEXTURE ::MAX];
	static const char* MODEL_PATHS   [(int)MODEL   ::MAX];
	static const char* SETUP3D_PATHS [(int)SETUP3D ::MAX];
	static const char* MOTION3D_PATHS[(int)MOTION3D::MAX];

	//========== [[[ 変数宣言 ]]]
	short m_soundIdxes   [(int)SOUND   ::MAX];
	short m_textureIdxes [(int)TEXTURE ::MAX];
	short m_modelIdxes   [(int)MODEL   ::MAX];
	short m_setUp3DIdxes [(int)SETUP3D ::MAX];
	short m_motion3DIdxes[(int)MOTION3D::MAX];
};
