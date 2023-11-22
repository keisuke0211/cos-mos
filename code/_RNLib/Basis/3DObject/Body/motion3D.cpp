//========================================
// 
// ���[�V����3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���[�V����3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �萔��`
//****************************************
const UShort CMotion3D::COMMAND_DATA_NUM[] = {
	5,	// �ړ� ( �ڕW�ʒuX,Y,Z,�ړ�����,��Ԕԍ� )
	5,	// ��] ( �ڕW����X,Y,Z,��]����,��Ԕԍ� )
	5,	// �g�k ( �g��{��X,Y,Z.�g�k����,��Ԕԍ� )
	0,	// ����
};

//========================================
// �R���X�g���N�^
//========================================
CMotion3D::CMotion3D() {

	m_datas    = NULL;
	m_editData = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CMotion3D::~CMotion3D() {

}

//========================================
// ����������
//========================================
void CMotion3D::Init(void) {

}

//========================================
// �I������
//========================================
void CMotion3D::Uninit(void) {

	// �f�[�^�̉��
	CMemory::ReleaseDouble(&m_datas, m_num);

	// �G�f�B�b�g�f�[�^�̉��
	CMemory::Release(&m_editData);
}

//========================================
// �X�V����
//========================================
void CMotion3D::Update(void) {

}

//========================================
// �ǂݍ��ݏ���
//========================================
short CMotion3D::Load(const char* loadPath, short idx) {

	const UShort numOld = m_num;

	if (CRegist::Load(loadPath, idx))
	{// �ǂݍ��ݐ���
		// �f�[�^�̃������Ċm��
		CMemory::ReAllocDouble(&m_datas, numOld, m_num);

		// �f�[�^�̔j��(�ԍ��w��̏ꍇ)
		if (idx != NONEDATA)
			m_datas[idx]->Release();

		// �ǂݍ��ݎ��s
		if (!ExecutionLoad(loadPath, *m_datas[idx]))
		{// ���s�������A
			// �ǂݍ��ݍσp�X�̃��������Z�b�g
			ReAllocLoadPath(numOld);

			return NONEDATA;
		}
	}

	return idx;
}

//========================================
// �ǂݍ��ݏ���(�G�f�B�b�g�f�[�^)
//========================================
bool CMotion3D::LoadEditData(const char* loadPath) {

	CData* loadData = NULL;
	CMemory::Alloc(&loadData);

	// �ǂݍ��ݎ��s
	if (!ExecutionLoad(loadPath, *loadData)) {
		CMemory::Release(&loadData);
		return false;
	}

	// �G�f�B�b�g�f�[�^�����
	CMemory::Release(&m_editData);

	// �G�f�B�b�g�f�[�^���㏑��
	m_editData = loadData;

	// �ăZ�b�g�A�b�v
	CDoll3D* doll = RNLib::Doll3DMgr().GetEditDoll();
	if (doll != NULL)
		doll->SetMotion(EDITDATA);

	return true;
}

//========================================
// �����o������
//========================================
void CMotion3D::SaveEditData(const char* savePath) {

	if (m_editData == NULL) {
		RNLib::Window().Message_ERROR(CreateText("���[�V����3D�f�[�^�����݂��܂���B"));
		return;
	}

	// �t�@�C�����J��
	if (RNLib::File().OpenSaveFile(savePath)) {
		fprintf(RNLib::File().GetFile(), "Motion3DFile\n");
		fprintf(RNLib::File().GetFile(), "loopTime %d\n", m_editData->loopTime);
		fprintf(RNLib::File().GetFile(), "isLoop %d\n", (int)m_editData->isLoop);
		fprintf(RNLib::File().GetFile(), "boneMotionDatas{ %d\n", m_editData->boneNum);

		// �{�[�����[�V�����f�[�^�̏����o��
		for (int cntBone = 0; cntBone < m_editData->boneNum; cntBone++) {
			BoneMotionData& boneMotionData = m_editData->boneMotionDatas[cntBone];

			fprintf(RNLib::File().GetFile(), "	boneMotionData{ %d\n", boneMotionData.commandDataNum);

			// �R�}���h�f�[�^�����o��
			for (int cntCmd = 0; cntCmd < boneMotionData.commandDataNum; cntCmd++) {
				CommandData& cmd = boneMotionData.commandDatas[cntCmd];

				fprintf(RNLib::File().GetFile(), "		time %d ", cmd.time);
				switch (cmd.command) {
				case COMMAND::MOVE: {
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::MOVE, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
				}break;
				case COMMAND::SPIN: {
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::SPIN, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
				}break;
				case COMMAND::SCALING: {
					fprintf(RNLib::File().GetFile(), "%d %f %f %f %d %d\n",
						(int)COMMAND::SCALING, cmd.datas[0], cmd.datas[1], cmd.datas[2], (int)cmd.datas[3], (int)cmd.datas[4]);
				}break;
				case COMMAND::STEP: {
					fprintf(RNLib::File().GetFile(), "%d %d\n",
						(int)COMMAND::STEP, (int)cmd.datas[0]);
				}break;
				default: {
					// �G���[���b�Z�[�W
					RNLib::Window().Message_ERROR(CreateText("�R�}���h���w�肳��Ă��܂���B\n%s", savePath));
				}break;
				}
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END\n");

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// ���������w�萔�ɏ�����
//========================================
void CMotion3D::InitMemory(const UShort& num) {
	CRegist::InitMemory(num);

	// �f�[�^�̃������m��
	CMemory::Alloc(&m_datas, num);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]���[�V����3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ǂݍ��ݎ��s����
//========================================
bool CMotion3D::ExecutionLoad(const char* loadPath, CData& data) {

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(loadPath, "Motion3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			RNLib::File().Scan(CFile::SCAN::SHORT, &data.loopTime, "loopTime");
			RNLib::File().Scan(CFile::SCAN::BOOL, &data.isLoop, "isLoop");

			// �{�[�����[�V�����f�[�^��ǂݍ���
			if (RNLib::File().CheckIdentifier("boneMotionDatas{")) {

				// �{�[���̐���ǂݍ��݁A
				// �{�[�����[�V�����f�[�^�̃������m��
				RNLib::File().Scan(CFile::SCAN::USHORT, &data.boneNum);
				CMemory::Alloc<BoneMotionData>(&data.boneMotionDatas, data.boneNum);

				int cntBone = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("boneMotionData{")) {

						// �G���[���b�Z�[�W
						if (cntBone >= data.boneNum) {
							RNLib::Window().Message_ERROR(CreateText("�{�[�����[�V�����f�[�^�̐����{�[�������I�[�o�[���Ă��܂��B\n%s", loadPath));
							break;
						}

						// �{�[�����[�V�����f�[�^������
						BoneMotionData& boneMotionData = data.boneMotionDatas[cntBone];
						boneMotionData = {};

						// �R�}���h���ǂݍ���
						RNLib::File().Scan(CFile::SCAN::USHORT, &boneMotionData.commandDataNum);

						// �R�}���h����0���z���Ă��鎞�A
						if (boneMotionData.commandDataNum > 0) {
							boneMotionData.commandDatas = new CommandData[boneMotionData.commandDataNum];

							// �R�}���h�̃J�E���g
							int cntCommand = 0;

							// �R�}���h�̓ǂݍ��݂��J�n
							while (RNLib::File().SearchLoop("}")) {
								if (RNLib::File().CheckIdentifier("time")) {

									// �G���[���b�Z�[�W
									if (cntCommand >= boneMotionData.commandDataNum) {
										RNLib::Window().Message_ERROR(CreateText("�R�}���h�f�[�^�̐����w�萔���I�[�o�[���Ă��܂��B\n%s", loadPath));
										
										// �t�@�C�������
										RNLib::File().CloseFile();

										return false;
									}

									// �R�}���h�J�n���ԓǂݍ���
									RNLib::File().Scan(CFile::SCAN::SHORT, &boneMotionData.commandDatas[cntCommand].time);

									{// �R�}���h��ǂݎ��
										int commandNum;
										RNLib::File().Scan(CFile::SCAN::INT, &commandNum);
										boneMotionData.commandDatas[cntCommand].command = (COMMAND)commandNum;
									}
									
									// �ړ�/��]/�g�k�t���O�𗧂Ă�
									if (boneMotionData.commandDatas[cntCommand].command == COMMAND::MOVE)
										boneMotionData.isMove = true;
									else if (boneMotionData.commandDatas[cntCommand].command == COMMAND::SPIN)
										boneMotionData.isSpin = true;
									else if (boneMotionData.commandDatas[cntCommand].command == COMMAND::SCALING)
										boneMotionData.isScale = true;

									// �����̃��������m��
									boneMotionData.commandDatas[cntCommand].datas = NULL;
									boneMotionData.commandDatas[cntCommand].datas = new float[COMMAND_DATA_NUM[(int)boneMotionData.commandDatas[cntCommand].command]];

									// �����̓ǂݍ���
									for (int cntData = 0; cntData < COMMAND_DATA_NUM[(int)boneMotionData.commandDatas[cntCommand].command]; cntData++)
										RNLib::File().Scan(CFile::SCAN::FLOAT, &boneMotionData.commandDatas[cntCommand].datas[cntData]);

									// �R�}���h�̃J�E���g
									cntCommand++;	
								}
							}

							// �G���[���b�Z�[�W
							if (cntCommand < boneMotionData.commandDataNum) {
								RNLib::Window().Message_ERROR(CreateText("�R�}���h�f�[�^�̐����w�萔�ɑ΂��ĕs�����Ă��܂��B\n%s", loadPath));

								// �t�@�C�������
								RNLib::File().CloseFile();

								return false;
							}
						}
						else
						{// �R�}���h����0�ȉ����������A
							boneMotionData.commandDatas = NULL;
							RNLib::File().NewLine();	// }
						}

						// �{�[���̃J�E���g
						cntBone++;
					}
				}
			}
		}

		// �t�@�C�������
		RNLib::File().CloseFile();

		return true;
	}
	else
	{// �ǂݍ��ݎ��s
		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR(CreateText("���[�V�����f�[�^�t�@�C�������݂��܂���B\n%s", loadPath));
	}

	return false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f�[�^�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CMotion3D::CData::CData() {

	loopTime        = 0;
	isLoop          = false;
	boneNum         = 0;
	boneMotionDatas = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
CMotion3D::CData::~CData() {

	// �������
	Release();
}

//========================================
// �������
//========================================
void CMotion3D::CData::Release(void) {

	// �{�[���R�}���h�f�[�^�����
	for (int cntParts = 0; cntParts < boneNum; cntParts++) {
		for (int cntCmd = 0; cntCmd < boneMotionDatas[cntParts].commandDataNum; cntCmd++) {
			CMemory::Release(&boneMotionDatas[cntParts].commandDatas[cntCmd].datas);
		}
		CMemory::Release(&boneMotionDatas[cntParts].commandDatas);
	}
	CMemory::Release(&boneMotionDatas);
}
