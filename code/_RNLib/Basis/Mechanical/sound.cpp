//========================================
// 
// �T�E���h�̏���
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�T�E���h�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CSound::CSound() : m_playMgr("RN_PlayMgr") {

}

//========================================
// �f�X�g���N�^
//========================================
CSound::~CSound() {

}

//========================================
// ����������
//========================================
void CSound::Init(void) {

	m_datas = NULL;
	for (int cntCategory = 0; cntCategory < (int)CATEGORY::MAX; m_categoryStates[cntCategory] = {}, cntCategory++);
	m_mic3DPos = INITPOS3D;
	m_XAudio2 = NULL;
	m_masteringVoice = NULL;

	// COM���C�u�����̏�����
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2�I�u�W�F�N�g�̍쐬
	if (FAILED(XAudio2Create(&m_XAudio2, 0))) {

		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR("XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s���܂����B");

		// COM���C�u�����̏I������
		CoUninitialize();

		return;
	}

	// �}�X�^�[�{�C�X�̐���
	if (FAILED(m_XAudio2->CreateMasteringVoice(&m_masteringVoice))) {

		// �G���[���b�Z�[�W
		RNLib::Window().Message_ERROR("�}�X�^�[�{�C�X�̐����Ɏ��s���܂����B");

		// XAudio2�I�u�W�F�N�g�̊J��
		m_XAudio2->Release();
		m_XAudio2 = NULL;

		// COM���C�u�����̏I������
		CoUninitialize();

		return;
	}
}

//========================================
// �I������
//========================================
void CSound::Uninit(void) {

	// �Đ��}�l�[�W���[�̑S���
	m_playMgr.ReleaseAll();

	// �f�[�^�̉��
	CMemory::ReleaseDouble(&m_datas, m_num);

	// �}�X�^�[�{�C�X�̔j��
	if (m_masteringVoice != NULL) {
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = NULL;
	}

	// XAudio2�I�u�W�F�N�g�̊J��
	if (m_XAudio2 != NULL) {
		m_XAudio2->Release();
		m_XAudio2 = NULL;
	}

	// COM���C�u�����̏I������
	CoUninitialize();
}

//========================================
// �X�V����
//========================================
void CSound::Update(void) {

}

