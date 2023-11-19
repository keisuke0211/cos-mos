//========================================
// 
// �f�t�H���g�f�[�^�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �f�t�H���g�f�[�^�N���X
class CDefaultData {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class SOUND   { TEST_BGM, TEST_SE, MAX, };
	enum class TEXTURE { TILE, GRID, STRIPE, BORDER, MAX };
	enum class MODEL   { SPHERE, DOME_UP, DOME_DOWN, OBSERVER, MONITOR, PRUFEN_HEAD, PRUFEN_BODY, PRUFEN_LIMB_UP, PRUFEN_LIMB_DOWN, PRUFEN_LIMB_LEFT, PRUFEN_LIMB_RIGHT, MAX, };
	enum class SETUP3D { CAPSULE, MAX };

	//========== [[[ �֐��錾 ]]]
	CDefaultData();
	~CDefaultData();
	void Init  (void);
	void Uninit(void);
	void Update(void);
	short& GetSoundIdx  (const SOUND&   sound  ) { return m_soundIdxes  [(int)sound  ]; }
	short& GetTextureIdx(const TEXTURE& texture) { return m_textureIdxes[(int)texture]; }
	short& GetModelIdx  (const MODEL&   model  ) { return m_modelIdxes  [(int)model  ]; }
	short& GetSetUp3DIdx(const SETUP3D& setUp3D) { return m_setUp3DIdxes[(int)setUp3D]; }

private:
	//========== [[[ �萔�錾 ]]]
	static const char* SOUND_PATHS  [(int)SOUND  ::MAX];
	static const char* TEXTURE_PATHS[(int)TEXTURE::MAX];
	static const char* MODEL_PATHS  [(int)MODEL  ::MAX];
	static const char* SETUP3D_PATHS[(int)SETUP3D::MAX];

	//========== [[[ �ϐ��錾 ]]]
	short m_soundIdxes  [(int)SOUND  ::MAX];
	short m_textureIdxes[(int)TEXTURE::MAX];
	short m_modelIdxes  [(int)MODEL  ::MAX];
	short m_setUp3DIdxes[(int)SETUP3D::MAX];
};
