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
	enum class SOUND { TEST_BGM, TEST_SE, MAX, };
	enum class MODEL { SPHERE, OBSERVER, PRUFEN_HEAD, PRUFEN_BODY, PRUFEN_LIMB_UP, PRUFEN_LIMB_DOWN, PRUFEN_LIMB_LEFT, PRUFEN_LIMB_RIGHT, MAX, };

	//========== [[[ �֐��錾 ]]]
	CDefaultData();
	~CDefaultData();
	void Init(void);
	void Uninit(void);
	void Update(void);
	short& GetSoundIdx(const SOUND& sound) { return m_soundIdxes[(int)sound]; }
	short& GetModelIdx(const MODEL& model) { return m_modelIdxes[(int)model]; }

private:
	//========== [[[ �萔�錾 ]]]
	static const char* SOUND_PATH[(int)SOUND::MAX];
	static const char* MODEL_PATH[(int)MODEL::MAX];

	//========== [[[ �ϐ��錾 ]]]
	short m_soundIdxes[(int)SOUND::MAX];
	short m_modelIdxes[(int)MODEL::MAX];
};
