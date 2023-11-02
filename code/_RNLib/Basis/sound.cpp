//========================================
// 
// サウンド処理
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
// [[[ sound.cpp ]]]
//========================================
#include "../RNLib.h"

//****************************************
// マクロ定義
//****************************************
#define DATAFILE_PATH "data\\_RNData\\SoundList.txt"	// サウンドのデータファイルパス

//****************************************
// 定数定義
//****************************************
namespace {
	// サウンド名
	const char* c_aSoundName[] = {
		"DIGBUG",
		"DIGBUG_BOSS",
		"MENU_SELECT",
		"MENU_DECIDE",
		"MENU_BACK",
		"SCORE_COUNT",
		"GOLD_COUNT",
		"ITEM_GET",
		"GAMECLEAR",
		"GAMEOVER",
		"TAKE_AIM",
		"BLOW_DAMAGE_LOW",
		"BLOW_DAMAGE_MID",	
		"BLOW_DAMAGE_HIGH",
		"SLASH_DAMAGE_LOW",
		"SLASH_DAMAGE_MID",
		"SLASH_DAMAGE_HIGH",
		"SPC_DAMAGE_LOW",	
		"SPC_DAMAGE_MID",	
		"ELEM_DAMAGE_FIRE",
		"ELEM_DAMAGE_WATER",
		"ELEM_DAMAGE_ICE",	
		"ELEM_DAMAGE_WIND",
		"ELEM_DAMAGE_ELEC",
		"ELEM_DAMAGE_SOIL",
		"ELEM_DAMAGE_DARK",
		"ELEM_DAMAGE_LIGHT",
		"IMPACT_MID",
		"BREATH_LOW",
		"BREATH_MID",
		"BREATH_HIGH",
		"BREATH_SHOT",
		"LANDING_GRASS",
		"LANDING_SOIL",
		"LANDING_STONE",
		"LANDING_SAND",
		"LANDING_IMPACT",
		"EARTHQUAKE",
		"BREAK_SOIL",
		"BREAK_STONE",
		"EXPLOSION_MID",
		"SWING_LOW",
		"SWING_MID",
		"SWING_HIGH",
		"METAL_STRIKE",
		"MOTOR",
	};

	// 距離
	const float c_aDistance[] = {
		0.0f,
		50.0f,
		250.0f,
		500.0f,
		1000.0f,
	};
}
CHECK_ARRAY_SIZE(c_aSoundName, CSound::MAX);
CHECK_ARRAY_SIZE(c_aDistance , CSound::DISTANCE_MAX);

//================================================================================
//----------|---------------------------------------------------------------------
//==========| Soundクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSound::CSound() {
	m_pXAudio2        = NULL;
	m_pMasteringVoice = NULL;
	m_mic3DPos        = INITD3DXVECTOR3;
	
	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
			m_apSourceVoice[nCntSound][nCntPlay] = NULL;
			m_aState       [nCntSound][nCntPlay] = {};
		}

		m_apDataAudio[nCntSound] = NULL;
		m_aSizeAudio [nCntSound] = NULL;							
	}

	for (int nCntCategory = 0; nCntCategory < (int)CATEGORY::MAX; nCntCategory++) {
		m_aCategoryVolume[nCntCategory] = 0.0f;
		m_aSettingVolume [nCntCategory] = 0.0f;
	}
}

//========================================
// デストラクタ
//========================================
CSound::~CSound() {

}

