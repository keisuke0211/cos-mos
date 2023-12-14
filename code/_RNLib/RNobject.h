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
	CMotion3D         m_motion3D;
	CSetUp3D          m_setUp3D;
	CDoll3DMgr        m_doll3DMgr;
	CEffect3D         m_effect3D;
	CEffect3DMgr      m_effect3DMgr;
	CStandardEffect3D m_standardEffect3D;
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
	}
	void Uninit(void) {
		m_ease	  .Uninit();
		m_geometry.Uninit();
		m_hitTest .Uninit();
		m_matrix  .Uninit();
	}
	void Update(void) {
		m_ease	  .Update();
		m_geometry.Update();
		m_hitTest .Update();
		m_matrix  .Update();
	}

	//========== [[[ 変数宣言 ]]]
	CEase     m_ease;
	CGeometry m_geometry;
	CHitTest  m_hitTest;
	CMatrix   m_matrix;
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
	CStaticMesh m_staticMesh;
	CModel      m_model;
	CPolygon2D  m_polygon2D;
	CPolygon3D  m_polygon3D;
	CText2D     m_text2D;
	CText3D     m_text3D;
	CCameraMgr  m_cameraMgr;
	CDrawMgr    m_drawMgr;
	CDrawState  m_drawState;
	CLight3D    m_light3D;
	CText       m_text;
	CTexture    m_texture;
};

// 機械
class CMechanical {
public:
	//========== [[[ 変数宣言 ]]]
	void Init(HINSTANCE& instanceHandle) {
		m_count	.Init();
		m_file	.Init();
		m_input	.Init(instanceHandle);
		m_memory.Init();
		m_sound	.Init();
		m_window.Init();
	}
	void Uninit(void) {
		m_count	.Uninit();
		m_file	.Uninit();
		m_input	.Uninit();
		m_memory.Uninit();
		m_sound	.Uninit();
		m_window.Uninit();
	}
	void Update(void) {
		m_count	.Update();
		m_file	.Update();
		m_input	.Update();
		m_memory.Update();
		m_sound	.Update();
		m_window.Update();
	}

	//========== [[[ 変数宣言 ]]]
	CCount  m_count;
	CFile   m_file;
	CInput  m_input;
	CMemory m_memory;
	CSound  m_sound;
	CWindow m_window;
};

// その他
class COther {
public:
	//========== [[[ 関数宣言 ]]]
	void Init(void) {
		m_defaultData.Init();
	}
	void Uninit(void) {
		m_defaultData.Uninit();
	}
	void Update(void) {
		m_defaultData.Update();
	}

	//========== [[[ 変数宣言 ]]]
	CDefaultData m_defaultData;
};