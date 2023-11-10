//========================================
// 
// RN�I�u�W�F�N�g�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "RNlib.h"
#include "RNsettings.h"

//****************************************
// �N���X��`
//****************************************
// 3D�I�u�W�F�N�g
class C3DObject {
public:
	//========== [[[ �֐��錾 ]]]
	void Init(void) {
		m_motion3D.Init();
		m_setUp3D .Init();
	}
	void Uninit(void) {
		m_motion3D.Uninit();
		m_setUp3D .Uninit();
	}
	void Update(void) {
		m_motion3D.Update();
		m_setUp3D .Update();
	}

	//========== [[[ �ϐ��錾 ]]]
	CMotion3D m_motion3D;
	CSetUp3D  m_setUp3D;
};

// �v�Z
class CCalculation {
public:
	//========== [[[ �֐��錾 ]]]
	void Init(void) {
		m_ease	  .Init();
		m_geometry.Init();
		m_matrix  .Init();
	}
	void Uninit(void) {
		m_ease	  .Uninit();
		m_geometry.Uninit();
		m_matrix  .Uninit();
	}
	void Update(void) {
		m_ease	  .Update();
		m_geometry.Update();
		m_matrix  .Update();
	}

	//========== [[[ �ϐ��錾 ]]]
	CEase     m_ease;
	CGeometry m_geometry;
	CMatrix   m_matrix;
};

// �`��
class CDraw {
public:
	//========== [[[ �֐��錾 ]]]
	void Init(Device& device) {
		m_model		.Init();
		m_polygon2D	.Init();
		m_polygon3D	.Init();
		m_text2D	.Init();
		m_text3D	.Init();
		m_cameraMgr	.Init();
		m_drawMgr	.Init();
		m_drawState	.Init(device);
		m_light3D	.Init();
		m_text		.Init();
		m_texture	.Init();
		m_transition.Init();
	}
	void Uninit(void) {
		m_model		.Uninit();
		m_polygon2D	.Uninit();
		m_polygon3D	.Uninit();
		m_text2D	.Uninit();
		m_text3D	.Uninit();
		m_cameraMgr	.Uninit();
		m_drawMgr	.Uninit();
		m_drawState	.Uninit();
		m_light3D	.Uninit();
		m_text		.Uninit();
		m_texture	.Uninit();
		m_transition.Uninit();
	}
	void Update(void) {
		m_model		.Update();
		m_polygon2D	.Update();
		m_polygon3D	.Update();
		m_text2D	.Update();
		m_text3D	.Update();
		m_cameraMgr	.Update();
		m_drawMgr	.Update();
		m_drawState	.Update();
		m_light3D	.Update();
		m_text		.Update();
		m_texture	.Update();
		m_transition.Update();
	}

	//========== [[[ �ϐ��錾 ]]]
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
	CTransition m_transition;
};

// �@�B
class CMechanical {
public:
	//========== [[[ �ϐ��錾 ]]]
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

	//========== [[[ �ϐ��錾 ]]]
	CCount  m_count;
	CFile   m_file;
	CInput  m_input;
	CMemory m_memory;
	CSound  m_sound;
	CWindow m_window;
};

// ���̑�
class COther {
public:
	//========== [[[ �֐��錾 ]]]
	void Init(void) {
		m_defaultData.Init();
	}
	void Uninit(void) {
		m_defaultData.Uninit();
	}
	void Update(void) {
		m_defaultData.Update();
	}

	//========== [[[ �ϐ��錾 ]]]
	CDefaultData m_defaultData;
};