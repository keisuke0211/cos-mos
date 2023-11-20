//========================================
// 
// �f���I�u�W�F�N�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNmain.h"
#include "../Basis/Mechanical/object.h"

//****************************************
// �N���X��`
//****************************************
// �f���I�u�W�F�N�g�N���X
class CDemoObject : public CObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class TYPE { PLAYER, ZONE, MAX, };

	//========== [[[ �֐��錾 ]]]
	CDemoObject(const TYPE& type);
	virtual ~CDemoObject();
	void  Update (void) = 0;
	TYPE& GetType(void)             { return m_type; }
	void  SetType(const TYPE& type) { m_type = type; }

protected:
	//========== [[[ �ϐ��錾 ]]]
	TYPE m_type;
};