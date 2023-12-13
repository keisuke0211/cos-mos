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

	//========== [[[ �֐��錾 ]]]
	CMode_Game();
	~CMode_Game();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void ProcessState(const PROCESS process);

	// *** �ÓI�ϐ� ***
	static CMenuUI *m_MenuUI;

private:
	//========== [[[ �֐��錾 ]]]

};