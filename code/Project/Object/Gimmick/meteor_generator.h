//================================================================================================
//
//覐ΐ�����̏���[meteor_generator.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _METEOR_GENERATOR_H_
#define _METEOR_GENERATOR_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"

//****************************************
// �N���X��`
//****************************************
// ���e�I�̏��\����
class CMeteorGenerator : public CStageObject {
public:
	//========== [[[ �֐��錾 ]]]
	CMeteorGenerator();
	virtual        ~CMeteorGenerator();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

private:
};

#endif