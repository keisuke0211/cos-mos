//========================================
// 
// RNオブジェクトの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "RNlib.h"
#include "RNsettings.h"

//****************************************
// クラス定義
//****************************************
// 3Dオブジェクト
class C3DObject {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {
		m_motion3D        .Init();
		m_setUp3D         .Init();
		m_effect3D        .Init();
		m_standardEffect3D.Init();
	}
	void Uninit(void) {
		m_motion3D        .Uninit();
		m_setUp3D         .Uninit();
		m_effect3D        .Uninit();
		m_standardEffect3D.Uninit();
	}
	void Update(void) {
		m_motion3D        .Update();
		m_setUp3D         .Update();
		m_effect3D        .Update();
		m_standardEffect3D.Update();
	}

	//========== [[[ 変数宣言 ]]]
	_RNC_Motion3D         m_motion3D;
	_RNC_SetUp3D          m_setUp3D;
	_RNC_Doll3DMgr        m_doll3DMgr;
	_RNC_Effect3D         m_effect3D;
	_RNC_Effect3DMgr      m_effect3DMgr;
	_RNC_StandardEffect3D m_standardEffect3D;
};

// 計算
class CCalculation {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {
		m_ease	  .Init();
		m_geometry.Init();
		m_hitTest .Init();
		m_matrix  .Init();
		m_number  .Init();
	}
	void Uninit(void) {
		m_ease	  .Uninit();
		m_geometry.Uninit();
		m_hitTest .Uninit();
		m_matrix  .Uninit();
		m_number  .Uninit();
	}
	void Update(void) {
		m_ease	  .Update();
		m_geometry.Update();
		m_hitTest .Update();
		m_matrix  .Update();
		m_number  .Update();
	}

	//========== [[[ 変数宣言 ]]]
	_RNC_Ease     m_ease;
	_RNC_Geometry m_geometry;
	_RNC_HitTest  m_hitTest;
	_RNC_Matrix   m_matrix;
	_RNC_Number   m_number;
};

// 描画
class CDraw {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(Device& device, const UShort& priorityMax) {
		m_staticMesh   .Init(priorityMax);
		m_model		.Init();
		m_polygon2D	.Init();
		m_polygon3D	.Init();
		m_text2D	.Init();
		m_text3D	.Init();
		m_drawMgr	.Init(priorityMax);
		m_drawState	.Init(device);
		m_light3D	.Init();
		m_text		.Init();
		m_texture	.Init();
	}
	void Uninit(void) {
		m_staticMesh   .Uninit();
		m_model		.Uninit();
		m_polygon2D	.Uninit();
		m_polygon3D	.Uninit();
		m_text2D	.Uninit();
		m_text3D	.Uninit();
		m_drawMgr	.Uninit();
		m_drawState	.Uninit();
		m_light3D	.Uninit();
		m_text		.Uninit();
		m_texture	.Uninit();
	}
	void Update(void) {
		m_staticMesh   .Update();
		m_model		.Update();
		m_polygon2D	.Update();
		m_polygon3D	.Update();
		m_text2D	.Update();
		m_text3D	.Update();
		m_drawMgr	.Update();
		m_drawState	.Update();
		m_light3D	.Update();
		m_text		.Update();
		m_texture	.Update();
	}

	//========== [[[ 変数宣言 ]]]
	_RNC_StaticMesh m_staticMesh;
	_RNC_Model      m_model;
	_RNC_Polygon2D  m_polygon2D;
	_RNC_Polygon3D  m_polygon3D;
	_RNC_Text2D     m_text2D;
	_RNC_Text3D     m_text3D;
	_RNC_CameraMgr  m_cameraMgr;
	_RNC_DrawMgr    m_drawMgr;
	_RNC_DrawState  m_drawState;
	_RNC_Light3D    m_light3D;
	_RNC_Text       m_text;
	_RNC_Texture    m_texture;
};

// 機械
class CMechanical {
public:
	//========== [[[ 変数宣言 ]]]
	void Init(HINSTANCE& instanceHandle) {
		m_count	.Init();
		m_file	.Init();
		m_input	.Init(instanceHandle);
		m_sound	.Init();
		m_window.Init();
	}
	void Uninit(void) {
		m_count	.Uninit();
		m_file	.Uninit();
		m_input	.Uninit();
		m_sound	.Uninit();
		m_window.Uninit();
	}
	void Update(void) {
		m_count	.Update();
		m_file	.Update();
		m_input	.Update();
		m_sound	.Update();
		m_window.Update();
	}

	//========== [[[ 変数宣言 ]]]
	_RNC_Count  m_count;
	_RNC_File   m_file;
	_RNC_Input  m_input;
	_RNC_Sound  m_sound;
	_RNC_Window m_window;
};

// その他
class COther {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {
		m_defaultData.Init();
		m_options    .Init();
	}
	void Uninit(void) {
		m_defaultData.Uninit();
		m_options    .Uninit();
	}
	void Update(void) {
		m_defaultData.Update();
		m_options    .Update();
	}

	//========== [[[ 変数宣言 ]]]
	_RNC_DefaultData m_defaultData;
	_RNC_Options     m_options;
};