//========================================
// 
// �J�ڂ̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../../_RNLib/RNlib.h"

//****************************************
// �N���X��`
//****************************************
// �J�ڃN���X
class CTransition {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class STATE { NONE, OPEN, OPEN_WAIT, BLACK, CLOSE, };
	enum class TYPE  { NONE, NUI,  FADE, };

	//========== [[[ �֐��錾 ]]]
	CTransition();
	~CTransition();
	void   Init    (void);
	void   Uninit  (void);
	void   Update  (void);
	bool   Open    (const TYPE& type, const UShort& time);
	bool   Close   (const TYPE& type, const Color& col, const UShort& time);
	STATE& GetState(void) { return m_state; }

private:
	//========== [[[ �֐��錾 ]]]
	void FillScreen(const float& rate);

	//========== [[[ �ϐ��錾 ]]]
	TYPE   m_type;
	STATE  m_state;
	UShort m_stateCounter;
	UShort m_time;
	Color  m_col;
	float  m_fMove;
	int	   m_texIdxes[3];
	float  m_startGearAngle;
};