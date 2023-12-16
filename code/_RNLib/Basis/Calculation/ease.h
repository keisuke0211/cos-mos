//========================================
// 
// ��ԏ���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// ��ԃN���X
class _RNC_Ease {
public:
	//========== [[[ �񋓌^��` ]]]
	// ���
	enum class TYPE { LINEAR, IN_SINE, OUT_SINE, INOUT_SINE, MAX, };

	//========== [[[ �֐��錾 ]]]
	_RNC_Ease   ();
	~_RNC_Ease  ();
	void  Init  (void);
	void  Uninit(void);
	void  Update(void);
	float Easing(const TYPE& ease, const int& base, const int& max);
	float Easing(const TYPE& ease, const float& base, const float& max);
};