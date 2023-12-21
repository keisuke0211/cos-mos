//========================================
// 
// ���[��3D�G�f�B�^�[�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../RNlib.h"
#include "../RNmode.h"
#include "rail3D-editor.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���[��3D�G�f�B�^�[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CRail3DEditor::CRail3DEditor() : m_camera(Scale2D(RNLib::Window().GetWidth(), RNLib::Window().GetHeight())), m_rail3D("NONEDATA"), m_editPath("NONEDATA"), m_doll((UShort)RNMode::PRIORITY::OBJECT3D, NONEDATA) {

	m_camera.SetGrabAirMouse(1.0f, 1.0f, 1.0f);
	m_camera.SetPosVAndPosR (Pos3D(0.0f, 100.0f, -100.0f), INITPOS3D);
	m_camera.SetLightID     ((new CLight3D("RNData\\StandardLight.txt"))->GetID());

	m_counter        = 0;
	m_time           = 60;
	m_modelIdx       = RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::PYRAMID_ALLOW);
	m_selectPointIdx = 0;
	m_isSaved        = true;
}

//========================================
// �f�X�g���N�^
//========================================
CRail3DEditor::~CRail3DEditor() {

}

//========================================
// ����������
//========================================
void CRail3DEditor::Init(void) {

}

//========================================
// �I������
//========================================
void CRail3DEditor::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CRail3DEditor::Update(void) {

	// [[[ �J�����`�� ]]]
	RNLib::Polygon2D().Put(0, RNLib::Window().GetCenterPos(), 0.0f, true)
		->SetTex(&m_camera)
		->SetSize(RNLib::Window().GetWidth(), RNLib::Window().GetHeight());

	// [[[ �O���b�h�`�� ]]]
	RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::STAGE3D, INITPOS3D, Rot3D(D3DX_PI_HALF, 0.0f, 0.0f))
		->SetSize(Size2D(90.0f, 90.0f))
		->SetTexUV(
			RNLib::DefaultData().GetTextureIdx(_RNC_DefaultData::TEXTURE::GRID),
			Pos2D(0.0f, 0.0f),
			Pos2D(9.0f, 0.0f),
			Pos2D(0.0f, 9.0f),
			Pos2D(9.0f, 9.0f));

	{// [[[ ���� ]]]
		// ���[��3D��V�����쐬
		if (RNLib::Input().GetKeyTrigger(DIK_1)) {
			if (!m_isSaved) {
				if (RNLib::Window().Message_OKCANCEL(String("�h�L�������g�̕ύX���ۑ�����Ă��܂���B\n�㏑�����Ă�낵���ł����H"), "�x��")) {
					m_rail3D.Clear();
					m_isSaved = true;
				}
			}
			else {
				m_rail3D.Clear();
				m_isSaved = true;
			}
		}

		// ���[��3D�̓ǂݍ���
		if (RNLib::Input().GetKeyTrigger(DIK_2)) {
			if (RNLib::File().GetSelectOpenFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_rail3D.Load(m_editPath);
				m_isSaved = true;
			}
		}

		// ���[��3D�̏�������
		if (RNLib::Input().GetKeyTrigger(DIK_3)) {
			if (RNLib::File().GetSelectSaveFileName(&m_editPath.GetCharPointer(), "", ".txt")) {
				m_rail3D.Save(m_editPath);
				m_isSaved = true;
			}
		}

		// ���f���̐ݒ�
		if (RNLib::Input().GetKeyTrigger(DIK_4)) {
			String modelPath;
			if (RNLib::File().GetSelectOpenFileName(&modelPath.GetCharPointer(), "", ".x")) {
				m_modelIdx = RNLib::Model().Load(modelPath);
			}
		}

		// �Z�b�g�A�b�v�̐ݒ�
		if (RNLib::Input().GetKeyTrigger(DIK_5)) {
			String setUpPath;
			if (RNLib::File().GetSelectOpenFileName(&setUpPath.GetCharPointer(), "", ".txt")) {
				short setUpIdx = RNLib::SetUp3D().Load(setUpPath);
				if (setUpIdx != NONEDATA) {
					m_doll.SetUp(setUpIdx);
					m_modelIdx = NONEDATA;
				}
			}
		}

		// ���Ԃ̉��Z/���Z
		if (RNLib::Count().GetCount() % 4 == 0) {
			if (RNLib::Input().GetKeyPress(DIK_T)) { m_time--; }
			if (RNLib::Input().GetKeyPress(DIK_Y)) { m_time++; }

			RNLib::Number().Clamp(&m_counter, SHRT_MAX, 0);
		}

		{// �X�P�[���̉��Z/���Z
			float& scale = m_rail3D.GetScale();

			if (RNLib::Input().GetKeyTrigger(DIK_G)) {
				if (scale <= 0.1f) {
					scale -= 0.01f;
				}
				else if (scale <= 1.0f) {
					scale -= 0.1f;
				}
				else {
					scale -= 1.0f;
				}
			}
			if (RNLib::Input().GetKeyTrigger(DIK_H)) {
				if (RNLib::Number().GetIsLs(scale, 0.1f)) {
					scale += 0.01f;
				}
				else if (scale < 1.0f) {
					scale += 0.1f;
				}
				else {
					scale += 1.0f;
				}
			}
			RNLib::Number().Clamp(&scale, FLT_MAX, 0.0f);
		}

		// �|�C���g�̃��[�v
		if (RNLib::Input().GetKeyTrigger(DIK_X)) {
			bool& isLoop = m_rail3D.GetIsLoop();
			isLoop = !isLoop;

			if (isLoop) {
				m_rail3D.AddPoint();
			}
			else {
				m_rail3D.SubPoint(m_rail3D.GetPointNum() - 1);
			}
		}

		// �|�C���g�̉��Z
		if (RNLib::Input().GetKeyTrigger(DIK_C)) {
			m_isSaved = false;
			m_rail3D.AddPoint();
			if (m_rail3D.GetIsLoop()) {
				m_rail3D.GetPoint(m_rail3D.GetPointNum() - 1) = m_rail3D.GetPoint(m_rail3D.GetPointNum() - 2);
				m_rail3D.GetPoint(m_rail3D.GetPointNum() - 2) = INITPOS3D;
				m_selectPointIdx = m_rail3D.GetPointNum() - 2;
			}
			else {
				m_selectPointIdx = m_rail3D.GetPointNum() - 1;
			}
		}

		// �|�C���g�̌��Z
		if (RNLib::Input().GetKeyTrigger(DIK_Z) && m_rail3D.GetPointNum() > 0) {
			if (RNLib::Window().Message_OKCANCEL(String("���j�A���C�g[%d]���폜���܂��B��낵���ł����H", m_selectPointIdx), "�x��")) {
				m_isSaved = false;
				m_rail3D.SubPoint(m_selectPointIdx);
			}
		}

		if (m_rail3D.GetPointNum() > 0) {

			// �|�C���g�̑I��
			if (RNLib::Input().GetKeyTrigger(DIK_Q)) { m_selectPointIdx--; }
			if (RNLib::Input().GetKeyTrigger(DIK_E)) { m_selectPointIdx++; }
			RNLib::Number().LoopClamp(&m_selectPointIdx, m_rail3D.GetPointNum() - 1 - m_rail3D.GetIsLoop(), 0);

			{// �|�C���g�̈ړ�
				Pos3D& point = m_rail3D.GetPoint(m_selectPointIdx);
				const float cameraAngle = m_camera.GetRot().y;
				float XZMoveAngle = 0.0f;
				bool  isXZMove    = true;

				if (RNLib::Input().GetKeyPress(DIK_W)) {
					if      (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.25f; }
					else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.25f;  }
					else                                        { XZMoveAngle = 0.0f;             }
				}
				else if (RNLib::Input().GetKeyPress(DIK_S)) {
					if      (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.75f; }
					else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.75f;  }
					else                                        { XZMoveAngle = D3DX_PI;          }
				}
				else if (RNLib::Input().GetKeyPress(DIK_A)) { XZMoveAngle = D3DX_PI * -0.5f; }
				else if (RNLib::Input().GetKeyPress(DIK_D)) { XZMoveAngle = D3DX_PI * 0.5f;  }
				else { isXZMove = false; }

				if (isXZMove) {
					point.x += (sinf(cameraAngle + XZMoveAngle) * 0.5f);
					point.z += (cosf(cameraAngle + XZMoveAngle) * 0.5f);
					m_isSaved = false;
				}

				if (RNLib::Input().GetKeyPress(DIK_R)) { point.y += 0.5f; m_isSaved = false; }
				if (RNLib::Input().GetKeyPress(DIK_F)) { point.y -= 0.5f; m_isSaved = false; }
			}

			// ���[�v���A�n�_�ƏI�_�̒Ǐ]
			if (m_rail3D.GetIsLoop()) {
				m_rail3D.GetPoint(m_rail3D.GetPointNum() - 1) = m_rail3D.GetPoint(0);
			}
		}
	}

	// ���f���̕`��
	if (m_modelIdx != NONEDATA) {
		
		// �h�[���͔�\��
		m_doll.SetIsShow(false);

		RNLib::Model().Put((UShort)RNMode::PRIORITY::OBJECT3D, m_modelIdx, m_rail3D.GetMtx((float)m_counter / m_time, false))
			->SetCol(m_modelIdx == RNLib::DefaultData().GetModelIdx(_RNC_DefaultData::MODEL::PYRAMID_ALLOW) ? Color(255, 0, 0, 255) : COLOR_WHITE);
	}
	else {

		// �h�[����\��
		m_doll.SetIsShow(true);

		Matrix mtx = m_rail3D.GetMtx((float)m_counter / m_time, false);
		m_doll.SetPos(RNLib::Matrix().ConvMtxToPos(mtx));
		m_doll.SetRot(RNLib::Matrix().ConvMtxToRot(mtx));
	}

	if (m_rail3D.GetPointNum() >= 2)
	{// ���C���̕`��
		Matrix mtx      = m_rail3D.GetMtx(0.0f, false);
		Pos3D  oldPos   = RNLib::Matrix().ConvMtxToPos(mtx);
		float  oldAngle = RNLib::Matrix().ConvMtxToRot(mtx).y;

		const UShort divNum = (m_rail3D.GetPointNum() - 1) * 5;
		for (int cntDiv = 1; cntDiv <= divNum; cntDiv++) {
			mtx = m_rail3D.GetMtx((float)cntDiv / divNum, false);
			Pos3D pos = RNLib::Matrix().ConvMtxToPos(mtx);
			float angle = RNLib::Matrix().ConvMtxToRot(mtx).y;
			RNLib::Polygon3D().Put((UShort)RNMode::PRIORITY::UI3D, INITMATRIX)
				->SetVtxPos(
					Pos3D(pos.x + sinf(angle - D3DX_PI_HALF), pos.y, pos.z + cosf(angle - D3DX_PI_HALF)),
					Pos3D(pos.x + sinf(angle + D3DX_PI_HALF), pos.y, pos.z + cosf(angle + D3DX_PI_HALF)),
					Pos3D(oldPos.x + sinf(oldAngle - D3DX_PI_HALF), oldPos.y, oldPos.z + cosf(oldAngle - D3DX_PI_HALF)),
					Pos3D(oldPos.x + sinf(oldAngle + D3DX_PI_HALF), oldPos.y, oldPos.z + cosf(oldAngle + D3DX_PI_HALF)))
				->SetCol(Color(255, 0, 0, 100))
				->SetCullingMode(_RNC_DrawState::CULLING_MODE::BOTH_SIDES);

			oldPos = pos;
			oldAngle = angle;
		}
	}

	// �|�C���g�̕`��
	for (UShort cntPoint = 0; cntPoint < m_rail3D.GetPointNum() - m_rail3D.GetIsLoop(); cntPoint++) {
		RNLib::Text3D().Put((UShort)RNMode::PRIORITY::UI3D, String("%d", cntPoint), _RNC_Text::ALIGNMENT::CENTER, NONEDATA, RNLib::Matrix().ConvPosToMtx(m_rail3D.GetPoint(cntPoint)))
			->SetSize(Size2D(4.0f, 4.0f))
			->SetCol(cntPoint == m_selectPointIdx ? Color(255, 255, ((RNLib::Count().GetCount() % 30) / 30.0f) * 255, 255) : COLOR_WHITE)
			->SetZTest(false)
			->SetBillboard(true);
	}

	// �J�E���^�[����
	m_counter++;
	RNLib::Number().LoopClamp(&m_counter, m_time, 0);
	
	// [[[ ���O�\�� ]]]
	RNLib::Text2D().PutDebugLog(String("-----Information"));
	RNLib::Text2D().PutDebugLog(String("EditPath:"));
	RNLib::Text2D().PutDebugLog(String("Counter :%d/%d %d%%", m_counter, m_time, (int)(((float)m_counter / m_time) * 100.0f)));
	RNLib::Text2D().PutDebugLog(String("-----Control"));
	RNLib::Text2D().PutDebugLog(String("Rail3DCreateNew   [1]"));
	RNLib::Text2D().PutDebugLog(String("Rail3DLoad        [2]"));
	RNLib::Text2D().PutDebugLog(String("Rail3DSave        [3]"));
	RNLib::Text2D().PutDebugLog(String("SetModel          [4]"));
	RNLib::Text2D().PutDebugLog(String("SetDoll           [5]"));
	RNLib::Text2D().PutDebugLog(String("Time              [T][Y]:%d", m_time));
	RNLib::Text2D().PutDebugLog(String("scale             [G][H]:%.2f", m_rail3D.GetScale()));
	RNLib::Text2D().PutDebugLog(String("IsLoop            [X]   :%s", m_rail3D.GetIsLoop() ? "TRUE" : "FALSE"));
	RNLib::Text2D().PutDebugLog(String("AddPointNum       [C]   :%d", m_rail3D.GetPointNum()));
	RNLib::Text2D().PutDebugLog(String("SubPoint          [Z]"));
	if (m_rail3D.GetPointNum() > 0) {
		RNLib::Text2D().PutDebugLog(String("SelectPointIdx    [Q][E]:%d", 0));
		RNLib::Text2D().PutDebugLog(String("SelectPointPos[WASD][RF]:%f %f %f",
			m_rail3D.GetPoint(m_selectPointIdx).x,
			m_rail3D.GetPoint(m_selectPointIdx).y,
			m_rail3D.GetPoint(m_selectPointIdx).z));
	}
}