//========================================
// 初期化処理
//========================================
HRESULT CSound::Init(void) {
	HWND hWnd = RNLib::Window().GetHandle();

	for (int nCntCategory = 0; nCntCategory < (int)CATEGORY::MAX; nCntCategory++) {
		m_aCategoryVolume[nCntCategory] = 1.0f;
		m_aSettingVolume [nCntCategory] = 1.0f;
	}

	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&m_pXAudio2, 0);
	if (FAILED(hr)) {
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// マスターボイスの生成
	hr = m_pXAudio2->CreateMasteringVoice(&m_pMasteringVoice);
	if (FAILED(hr)) {
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		// XAudio2オブジェクトの開放
		if (m_pXAudio2 != NULL) {
			m_pXAudio2->Release();
			m_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		HANDLE hFile;
		DWORD dwChunkSize     = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFilecategory;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(m_aData[nCntSound].aFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			char aString[TXT_MAX];
			sprintf(aString, "サウンドデータファイルの生成に失敗！(1) (カウント%d)", nCntSound);
			MessageBox(hWnd, aString, "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		hr = ReadChunkData(hFile, &dwFilecategory, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		if (dwFilecategory != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr)) {
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr)) {
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &m_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr)) {
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		m_apDataAudio[nCntSound] = (BYTE*)malloc(m_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, m_apDataAudio[nCntSound], m_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr)) {
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
			// ソースボイスの生成
			hr = m_pXAudio2->CreateSourceVoice(&m_apSourceVoice[nCntSound][nCntPlay], &(wfx.Format));
			if (FAILED(hr)) {
				MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
				return S_FALSE;
			}

			// オーディオバッファの登録
			m_apSourceVoice[nCntSound][nCntPlay]->SubmitSourceBuffer(&buffer);
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = m_aSizeAudio[nCntSound];
		buffer.pAudioData = m_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = m_aData[nCntSound].nCntLoop;

		// ファイルをクローズ
		CloseHandle(hFile);
	}

	return S_OK;
}

//========================================
// 終了処理
//========================================
void CSound::Uninit(void) {
	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
			if (m_apSourceVoice[nCntSound][nCntPlay] != NULL) {
				m_apSourceVoice[nCntSound][nCntPlay]->Stop(0);

				// ソースボイスの破棄
				m_apSourceVoice[nCntSound][nCntPlay]->DestroyVoice();
				m_apSourceVoice[nCntSound][nCntPlay] = NULL;
			}

			// オーディオデータの開放
			free(m_apDataAudio[nCntSound]);
			m_apDataAudio[nCntSound] = NULL;
		}
	}

	// マスターボイスの破棄
	m_pMasteringVoice->DestroyVoice();
	m_pMasteringVoice = NULL;

	// XAudio2オブジェクトの開放
	if (m_pXAudio2 != NULL) {
		m_pXAudio2->Release();
		m_pXAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//========================================
// 更新処理
//========================================
void CSound::Update(void) {
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
			State& state = m_aState[nCntSound][nCntPlay];

			if (!state.bPlay)
				continue;
			
			m_apSourceVoice[nCntSound][nCntPlay]->GetState(&xa2state);

			if (xa2state.BuffersQueued == 0)
			{// 再生中でない時、
				state.bPlay = false;

				// 停止し、オーディオバッファの削除
				m_apSourceVoice[nCntSound][nCntPlay]->Stop(0);
				m_apSourceVoice[nCntSound][nCntPlay]->FlushSourceBuffers();

				continue;
			}

			switch (state.space) {
			case SPACE_3D: {
				float fRate = 1.0f - (CGeometry::FindDistance(state.pos, m_mic3DPos) / c_aDistance[state.dist]);
				if (fRate <= 0.0f) {
					m_apSourceVoice[nCntSound][nCntPlay]->SetVolume(0.0f);
					break;
				}
				else if (fRate > 1.0f)
					fRate = 1.0f;

				m_apSourceVoice[nCntSound][nCntPlay]->SetVolume(
					m_aData[nCntSound].fVolume *
					m_aCategoryVolume[(int)m_aData[nCntSound].category] *
					m_aSettingVolume [(int)m_aData[nCntSound].category] *
					fRate);
			}break;
			}
		}
	}
}

