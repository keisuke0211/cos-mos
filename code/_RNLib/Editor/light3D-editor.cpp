//========================================
// 
// ���C�g3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "light3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���C�g3D�G�f�B�^�[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CLight3DEditor::CLight3DEditor() : m_editPath("NONEDATA") {

	m_light = new CLight3D(NULL);

	m_camera = new CCamera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight()));
	m_camera->SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera->SetPosVAndPosR(Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera->SetLightID(m_light->GetID());

	m_selectLinearLightIdx = 0;
	m_isSaved              = true;
}

//========================================
// �f�X�g���N�^
//========================================
CLight3DEditor::~CLight3DEditor() {

}

//========================================
// ����������
//========================================
void CLight3DEditor::Init(void) {

	// ���f���ݒ�
	RNLib::StaticMesh().SetModel((UShort)RNMode::PRIORITY::OBJECT3D, Pos3D(0.0f, 5.0f, 0.0f), Rot3D(0.0f, 0.0f, 0.0f), RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::CUBE), COLOR_WHITE);
	RNLib::StaticMesh().SetModel((UShort)RNMode::PRIORITY::OBJECT3D, Pos3D(0.0f, 15.0f, 0.0f), Rot3D(0.0f, 0.0f, 0.0f), RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::SPHERE), COLOR_WHITE);
}

