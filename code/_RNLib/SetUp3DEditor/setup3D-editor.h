//========================================
// 
// セットアップ3Dエディターの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../RNlib.h"

//****************************************
// クラス定義
//****************************************
// セットアップ3Dエディタークラス
class CSetUp3DEditor {
public:
	//========== [[[ 関数宣言 ]]]
	CSetUp3DEditor();
	~CSetUp3DEditor();
	void Init(void);
	void Uninit(void);
	void Update(void);

private:
	//========== [[[ 定数定義 ]]]
	static const int SLITHER_TIME = 60;

	//========== [[[ 列挙型定義 ]]]
	enum class MESSAGE_TYPE { NONE, SETUP_RELOAD_SUCCEEDED, MOTION_RELOAD_SUCCEEDED, SETUP_RELOAD_FAILED, MOTION_RELOAD_FAILED, };

	//========== [[[ 変数宣言 ]]]
	CCamera*     m_camera;
	CDoll3D*     m_doll;
	Vector3D     m_slither;
	int          m_slitherCounter;
	char*        m_editPath;
	char*        m_motionPath;
	MESSAGE_TYPE m_messageType;
	UShort       m_messageCounter;
};