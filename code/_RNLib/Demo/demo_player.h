//========================================
// 
// デモプレイヤーの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// クラス定義
//****************************************
// デモプレイヤークラス
class CDemoPlayer : CObject {
public:
	//========== [[[ 関数宣言 ]]]
	static void SetControl(CDemoPlayer* control) { ms_control = control; }
	static void SetCameraClipping(const bool& isCameraClipping) { ms_isCameraClipping = isCameraClipping; }
	static bool GetCameraClipping(void) { return ms_isCameraClipping; }
	CDemoPlayer(const Pos3D& pos, const Rot3D& rot, const Color& col, const Color& cameraCol);
	~CDemoPlayer();
	void Update(void);
	CCamera& GetCamera(void) { return *m_camera; }
	void SetClippingCameraID(const short& ID) { m_clippingCameraID = ID; }

private:
	//========== [[[ 変数宣言 ]]]
	static CDemoPlayer* ms_control;
	static bool ms_isCameraClipping;
	Pos3D    m_pos;
	Rot3D    m_rot;
	Color    m_col;
	CCamera* m_camera;
	short    m_clippingCameraID;
};