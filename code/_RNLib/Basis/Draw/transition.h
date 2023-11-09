//========================================
// 
// �J�ڂ̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �J�ڃN���X
class CTransition {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class STATE { NONE, OPEN, OPEN_WAIT, CLOSE, };
	enum class TYPE { NONE, FADE, };

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
};