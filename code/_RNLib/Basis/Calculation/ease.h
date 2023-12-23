//========================================
// 
// ��ԏ���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �񋓌^��`
//****************************************
// ��Ԃ̎��
enum class EASE_TYPE { 
	LINEAR,
	IN_SINE,
	OUT_SINE,
	INOUT_SINE,
	MAX,
};

//****************************************
// �N���X��`
//****************************************
// ��ԃN���X
class _RNC_Ease {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Ease ();
	~_RNC_Ease();
	float Easing(const EASE_TYPE& ease, const int& base, const int& max);
	float Easing(const EASE_TYPE& ease, const float& base, const float& max);

private:
	//========== [[[ �F�B�錾 ]]]
	friend class _RNC_Calculation;

	//========== [[[ �֐��錾 ]]]
	void Init  (void);
	void Uninit(void);
	void Update(void);
};