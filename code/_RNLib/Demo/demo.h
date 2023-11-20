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
	enum class PLAYER   { RED, BLUE, MAX, };

	//========== [[[ �֐��錾 ]]]
	CDemo();
	~CDemo();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CDemoPlayer& GetPlayer(const PLAYER& player) { return *m_players[(int)player]; }
	CObjectMgr&  GetDemoObjMgr(void) { return m_demoObjMgr; }

private:
	//========== [[[ �񋓌^��` ]]]
	enum class SCREEN_TYPE { NORMAL, DIVISION, MAX };

	//========== [[[ �萔�錾 ]]]
	static const char* PLAYER_NAMES[(int)PLAYER::MAX];
	static const char* SCREEN_TYPE_NAMES[(int)SCREEN_TYPE::MAX];

	//========== [[[ �ϐ��錾 ]]]
	CDemoPlayer* m_players[(int)PLAYER::MAX];
	UShort       m_controlPlayerCount;
	UShort       m_screenTypeCount;
	CObjectMgr   m_demoObjMgr;
};

// RN�f��
namespace RNDemo {
	CDemo& Get(void);
}