//========================================
// 
// レール3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// クラス定義
//****************************************
// レール3Dエディタークラス
class CRail3DEditor {
public:
	//========== [[[ 関数宣言 ]]]
	CRail3DEditor();
	~CRail3DEditor();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ 変数宣言 ]]]
	CCamera m_camera;
	String  m_editPath;
	CRail3D m_rail3D;
	short   m_counter;
	short   m_time;
	short   m_selectPointIdx;
	bool    m_isSaved;
};