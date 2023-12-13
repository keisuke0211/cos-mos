//========================================
// 
// ���[�h:�Q�[���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// �O���錾
//****************************************
class CPlayer;
class CFontText;
class CRocketPartsUI;
class CCoinUI;
class CMenuUI;

//****************************************
// �N���X��`
//****************************************
// ���[�h(�Q�[��)�N���X
class CMode_Game :public CMode {
public:
	//========== [[[ �萔��` ]]]

	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		NONE,
		PAUSE,
	};

	// �v���C�^�C�����
	struct GameTime {
		DWORD Start; // �Q�[���J�n����
		DWORD Pause; // �|�[�Y���̎���
		DWORD Play;  // �v���C���Ԃ��i�[
		DWORD End;   // �X�e�[�W�N���A����

		DWORD LastPause;//�Ō�Ƀ|�[�Y����������

		bool bMeasure;// �v�����邩�ǂ���
	};
	//========== [[[ �֐��錾 ]]]
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);

	//�v���C���Ԃ�Ԃ�
	static float GetPlayTime(void);

	//�v��ON/OFF true�FON  false�FOFF
	static void SetMeasureTime(const bool bMeasure) { s_GameTime.bMeasure = bMeasure; }

	static void RestartTime(void);

private:
	//========== [[[ �񋓌^��` ]]]
	enum class TimeType {
		Play = 0, // �v���C����
		Pause,    // �|�[�Y����
		Max
	};

	//========== [[[ �֐��錾 ]]]
	oid MeasureTime(TimeType type);
	static void FormatGameTime(void);

	//========== [[[ �ϐ��錾 ]]]
	static GameTime s_GameTime; //�v���C����
};