//========================================
// 読み込み処理
//========================================
void CSound::Load(void) {
	if (!RNLib::File().OpenLoadFile(DATAFILE_PATH))
		return;

	// 読み込みループ
	while (RNLib::File().SearchLoop("END")) {
		if (RNLib::File().CheckIdentifier("BGM{")) {
			while (RNLib::File().SearchLoop("}")) {
				for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
					if (RNLib::File().CheckIdentifier(CreateText("%s:", c_aSoundName[nCntSound]))) {
						m_aData[nCntSound].category = CATEGORY::BGM;
						RNLib::File().Scan(CFile::SCAN::STRING,  m_aData[nCntSound].aFileName);
						RNLib::File().Scan(CFile::SCAN::FLOAT, &m_aData[nCntSound].fVolume);
						RNLib::File().Scan(CFile::SCAN::INT, &m_aData[nCntSound].nCntLoop);
						break;
					}
				}
			}
		}
		else if (RNLib::File().CheckIdentifier("SE{")) {
			while (RNLib::File().SearchLoop("}")) {
				for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
					if (RNLib::File().CheckIdentifier(CreateText("%s:", c_aSoundName[nCntSound]))) {
						m_aData[nCntSound].category = CATEGORY::SE;
						RNLib::File().Scan(CFile::SCAN::STRING,  m_aData[nCntSound].aFileName);
						RNLib::File().Scan(CFile::SCAN::FLOAT, &m_aData[nCntSound].fVolume);
						RNLib::File().Scan(CFile::SCAN::INT, &m_aData[nCntSound].nCntLoop);
						break;
					}
				}
			}
		}
	}

	RNLib::File().CloseFile();
}

//========================================
// チャンクのチェック
//========================================
HRESULT CSound::CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition) {
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK) {
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType) {
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format) {
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize) {
			return S_FALSE;
		}
	}

	return S_OK;
}

//========================================
// チャンクデータの読み込み
//========================================
HRESULT CSound::ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset) {
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}

//========================================
// セグメント再生(再生中なら停止)
//========================================
int CSound::Play(int nSoundIdx, SPACE space, D3DXVECTOR3 pos, DISTANCE dist) {
	if (nSoundIdx < 0 || nSoundIdx >= MAX)
		return NONEDATA;

	// BGMだった時、BGMを一括停止する
	if (m_aData[nSoundIdx].category == CATEGORY::BGM)
		Stop(CATEGORY::BGM);

	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER      buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = m_aSizeAudio[nSoundIdx];
	buffer.pAudioData = m_apDataAudio[nSoundIdx];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = m_aData[nSoundIdx].nCntLoop;
	
	int nCntPlay = 0;
	for (; nCntPlay < PLAY_MAX; nCntPlay++) {
		State& state = m_aState[nSoundIdx][nCntPlay];

		// 再生中の時、処理を折り返す
		if (state.bPlay)
			continue;

		state.bPlay = true;
		state.space = space;
		state.pos   = pos;
		state.dist  = dist;

		// 状態取得
		m_apSourceVoice[nSoundIdx][nCntPlay]->GetState(&xa2state);

		if (xa2state.BuffersQueued != 0)
		{// 再生中の時、停止し、オーディオバッファを削除
			m_apSourceVoice[nSoundIdx][nCntPlay]->Stop(0);
			m_apSourceVoice[nSoundIdx][nCntPlay]->FlushSourceBuffers();
		}

		// オーディオバッファの登録
		m_apSourceVoice[nSoundIdx][nCntPlay]->SubmitSourceBuffer(&buffer);

		// 音量を設定
		if (state.space == SPACE_NONE) {
			m_apSourceVoice[nSoundIdx][nCntPlay]->SetVolume(
				m_aData[nSoundIdx].fVolume *
				m_aCategoryVolume[(int)m_aData[nSoundIdx].category] *
				m_aSettingVolume [(int)m_aData[nSoundIdx].category]);
		}
		else {
			m_apSourceVoice[nSoundIdx][nCntPlay]->SetVolume(0.0f);
		}

		// 再生
		m_apSourceVoice[nSoundIdx][nCntPlay]->Start(0);

		break;
	}

	if (nCntPlay >= PLAY_MAX)
		return NONEDATA;

	return (nSoundIdx * PLAY_MAX) + nCntPlay;
}

//========================================
// 位置設定
//========================================
void CSound::SetPos(int nID, D3DXVECTOR3 pos) {
	int nSoundIdx = nID / PLAY_MAX;
	int nPlayIdx  = nID % PLAY_MAX;

	m_aState[nSoundIdx][nPlayIdx].pos = pos;
}