//========================================
// �ǂݍ��ݏ���
//========================================
short CSound::Load(const char* loadPath, short idx) {
	
	const UShort oldNum = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx)) 
	{// �ǂݍ��ݐ���
		// �f�[�^�̃������Ċm��
		CMemory::ReAllocDouble(&m_datas, oldNum, m_num);

		// �f�[�^�̔j��(�ԍ��w��̏ꍇ)
		if (idxOld != NONEDATA)
			m_datas[idx]->Release();

		DWORD shunkSize    = 0;
		DWORD chunkPos     = 0;
		DWORD fileCategory = 0;

		// �T�E���h�f�[�^�t�@�C���̐���
		HANDLE fileHandle = CreateFile(loadPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s���܂����B(1)\n%s", loadPath));
			
			return NONEDATA;
		}

		// �t�@�C���|�C���^��擪�Ɉړ�
		if (SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�T�E���h�f�[�^�t�@�C���̐����Ɏ��s���܂����B(2)\n%s", loadPath));
			
			return NONEDATA;
		}

		// WAVE�t�@�C���̃`�F�b�N
		if (FAILED(CheckChunk(fileHandle, 'FFIR', &shunkSize, &chunkPos))) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("WAVE�t�@�C���̃`�F�b�N�Ɏ��s���܂����B(1)\n%s", loadPath));

			return NONEDATA;
		}

		// WAVE�t�@�C���̃`�F�b�N
		if (FAILED(ReadChunkData(fileHandle, &fileCategory, sizeof(DWORD), chunkPos))) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("WAVE�t�@�C���̃`�F�b�N�Ɏ��s���܂����B(2)\n%s", loadPath));

			return NONEDATA;
		}

		// WAVE�t�@�C���̃`�F�b�N
		if (fileCategory != 'EVAW') {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("WAVE�t�@�C���̃`�F�b�N�Ɏ��s���܂����B(3)\n%s", loadPath));

			return NONEDATA;
		}

		// �t�H�[�}�b�g�`�F�b�N
		if (FAILED(CheckChunk(fileHandle, ' tmf', &shunkSize, &chunkPos))) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�t�H�[�}�b�g�̃`�F�b�N�Ɏ��s���܂����B(1)\n%s", loadPath));

			return NONEDATA;
		}

		// �t�H�[�}�b�g�`�F�b�N
		if (FAILED(ReadChunkData(fileHandle, &m_datas[idx]->m_wfx, shunkSize, chunkPos))) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�t�H�[�}�b�g�̃`�F�b�N�Ɏ��s���܂����B(2)\n%s", loadPath));

			return NONEDATA;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		if (FAILED(CheckChunk(fileHandle, 'atad', &m_datas[idx]->m_audioDataSize, &chunkPos))) {

			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s���܂����B(1)\n%s", loadPath));
			
			return NONEDATA;
		}

		// �I�[�f�B�I�f�[�^�ǂݍ���
		m_datas[idx]->m_audioData = (BYTE*)malloc(m_datas[idx]->m_audioDataSize);
		if (FAILED(ReadChunkData(fileHandle, m_datas[idx]->m_audioData, m_datas[idx]->m_audioDataSize, chunkPos))) {
			
			// �G���[���b�Z�[�W
			RNLib::Window().Message_ERROR(CreateText("�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s���܂����B(2)\n%s", loadPath));

			return NONEDATA;
		}

		// �o�b�t�@�̒l�ݒ�
		m_datas[idx]->m_audioBuffer.AudioBytes = m_datas[idx]->m_audioDataSize;
		m_datas[idx]->m_audioBuffer.pAudioData = m_datas[idx]->m_audioData;
		m_datas[idx]->m_audioBuffer.Flags      = XAUDIO2_END_OF_STREAM;
		m_datas[idx]->m_audioBuffer.LoopCount  = 0;

		// �t�@�C�������
		CloseHandle(fileHandle);
	}

	return idx;
}

//========================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//========================================
UShort CSound::Play(const short& sountIdx, const CATEGORY& category, const float& volume, const bool& isLoop) {
	return (new CSound::CPlay(sountIdx, category, volume, isLoop, NULL, NULL, 0.0f))->GetID();
}
UShort CSound::Play(const short& sountIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos2D& pos, const float& dist) {
	return (new CSound::CPlay(sountIdx, category, volume, isLoop, &pos, NULL, dist))->GetID();
}
UShort CSound::Play(const short& sountIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos3D& pos, const float& dist) {
	return (new CSound::CPlay(sountIdx, category, volume, isLoop, NULL, &pos, dist))->GetID();
}

//========================================
// ��~(���ގw��)
//========================================
void CSound::StopCategory(const CATEGORY& category) {

	CSound::CPlay* play = NULL;
	while (m_playMgr.ListLoop((CObject**)&play)) {

		// ���ނ���v���Ă��鎞�A�폜
		if (play->GetCategory() == category)
			play->Delete();
	}
}

//========================================
// ��~(�S��)
//========================================
void CSound::StopAll(void) {
	
	CSound::CPlay* play = NULL;
	while (m_playMgr.ListLoop((CObject**)&play)) {

		// �폜
		play->Delete();
	}
}

//========================================
// ���ޖ��ɉ��ʕύX
//========================================
void CSound::ChangeCategoryVolume(const CATEGORY& category, float& volume) {

	// ���ʂ𐧌�
	if (volume < 0.0f)
		volume = 0.0f;
	else if (volume > 1.0f)
		volume = 1.0f;

	// ���ʂ�ݒ�
	m_categoryStates[(int)category].volume = volume * m_categoryStates[(int)category].settingVolume;
}

//========================================
// �ݒ艹�ʕύX
//========================================
void CSound::ChangeSetVolume(const CATEGORY& category, float& volume) {

	// �ݒ艹�ʂ𐧌�
	if (volume < 0.0f)
		volume = 0.0f;
	else if (volume > 1.0f)
		volume = 1.0f;

	// �ݒ艹�ʂ�ݒ�
	m_categoryStates[(int)category].settingVolume = volume;
}

//========================================
// �v���C�I�u�W�F�N�g�擾
//========================================
CSound::CPlay& CSound::GetPlay(const UShort& ID) {

	CSound::CPlay* play = NULL;
	while (m_playMgr.ListLoop((CObject**)&play)) {

		if (play->GetID() == ID)
			return *play;
	}

	return *(CPlay*)nullptr;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�T�E���h�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �`�����N�̃`�F�b�N
//========================================
HRESULT CSound::CheckChunk(HANDLE fileHandle, DWORD format, DWORD *chunkSize, DWORD *chunkDataPosition) {

	HRESULT hr            = S_OK;
	DWORD   read          = 0;
	DWORD   chunkType     = 0;
	DWORD   chunkDataSize = 0;
	DWORD   RIFFDataSize  = 0;
	DWORD   fileType      = 0;
	DWORD   bytesRead     = 0;
	DWORD   offset        = 0;

	// �t�@�C���|�C���^��擪�Ɉړ�
	if (SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	while (hr == S_OK) {

		// �`�����N�̓ǂݍ���
		if (ReadFile(fileHandle, &chunkType, sizeof(DWORD), &read, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());

		// �`�����N�f�[�^�̓ǂݍ���
		if (ReadFile(fileHandle, &chunkDataSize, sizeof(DWORD), &read, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (chunkType) {
		case 'FFIR':
			RIFFDataSize  = chunkDataSize;
			chunkDataSize = 4;

			// �t�@�C���^�C�v�̓ǂݍ���
			if (ReadFile(fileHandle, &fileType, sizeof(DWORD), &read, NULL) == 0)
				hr = HRESULT_FROM_WIN32(GetLastError());

			break;

		default:

			// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
			if (SetFilePointer(fileHandle, chunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
				return HRESULT_FROM_WIN32(GetLastError());

			break;
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format) {
			*chunkSize = chunkDataSize;
			*chunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= RIFFDataSize) {
			return S_FALSE;
		}
	}

	return S_OK;
}

//========================================
// �`�����N�f�[�^�̓ǂݍ���
//========================================
HRESULT CSound::ReadChunkData(HANDLE fileHandle, void *buffer, DWORD bufferSize, DWORD bufferOffset) {
	
	DWORD read;

	// �t�@�C���|�C���^���w��ʒu�܂ňړ�
	if (SetFilePointer(fileHandle, bufferOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	// �f�[�^�̓ǂݍ���
	if (ReadFile(fileHandle, buffer, bufferSize, &read, NULL) == 0)
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�f�[�^�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CSound::CData::CData() {

	m_audioData     = NULL;
	m_audioDataSize = 0;
	memset(&m_audioBuffer, 0, sizeof(XAUDIO2_BUFFER));
	memset(&m_wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
}

//========================================
// �f�X�g���N�^
//========================================
CSound::CData::~CData() {

	// �������
	Release();
}

//========================================
// �������
//========================================
void CSound::CData::Release(void) {

	// �I�[�f�B�I�f�[�^�̊J��
	if (m_audioData != NULL) {
		CMemory::Release(&m_audioData);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�Đ��N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
UShort CSound::CPlay::ms_IDCount = 0;

//========================================
// �R���X�g���N�^
//========================================
CSound::CPlay::CPlay(const short& sountIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos2D* pos2D, const Pos3D* pos3D, const float& dist) {

	// ���X�g�ɒǉ�
	RNLib::Sound().GetPlayMgr().AddList(this);

	// ID��ݒ�
	m_ID = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % USHRT_MAX;

	m_soundIdx = sountIdx;
	m_volume   = volume;
	m_category = category;
	m_isLoop   = isLoop;
	m_dist     = dist;

	// �ʒu���̊m��
	if (pos2D == NULL) { m_pos2D = NULL; }
	else {
		CMemory::Alloc(&m_pos2D);
		*m_pos2D = *pos2D;
	}
	if (pos3D == NULL) { m_pos3D = NULL; }
	else {
		CMemory::Alloc(&m_pos3D);
		*m_pos3D = *pos3D;
	}

	// �\�[�X�{�C�X�̐���
	if (SUCCEEDED(RNLib::Sound().GetXAudio2().CreateSourceVoice(&m_sourceVoice, &(RNLib::Sound().GetData(m_soundIdx).m_wfx.Format)))) {

		// �I�[�f�B�I�o�b�t�@�̓o�^
		m_sourceVoice->SubmitSourceBuffer(&RNLib::Sound().GetData(m_soundIdx).m_audioBuffer);

		// ���ʂ�0�ɂ��Ă���
		m_sourceVoice->SetVolume(0.0f);

		// �Đ�
		m_sourceVoice->Start(0);
	}
	else {
		m_sourceVoice = NULL;
		Delete();
	}
}

//========================================
// �f�X�g���N�^
//========================================
CSound::CPlay::~CPlay() {
	
	// �ʒu���̉��
	CMemory::Release(&m_pos2D);
	CMemory::Release(&m_pos3D);

	// ���X�g����폜
	RNLib::Sound().GetPlayMgr().SubList(this);

	// ��~���A�I�[�f�B�I�o�b�t�@�̍폜
	if (m_sourceVoice != NULL) {
		m_sourceVoice->Stop(0);
		m_sourceVoice->FlushSourceBuffers();
	}
}

//========================================
// �X�V����
//========================================
void CSound::CPlay::Update(void) {

	// �\�[�X�{�C�X��NULL�ł���΁A���g��j�����ďI��
	if (m_sourceVoice == NULL) {
		Delete();
		return;
	}

	// ��Ԃ��擾
	XAUDIO2_VOICE_STATE xa2state;
	m_sourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued == 0)
	{// �Đ����łȂ����A

		if (m_isLoop) 
		{// ���[�v�t���O���^�Ȃ�ĂэĐ�
			m_sourceVoice->SubmitSourceBuffer(&RNLib::Sound().GetData(m_soundIdx).m_audioBuffer);	// �I�[�f�B�I�o�b�t�@�̓o�^
			m_sourceVoice->Start(0);
		}
		else 
		{// ���[�v�t���O���U�Ȃ玩�g���폜
			Delete();
			return;
		}
	}

	if (m_pos2D != NULL) {

	}
	else if (m_pos3D != NULL) {

		// �����̊��������߂�
		float distRateOpp = 1.0f - (CGeometry::FindDistance(*m_pos3D, RNLib::Sound().GetMic3DPos()) / m_dist);
		if (distRateOpp <= 0.0f) {
			m_sourceVoice->SetVolume(0.0f);
		}
		else {
			// ���ʂ𔽉f������
			m_sourceVoice->SetVolume(m_volume * RNLib::Sound().GetCategoryState(m_category).volume * RNLib::Sound().GetCategoryState(m_category).settingVolume * distRateOpp);
		}
	}
	else {
		// ���ʂ𔽉f������
		m_sourceVoice->SetVolume(m_volume * RNLib::Sound().GetCategoryState(m_category).volume * RNLib::Sound().GetCategoryState(m_category).settingVolume);
	}
}