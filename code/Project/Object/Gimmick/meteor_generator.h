//================================================================================================
//
//隕石生成器の処理[meteor_generator.h]
//Author:Hirasawa Shion
//
//================================================================================================
#ifndef _METEOR_GENERATOR_H_
#define _METEOR_GENERATOR_H_

#include "../../../_RNLib/RNlib.h"
#include"../stage-object.h"

//****************************************
// クラス定義
//****************************************
// メテオの情報構造体
class CMeteorGenerator : public CStageObject {
public:
	//========== [[[ 関数宣言 ]]]
	CMeteorGenerator();
	virtual        ~CMeteorGenerator();
	virtual void   Init(void);
	virtual void   Uninit(void);
	virtual void   Update(void);

private:
};

#endif