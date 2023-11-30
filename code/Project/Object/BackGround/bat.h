//========================================
// 
// �啂̏���
// Author:KOMURO HIROMU
// 
//========================================
#pragma once

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"
//****************************************
// �N���X��`
//****************************************
// �f���̏��\����
class CBat : public CStageObject {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class STATE {
		STATE_DEFAULT,
		STATE_FLY,
		STATE_MAX,
	};

	//========== [[[ �֐��錾 ]]]
	CBat();
	virtual        ~CBat();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

	void SeState(STATE state) { m_State = state; }	// ��Ԃ̐ݒ�

protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	CDoll3D* m_doll;
	STATE m_State;
	D3DXVECTOR3 m_move;
	int m_nFlyCounter;
	int m_nStayCounter;
	int m_nIdxMotion;
	bool m_bUpDown;
};

