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

	//========== [[[ �֐��錾 ]]]
	CBat();
	virtual        ~CBat();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);
	virtual void   Draw(void);

	void	SetModelIdx(int idx) { ModelIdx = idx; }

protected:

private:
	//========== [[[ �ϐ��錾 ]]]
	int ModelIdx;

};