//========================================
// セグメント停止(ラベル指定)
//========================================
int CSound::Stop(int nID) {
	int nSoundIdx = nID / PLAY_MAX;
	int nPlayIdx  = nID % PLAY_MAX;

	if (m_apSourceVoice[nSoundIdx][nPlayIdx] == NULL)
		return NONEDATA;

	// 状態取得
	XAUDIO2_VOICE_STATE xa2state;
	m_apSourceVoice[nSoundIdx][nPlayIdx]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		m_apSourceVoice[nSoundIdx][nPlayIdx]->Stop(0);

		// オーディオバッファの削除
		m_apSourceVoice[nSoundIdx][nPlayIdx]->FlushSourceBuffers();
	}

	return NONEDATA;
}

//========================================
// セグメント停止(種類指定)
//========================================
void CSound::Stop(TYPE type) {
	for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
		if (m_apSourceVoice[type][nCntPlay] == NULL)
			return;

		// 状態取得
		XAUDIO2_VOICE_STATE xa2state;
		m_apSourceVoice[type][nCntPlay]->GetState(&xa2state);
		if (xa2state.BuffersQueued != 0)
		{// 再生中
			// 一時停止
			m_apSourceVoice[type][nCntPlay]->Stop(0);

			// オーディオバッファの削除
			m_apSourceVoice[type][nCntPlay]->FlushSourceBuffers();
		}
	}
}

//========================================
// セグメント停止(※カテゴリー指定)
//========================================
void CSound::Stop(CATEGORY category) {
	XAUDIO2_VOICE_STATE xa2state;

	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		if (m_aData[nCntSound].category == category)
		{// サウンドの分類が一致した時、
			for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
				// 状態取得
				m_apSourceVoice[nCntSound][nCntPlay]->GetState(&xa2state);

				if (xa2state.BuffersQueued != 0)
				{// 再生中の時、停止し、オーディオバッファを削除
					m_apSourceVoice[nCntSound][nCntPlay]->Stop(0);
					m_apSourceVoice[nCntSound][nCntPlay]->FlushSourceBuffers();
				}
			}
		}
	}
}

//========================================
// セグメント停止(全て)
//========================================
void CSound::Stop(void) {
	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {
		for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
			if (m_apSourceVoice[nCntSound][nCntPlay] != NULL) {
				m_apSourceVoice[nCntSound][nCntPlay]->Stop(0);
			}
		}
	}
}

//========================================
// 分類毎に音量変更
//========================================
void CSound::TypeVolumeChange(CATEGORY category, float fVolume) {
	// 音量を制御
	if (fVolume <= 0.0f) {
		fVolume = 0.0f;
	}
	else if (fVolume >= 1.0f) {
		fVolume = 1.0f;
	}

	// 音量を設定
	m_aCategoryVolume[(int)category] = fVolume;

	for (int nCntSound = 0; nCntSound < MAX; nCntSound++) {

		if (m_aData[nCntSound].category == category)
		{// サウンドの分類が一致した時、
			for (int nCntPlay = 0; nCntPlay < PLAY_MAX; nCntPlay++) {
				// 音量を設定
				m_apSourceVoice[nCntSound][nCntPlay]->SetVolume(m_aData[nCntSound].fVolume * m_aCategoryVolume[(int)category] * m_aSettingVolume[(int)category]);
			}
		}
	}
}

//========================================
// 設定音量を設定
//========================================
void CSound::SetSettingVolume(CATEGORY category) {
	// 分類に応じた設定音量を取得
	if (category == CATEGORY::BGM) {
		m_aSettingVolume[(int)category] = 1.0f;	// ToDo:設定処理追加したらそこから取得するようにする (float)GetSetting()->nBGMVolume / (float)VOLUME_STAGE;
	}
	else if (category == CATEGORY::SE) {
		m_aSettingVolume[(int)category] = 1.0f;	// ToDo:設定処理追加したらそこから取得するようにする
	}

	// 分類毎に音量を適用
	TypeVolumeChange(category, m_aCategoryVolume[(int)category]);
}