//========================================
// �I������
//========================================
void CLight3DEditor::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CLight3DEditor::Update(void) {

	{// [[[ ���� ]]]
		// ���C�g3D��V�����쐬
		if (RNLib::Input().GetKeyPress(DIK_1)) {
			if (!m_isSaved) {
				if (RNLib::Window().Message_OKCANCEL(String("�h�L�������g�̕ύX���ۑ�����Ă��܂���B\n�㏑�����Ă�낵���ł����H"), "�x��")) {
					m_light->Clear();
					m_isSaved = true;
				}
			}
			else {
				m_light->Clear();
				m_isSaved = true;
			}
		}

		// ���C�g3D�̓ǂݍ���
		if (RNLib::Input().GetKeyPress(DIK_2)) {
			if (RNLib::File().GetSelectOpenFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_light->Load(m_editPath);
				m_isSaved = true;
			}
		}

		// ���C�g3D�̏�������
		if (RNLib::Input().GetKeyPress(DIK_3)) {
			if (RNLib::File().GetSelectSaveFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_light->Save(m_editPath);
				m_isSaved = true;
			}
		}

		// ���C�g3D�̌������Z�b�g
		if (RNLib::Input().GetKeyPress(DIK_P)) {
			m_light->SetRot(INITROT3D);
		}
		else
		{// ���C�g3D�̉�]
			Rot3D& lightRot = m_light->GetRot();
			if (RNLib::Input().GetKeyPress(DIK_UP   ))lightRot.x += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_DOWN ))lightRot.x -= 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_LEFT ))lightRot.y += 0.02f;
			if (RNLib::Input().GetKeyPress(DIK_RIGHT))lightRot.y -= 0.02f;
			RNLib::Number().LoopClamp(&lightRot.x, D3DX_PI, -D3DX_PI);
			RNLib::Number().LoopClamp(&lightRot.y, D3DX_PI, -D3DX_PI);
		}

		if (RNLib::Count().GetCount() % 3 == 0)
		{// �w�i�̐F
			if (RNLib::Input().GetKeyPress(DIK_Y)) { m_light->GetBGCol().r--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_U)) { m_light->GetBGCol().r++; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_H)) { m_light->GetBGCol().g--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_J)) { m_light->GetBGCol().g++; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_N)) { m_light->GetBGCol().b--; m_isSaved = false; }
			if (RNLib::Input().GetKeyPress(DIK_M)) { m_light->GetBGCol().b++; m_isSaved = false; }

			// ����
			RNLib::Number().LoopClamp(&m_light->GetBGCol().r, 255, 0);
			RNLib::Number().LoopClamp(&m_light->GetBGCol().g, 255, 0);
			RNLib::Number().LoopClamp(&m_light->GetBGCol().b, 255, 0);
		}

		// �t�H�O��ON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_X)) {
			m_isSaved = false;
			m_light->SetIsFog(!m_light->GetIsFog());
		}

		// �t�H�O�̃X�^�[�g/�G���h
		if (RNLib::Input().GetKeyPress(DIK_I) && m_light->GetFogInfo().start > 0) { m_light->GetFogInfo().start--; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_O))                                    { m_light->GetFogInfo().start++; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_K) && m_light->GetFogInfo().end > 0)   { m_light->GetFogInfo().end--  ; m_isSaved = false; }
		if (RNLib::Input().GetKeyPress(DIK_L))                                    { m_light->GetFogInfo().end++  ; m_isSaved = false; }
		RNLib::Number().Clamp(&m_light->GetFogInfo().start, USHRT_MAX, 0);
		RNLib::Number().Clamp(&m_light->GetFogInfo().end, USHRT_MAX, m_light->GetFogInfo().start);

		// ���j�A���C�g�̉��Z
		if (RNLib::Input().GetKeyTrigger(DIK_C)) {
			m_isSaved = false;
			m_light->AddLinearLight();
		}

		// ���j�A���C�g�̌��Z
		if (RNLib::Input().GetKeyTrigger(DIK_Z) && m_light->GetLinearLightNum() > 0) {
			if (RNLib::Window().Message_OKCANCEL(String("���j�A���C�g[%d]���폜���܂��B��낵���ł����H", m_selectLinearLightIdx), "�x��")) {
				m_isSaved = false;
				m_light->SubLinearLight(m_selectLinearLightIdx);
			}
		}

		// �I�����j�A���C�g�̉��Z
		if (RNLib::Input().GetKeyTrigger(DIK_Q)) {
			m_selectLinearLightIdx--;
		}

		// �I�����j�A���C�g�̌��Z
		if (RNLib::Input().GetKeyTrigger(DIK_E)) {
			m_selectLinearLightIdx++;
		}

		if (m_light->GetLinearLightNum() > 0) {
		
			// �I�����j�A���C�g�̐���
			RNLib::Number().LoopClamp(&m_selectLinearLightIdx, m_light->GetLinearLightNum() - 1, 0);

			if (RNLib::Count().GetCount() % 3 == 0)
			{// �I�����j�A���C�g�̐F
				if (RNLib::Input().GetKeyPress(DIK_R)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.r--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_T)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.r++; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_F)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.g--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_G)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.g++; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_V)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.b--; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_B)) { m_light->GetLinearLight(m_selectLinearLightIdx).col.b++; m_isSaved = false; }

				// ����
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.r, 255, 0);
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.g, 255, 0);
				RNLib::Number().LoopClamp(&m_light->GetLinearLight(m_selectLinearLightIdx).col.b, 255, 0);
			}

			{// �I�����j�A���C�g�̉�]
				Rot3D lightRot = RNLib::Matrix().ConvMtxToRot(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx);
				bool isSpin = false;
				if (RNLib::Input().GetKeyPress(DIK_W)) { lightRot.x += 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_S)) { lightRot.x -= 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_A)) { lightRot.y += 0.02f; isSpin = true; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_D)) { lightRot.y -= 0.02f; isSpin = true; m_isSaved = false; }
				RNLib::Number().LoopClamp(&lightRot.x, D3DX_PI, -D3DX_PI);
				RNLib::Number().LoopClamp(&lightRot.y, D3DX_PI, -D3DX_PI);
				m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx = RNLib::Matrix().ConvRotToMtx(lightRot);
			}
		}
		else {
			m_selectLinearLightIdx = 0;
		}
	}

	// [[[ �J�����`�� ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(m_camera)
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ �O���b�h�`�� ]]]
	if (m_light->GetIsFog()) {
		RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, Pos3D(0.0f, -50.0f, 0.0f), Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
			->SetSize(Size2D(100000.0f, 100000.0f))
			->SetCol(Color(0, 0, 0, 255));
	}
	RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, INITPOS3D, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
		->SetSize(Size2D(90.0f, 90.0f))
		->SetTexUV(
			RNLib::DefaultData().GetTextureIdx(_RNC_DefaultData::TEXTURE::GRID),
			Pos2D(0.0f, 0.0f),
			Pos2D(9.0f, 0.0f),
			Pos2D(0.0f, 9.0f),
			Pos2D(9.0f, 9.0f));
	
	{
		const float  dist = 50.0f;
		const Pos3D  lightRot = m_light->GetRot();
		const Pos3D  lightPos = -RNLib::Geometry().FindRotVec(lightRot) * dist;
		const Matrix lightMtx = RNLib::Matrix().ConvPosRotToMtx(lightPos, lightRot);

		// [[[ ���C�g3D�`�� ]]]
		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::SPHERE), lightPos, lightRot, Scale3D(0.25f, 0.25f, 0.25f))
			->SetLighting(false);

		// [[[ ���j�A���C�g�`�� ]]]
		for (int cntLinearLight = 0; cntLinearLight < m_light->GetLinearLightNum(); cntLinearLight++) {
			RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::LINEAR_LIGHT), RNLib::Matrix().MultiplyMtx(m_light->GetLinearLight(cntLinearLight).rotMtx, lightMtx))
				->SetCol(m_light->GetLinearLight(cntLinearLight).col.GetAlphaApplied(m_selectLinearLightIdx == cntLinearLight ? 1.0f : 0.5f));
		}
	}

	// [[[ ���O�\�� ]]]
	RNLib::Text2D().PutDebugLog(String("-----Information"));
	RNLib::Text2D().PutDebugLog(String("EditPath:"));
	RNLib::Text2D().PutDebugLog(String("%s", m_editPath));
	RNLib::Text2D().PutDebugLog(String("-----Control-----"));
	RNLib::Text2D().PutDebugLog(String("Light3DCreateNew    [1]"));
	RNLib::Text2D().PutDebugLog(String("Light3DLoad         [2]"));
	RNLib::Text2D().PutDebugLog(String("Light3DSave         [3]"));
	RNLib::Text2D().PutDebugLog(String("Light3DRot          [DIR]       :%.2fPI,%.2fPI", m_light->GetRot().x / D3DX_PI, m_light->GetRot().y / D3DX_PI));
	RNLib::Text2D().PutDebugLog(String("Light3DRotReset     [P]"));
	RNLib::Text2D().PutDebugLog(String("BackGroundCol       [YU][HJ][NM]:%d %d %d",
		m_light->GetBGCol().r,
		m_light->GetBGCol().g,
		m_light->GetBGCol().b));
	RNLib::Text2D().PutDebugLog(String("IsFog               [X]         :%s", m_light->GetIsFog() ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(String("FogStart            [I][O]      :%d", m_light->GetFogInfo().start));
	RNLib::Text2D().PutDebugLog(String("FogEnd              [K][L]      :%d", m_light->GetFogInfo().end));
	RNLib::Text2D().PutDebugLog(String("AddLinearLightNum   [C]         :%d", m_light->GetLinearLightNum()));
	RNLib::Text2D().PutDebugLog(String("SubLinearLight      [Z]"));
	if (m_light->GetLinearLightNum() > 0) {
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightIdx[Q][E]      :%d", m_selectLinearLightIdx));
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightCol[RT][FG][VB]:%d %d %d",
			m_light->GetLinearLight(m_selectLinearLightIdx).col.r,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.g,
			m_light->GetLinearLight(m_selectLinearLightIdx).col.b));
		const Rot3D rot = RNLib::Matrix().ConvMtxToRot(m_light->GetLinearLight(m_selectLinearLightIdx).rotMtx);
		RNLib::Text2D().PutDebugLog(String("SelectLinearLightRot[WASD]      :%.2fPI,%.2fPI", rot.x / D3DX_PI, rot.y / D3DX_PI));
	}
}