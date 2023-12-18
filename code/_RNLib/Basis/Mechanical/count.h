//========================================
// 
// �J�E���g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �J�E���g�N���X
class _RNC_Count {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Count       ();
	~_RNC_Count      ();
	void   Init      (void);
	void   Uninit    (void);
	void   Update    (void);
	UShort GetCount  (void) { return m_count; };
	bool   GetBlinkF2(void) { return m_isBlinkF2; }
	bool   GetBlinkF4(void) { return m_isBlinkF4; }

private:
	//========== [[[ �ϐ��錾 ]]]
	UShort m_count;
	bool   m_isBlinkF2;
	bool   m_isBlinkF4;
};