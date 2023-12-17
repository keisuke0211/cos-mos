//========================================
// 
// ���C�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���C�g3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
short  CLight3D::ms_IDCount           = 0;
UShort CLight3D::ms_linearLightNumOld = 0;

//========================================
// [�ÓI]�S�Ė����ɂ���
//========================================
void CLight3D::AllDisable(Device& device) {

	// ����S�Ė����ɂ���
	for (int cntLinearLight = 0; cntLinearLight < ms_linearLightNumOld; device->LightEnable(cntLinearLight, FALSE), cntLinearLight++);
	ms_linearLightNumOld = 0;
}

//========================================
// �R���X�g���N�^
//========================================
CLight3D::CLight3D(const String& loadPath) {

	// ���X�g�ɒǉ�
	RNSystem::GetLight3DMgr().AddList(this);

	// ID��ݒ�
	m_ID       = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % SHRT_MAX;

	// �N���A����
	m_linearLights = NULL;
	Clear();

	// �ǂݍ��ݏ���
	if (loadPath != NULL)
		Load(loadPath);
}

//========================================
// �f�X�g���N�^
//========================================
CLight3D::~CLight3D() {

	// �N���A����
	Clear();

	// ���X�g����폜
	RNSystem::GetLight3DMgr().SubList(this);
}

//========================================
// �N���A����
//========================================
void CLight3D::Clear(void) {

	// ���j�A���C�g�����
	RNLib::Memory().Release(&m_linearLights);
	m_linearLightNum = 0;
	m_rot            = INITROT3D;
	m_col            = COLOR_WHITE;
}

//========================================
// �ǂݍ��ݏ���
//========================================
void CLight3D::Load(const String& loadPath) {

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(loadPath, "Light3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			if (RNLib::File().CheckIdentifier("linearLights{")) {
				RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_linearLightNum);
				RNLib::Memory().Alloc(&m_linearLights, m_linearLightNum);

				UShort linearLightCount = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("linearLight{")) {
						Rot3D rot = INITROT3D;

						while (RNLib::File().SearchLoop("}")) {
							RNLib::File().Scan(_RNC_File::SCAN::ROT3D, &rot, "rot");
							RNLib::File().Scan(_RNC_File::SCAN::COLOR, &m_linearLights[linearLightCount].col, "col");
						}
						m_linearLights[linearLightCount].rotMtx = RNLib::Matrix().ConvRotToMtx(rot);

						linearLightCount++;
					}
				}
			}
		}

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// �������ݏ���
//========================================
void CLight3D::Save(const String& savePath) {

	// �t�@�C�����J��
	if (RNLib::File().OpenSaveFile(savePath)) {
		fprintf(RNLib::File().GetFile(), "Light3DFile\n");
		fprintf(RNLib::File().GetFile(), "linearLights{ %d\n", m_linearLightNum);
		for (int cntLinearLight = 0; cntLinearLight < m_linearLightNum; cntLinearLight++) {
			fprintf(RNLib::File().GetFile(), "	linearLight{\n");
			const Rot3D rot = RNLib::Matrix().ConvMtxToRot(m_linearLights[cntLinearLight].rotMtx);
			fprintf(RNLib::File().GetFile(), "		rot %f %f %f\n", rot.x, rot.y, rot.z);
			fprintf(RNLib::File().GetFile(), "		col %d %d %d %d\n", m_linearLights[cntLinearLight].col.r, m_linearLights[cntLinearLight].col.g, m_linearLights[cntLinearLight].col.b, m_linearLights[cntLinearLight].col.a);
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END");

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// �Z�b�e�B���O����
//========================================
void CLight3D::Setting(Device& device) {

	{// ���j�A���C�g�̐ݒ�
		// �����g�p����Ȃ�����S�Ė����ɂ���
		for (int cntLinearLight = m_linearLightNum; cntLinearLight < ms_linearLightNumOld; device->LightEnable(cntLinearLight, FALSE), cntLinearLight++);

		// ��3D�̏����N���A���A��ނ�ݒ�
		D3DLIGHT9 light3D;
		ZeroMemory(&light3D, sizeof(D3DLIGHT9));
		light3D.Type = D3DLIGHT_DIRECTIONAL;

		// �����}�g���b�N�X���Z�o
		const Matrix rotMtx = RNLib::Matrix().ConvRotToMtx(m_rot);

		for (UShort cntLinearLight = 0; cntLinearLight < m_linearLightNum; cntLinearLight++) {

			// �g�U���ƕ�����ݒ�
			light3D.Diffuse   = m_linearLights[cntLinearLight].col.GetMixed(m_col);
			light3D.Direction = RNLib::Matrix().ConvMtxToRotVec(RNLib::Matrix().MultiplyMtx(m_linearLights[cntLinearLight].rotMtx, rotMtx));

			// ���̏���ݒ肵�A�L���ɂ���
			device->SetLight(cntLinearLight, &light3D);
			device->LightEnable(cntLinearLight, TRUE);
		}

		// �ߋ��̐��Ƃ��ĕۑ����Ă���
		ms_linearLightNumOld = m_linearLightNum;
	}
}

//========================================
// ���j�A���C�g�����Z
//========================================
void CLight3D::AddLinearLight(void) {

	const UShort oldNum = m_linearLightNum++;
	RNLib::Memory().ReAlloc(&m_linearLights, oldNum, m_linearLightNum);
	m_linearLights[oldNum] = {};
	m_linearLights[oldNum].rotMtx = RNLib::Matrix().ConvRotToMtx(INITROT3D);
}

//========================================
// ���j�A���C�g�����Z
//========================================
void CLight3D::SubLinearLight(const UShort& idx) {

	if (idx < 0 || idx >= m_linearLightNum)
		return;

	for (UShort cntLinearLight = idx; cntLinearLight < m_linearLightNum - 1; cntLinearLight++) {
		m_linearLights[cntLinearLight] = m_linearLights[cntLinearLight + 1];
	}
	const UShort oldNum = m_linearLightNum--;
	RNLib::Memory().ReAlloc(&m_linearLights, oldNum, m_linearLightNum);
}