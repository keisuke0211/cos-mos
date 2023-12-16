//========================================
// 
// �Z�b�g�A�b�v3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�Z�b�g�A�b�v3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_SetUp3D::_RNC_SetUp3D() {

	m_datas    = NULL;
	m_editData = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_SetUp3D::~_RNC_SetUp3D() {

}

//========================================
// ����������
//========================================
void _RNC_SetUp3D::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_SetUp3D::Uninit(void) {

	// �f�[�^�̉��
	RNLib::Memory().ReleaseDouble(&m_datas, m_num);

	// �G�f�B�b�g�f�[�^�̉��
	RNLib::Memory().Release(&m_editData);
}

//========================================
// �X�V����
//========================================
void _RNC_SetUp3D::Update(void) {

}

//========================================
// �ǂݍ��ݏ���
//========================================
short _RNC_SetUp3D::Load(const char* loadPath, short idx) {

	const UShort numOld = m_num;

	if (CRegist::Load(loadPath, idx))
	{// �ǂݍ��ݐ���
		// �f�[�^�̃������Ċm��
		RNLib::Memory().ReAllocDouble(&m_datas, numOld, m_num);

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
bool _RNC_SetUp3D::LoadEditData(const char* loadPath) {

	CData* loadData = NULL;
	RNLib::Memory().Alloc(&loadData);

	// �ǂݍ��ݎ��s
	if (!ExecutionLoad(loadPath, *loadData)) {
		RNLib::Memory().Release(&loadData);
		return false;
	}

	// �G�f�B�b�g�f�[�^�����
	RNLib::Memory().Release(&m_editData);

	// �G�f�B�b�g�f�[�^���㏑��
	m_editData = loadData;

	// �ăZ�b�g�A�b�v
	CDoll3D* doll = RNLib::Doll3DMgr().GetEditDoll();
	if (doll != NULL)
		doll->SetUp(EDITDATA);

	return true;
}

//========================================
// �����o������(�G�f�B�b�g�f�[�^)
//========================================
void _RNC_SetUp3D::SaveEditData(const char* savePath) {

	if (m_editData == NULL) {
		RNLib::Window().Message_ERROR(String("�Z�b�g�A�b�v3D�f�[�^�����݂��܂���B"));
		return;
	}

	// �t�@�C�����J��
	if (RNLib::File().OpenSaveFile(savePath)) {

		fprintf(RNLib::File().GetFile(), "SetUp3DFile\n");
		fprintf(RNLib::File().GetFile(), "boneDataSum{ %d\n", m_editData->m_boneDataNum);
		for (int cntBoneData = 0; cntBoneData < m_editData->m_boneDataNum; cntBoneData++) {
			fprintf(RNLib::File().GetFile(), "	boneData{\n");
			fprintf(RNLib::File().GetFile(), "		idx %d\n", m_editData->m_boneDatas[cntBoneData].idx);
			fprintf(RNLib::File().GetFile(), "		modelIdx %s\n", RNLib::Model().GetLoadPath(m_editData->m_boneDatas[cntBoneData].modelIdx));
			fprintf(RNLib::File().GetFile(), "		parentIdx %d\n", m_editData->m_boneDatas[cntBoneData].parentIdx);
			fprintf(RNLib::File().GetFile(), "		relativePos %.2f %.2f %.2f\n", 
				m_editData->m_boneDatas[cntBoneData].relativePos.x,
				m_editData->m_boneDatas[cntBoneData].relativePos.y,
				m_editData->m_boneDatas[cntBoneData].relativePos.z);
			fprintf(RNLib::File().GetFile(), "		relativeRot %.2f %.2f %.2f\n", 
				m_editData->m_boneDatas[cntBoneData].relativeRot.x / D3DX_PI,
				m_editData->m_boneDatas[cntBoneData].relativeRot.y / D3DX_PI,
				m_editData->m_boneDatas[cntBoneData].relativeRot.z / D3DX_PI);
			if (m_editData->m_boneDatas[cntBoneData].swaying != NULL) {
				fprintf(RNLib::File().GetFile(), "		swaying{\n");
				fprintf(RNLib::File().GetFile(), "			timeMin %d\n", m_editData->m_boneDatas[cntBoneData].swaying->timeMin);
				fprintf(RNLib::File().GetFile(), "			timeAdd %d\n", m_editData->m_boneDatas[cntBoneData].swaying->timeAdd);
				fprintf(RNLib::File().GetFile(), "			distMin %f\n", m_editData->m_boneDatas[cntBoneData].swaying->distMin);
				fprintf(RNLib::File().GetFile(), "			distAdd %f\n", m_editData->m_boneDatas[cntBoneData].swaying->distAdd);
				fprintf(RNLib::File().GetFile(), "		}\n");
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");

		fprintf(RNLib::File().GetFile(), "faceDataSum{ %d\n", m_editData->m_faceDataNum);
		for (int cntFaceData = 0; cntFaceData < m_editData->m_faceDataNum; cntFaceData++) {
			fprintf(RNLib::File().GetFile(), "	faceData{\n");
			fprintf(RNLib::File().GetFile(), "		texIdx %s\n", RNLib::Texture().GetLoadPath(m_editData->m_faceDatas[cntFaceData].texIdx));
			fprintf(RNLib::File().GetFile(), "		col %d %d %d %d\n",
				m_editData->m_faceDatas[cntFaceData].col.r,
				m_editData->m_faceDatas[cntFaceData].col.g,
				m_editData->m_faceDatas[cntFaceData].col.b,
				m_editData->m_faceDatas[cntFaceData].col.a);
			for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
				fprintf(RNLib::File().GetFile(), "		vtx%d{\n", cntVtx);
				fprintf(RNLib::File().GetFile(), "			boneIdx %d\n", m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].boneIdx);
				fprintf(RNLib::File().GetFile(), "			vtxIdx %d\n", m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].vtxIdx);
				fprintf(RNLib::File().GetFile(), "			texPos %.2f %.2f\n",
					m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].texPos.x,
					m_editData->m_faceDatas[cntFaceData].vtxs[cntVtx].texPos.y);
				fprintf(RNLib::File().GetFile(), "		}\n");
			}
			fprintf(RNLib::File().GetFile(), "	}\n");
		}
		fprintf(RNLib::File().GetFile(), "}\n");
		fprintf(RNLib::File().GetFile(), "END\n");

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
	else
	{// �����o�����s
		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR(String("�Z�b�g�A�b�v3D�f�[�^�t�@�C����������܂���ł����B\n%s", savePath));
	}
}

//========================================
// ���������w�萔�ɏ�����
//========================================
void _RNC_SetUp3D::InitMemory(const UShort& num) {
	CRegist::InitMemory(num);

	// �f�[�^�̃������m��
	RNLib::Memory().Alloc(&m_datas, num);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�Z�b�g�A�b�v3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ǂݍ��ݎ��s����
//========================================
bool _RNC_SetUp3D::ExecutionLoad(const char* loadPath, CData& data) {

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(loadPath, "SetUp3DFile")) {
		while (RNLib::File().SearchLoop("END")) {
			if (RNLib::File().CheckIdentifier("boneDataSum{")) {
				BoneData*& boneDatas = data.m_boneDatas;
				
				// �{�[���f�[�^�Q�����
				RNLib::Memory().Release(&boneDatas);

				// �{�[���f�[�^���ǂݍ���
				RNLib::File().Scan(_RNC_File::SCAN::SHORT, &data.m_boneDataNum);

				// �{�[���f�[�^�Q�̃������m��
				RNLib::Memory().Alloc(&boneDatas, data.m_boneDataNum);

				// �{�[���f�[�^�Q�̓ǂݍ���
				int cntBoneData = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("boneData{")) {

						// �G���[���b�Z�[�W
						if (cntBoneData >= data.m_boneDataNum) {
							RNLib::Window().Message_ERROR(String("�{�[���f�[�^�̐����w�萔���I�[�o�[���Ă��܂��B\n%s", loadPath));

							// �t�@�C�������
							RNLib::File().CloseFile();

							return false;
						}

						// �{�[���f�[�^������
						BoneData& boneData = boneDatas[cntBoneData];
						boneData = {};

						// ���i���̓ǂݍ��݂��J�n
						while (RNLib::File().SearchLoop("}")) {
							RNLib::File().Scan(_RNC_File::SCAN::SHORT, &boneData.idx, "idx");
							RNLib::File().Scan(_RNC_File::SCAN::MODELIDX, &boneData.modelIdx, "modelIdx");
							RNLib::File().Scan(_RNC_File::SCAN::SHORT, &boneData.parentIdx, "parentIdx");
							RNLib::File().Scan(_RNC_File::SCAN::POS3D, &boneData.relativePos, "relativePos");
							RNLib::File().Scan(_RNC_File::SCAN::ROT3D_CORRECT, &boneData.relativeRot, "relativeRot");
							if (RNLib::File().CheckIdentifier("swaying{")) {
								RNLib::Memory().Alloc(&boneData.swaying);

								while (RNLib::File().SearchLoop("}")) {
									RNLib::File().Scan(_RNC_File::SCAN::USHORT, &boneData.swaying->timeMin, "timeMin");
									RNLib::File().Scan(_RNC_File::SCAN::USHORT, &boneData.swaying->timeAdd, "timeAdd");
									RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &boneData.swaying->distMin, "distMin");
									RNLib::File().Scan(_RNC_File::SCAN::FLOAT, &boneData.swaying->distAdd, "distAdd");
								}
							}
							else if (RNLib::File().CheckIdentifier("follow{")) {
								RNLib::Memory().Alloc(&boneData.follow);

								while (RNLib::File().SearchLoop("}")) {
									RNLib::File().Scan(_RNC_File::SCAN::SHORT, &boneData.follow->followIdx, "followIdx");
									RNLib::File().Scan(_RNC_File::SCAN::POS3D, &boneData.follow->posRate, "posRate");
									RNLib::File().Scan(_RNC_File::SCAN::POS3D, &boneData.follow->rotRate, "rotRate");
									RNLib::File().Scan(_RNC_File::SCAN::POS3D, &boneData.follow->scaleRate, "scaleRate");
								}
							}
						}

						// �J�E���g�����Z
						cntBoneData++;
					}
				}// �{�[���f�[�^�Q�̓ǂݍ��ݏI���c

				// �G���[���b�Z�[�W
				if (cntBoneData < data.m_boneDataNum) {
					RNLib::Window().Message_ERROR(String("�{�[���f�[�^�̐����w�萔�ɑ΂��ĕs�����Ă��܂��B\n%s", loadPath));

					// �t�@�C�������
					RNLib::File().CloseFile();

					return false;
				}
				
				// �{�[���f�[�^1��1�̐e�ԍ����J�E���g�ɕϊ����Ă���
				for (cntBoneData = 0; cntBoneData < data.m_boneDataNum; cntBoneData++) {
					BoneData& boneData = boneDatas[cntBoneData];

					// �e��DATANONE�̎��A�܂�Ԃ�
					if (boneData.parentIdx == NONEDATA)
						continue;

					// �e�ԍ��ƈ�v�����{�[���f�[�^��e�Ƃ��A
					// �{�[���f�[�^�̐e�ԍ����J�E���g�ɕϊ�����
					int cntBoneData2;
					for (cntBoneData2 = 0; cntBoneData2 < data.m_boneDataNum; cntBoneData2++) {
						if (boneData.parentIdx == boneDatas[cntBoneData2].idx) {
							boneData.parentIdx = cntBoneData2;
							break;
						}
					}

					// �e��������Ȃ��������A�e�ԍ���DATANONE�ɂ���
					if (cntBoneData2 == data.m_boneDataNum)
						boneData.parentIdx = NONEDATA;
				}
			}
			else if (RNLib::File().CheckIdentifier("faceDataSum{")) {
				FaceData*& faceDatas = data.m_faceDatas;

				// �t�F�C�X�f�[�^�Q�����
				RNLib::Memory().Release(&faceDatas);

				// �t�F�C�X�f�[�^���ǂݍ���
				RNLib::File().Scan(_RNC_File::SCAN::SHORT, &data.m_faceDataNum);

				// �t�F�C�X�f�[�^�Q�̃������m��
				RNLib::Memory().Alloc(&faceDatas, data.m_faceDataNum);

				// �t�F�C�X�f�[�^�Q�̓ǂݍ���
				int cntFaceData = 0;
				while (RNLib::File().SearchLoop("}")) {
					if (RNLib::File().CheckIdentifier("faceData{")) {

						// �G���[���b�Z�[�W
						if (cntFaceData >= data.m_faceDataNum) {
							RNLib::Window().Message_ERROR(String("�t�F�C�X�f�[�^�̐����w�萔���I�[�o�[���Ă��܂��B\n%s", loadPath));

							// �t�@�C�������
							RNLib::File().CloseFile();

							return false;
						}

						// �t�F�C�X�f�[�^������
						FaceData& faceData = faceDatas[cntFaceData];
						faceData = {};

						// ���_���̓ǂݍ��݂��J�n
						while (RNLib::File().SearchLoop("}")) {
							struct LocalFunc {
								static void LoadFaceVtxData(_RNC_SetUp3D::FaceVtxData& vtxData) {
									while (RNLib::File().SearchLoop("}")) {
										RNLib::File().Scan(_RNC_File::SCAN::USHORT, &vtxData.boneIdx, "boneIdx");
										RNLib::File().Scan(_RNC_File::SCAN::UINT, &vtxData.vtxIdx, "vtxIdx");
										RNLib::File().Scan(_RNC_File::SCAN::POS2D, &vtxData.texPos, "texPos");
									}
								}
							};

							// ���_���̓ǂݍ���
							if (RNLib::File().CheckIdentifier("vtx0{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[0]);
							else if (RNLib::File().CheckIdentifier("vtx1{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[1]);
							else if (RNLib::File().CheckIdentifier("vtx2{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[2]);
							else if (RNLib::File().CheckIdentifier("vtx3{"))
								LocalFunc::LoadFaceVtxData(faceData.vtxs[3]);

							RNLib::File().Scan(_RNC_File::SCAN::TEXIDX, &faceData.texIdx, "texIdx");
							RNLib::File().Scan(_RNC_File::SCAN::COLOR, &faceData.col, "col");
						}

						// �J�E���g�����Z
						cntFaceData++;
					}
				}// �t�F�C�X�f�[�^�Q�̓ǂݍ��ݏI���c

				// �G���[���b�Z�[�W
				if (cntFaceData < data.m_faceDataNum) {
					RNLib::Window().Message_ERROR(String("�t�F�C�X�f�[�^�̐����w�萔�ɑ΂��ĕs�����Ă��܂��B\n%s", loadPath));

					// �t�@�C�������
					RNLib::File().CloseFile();

					return false;
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
		RNLib::Window().Message_ERROR(String("�Z�b�g�A�b�v3D�f�[�^�t�@�C�������݂��܂���B\n%s", loadPath));
	}

	return false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f�[�^�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_SetUp3D::CData::CData() {

	m_boneDatas   = NULL;
	m_boneDataNum = 0;
	m_faceDatas   = NULL;
	m_faceDataNum = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_SetUp3D::CData::~CData() {

	// �������
	Release();
}

//========================================
// �������
//========================================
void _RNC_SetUp3D::CData::Release(void) {

	for (int cntBone = 0; cntBone < m_boneDataNum; cntBone++) {
		RNLib::Memory().Release(&m_boneDatas[cntBone].swaying);
		RNLib::Memory().Release(&m_boneDatas[cntBone].follow);
	}
	RNLib::Memory().Release(&m_boneDatas);
	RNLib::Memory().Release(&m_faceDatas);
}