//========================================
// 
// �f���̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// �O���錾
//****************************************
class CDemoPlayer;

//****************************************
// �N���X��`
//****************************************
// �f���N���X
class CDemo {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class PLAYER { RED, BLUE, MAX, };

	//========== [[[ �֐��錾 ]]]
	CDemo();
	~CDemo();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CObjectMgr& GetDemoObjMgr(void) { return m_demoObjMgr; }

private:
	//========== [[[ �ϐ��錾 ]]]
	CDemoPlayer* m_players[(int)PLAYER::MAX];
	UShort       m_controlPlayerCount;
	CObjectMgr   m_demoObjMgr;
};

// RN�f��
namespace RNDemo {
	CDemo& Get(void);
}