//========================================
// 
// サウンドの処理
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]サウンドクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSound::CSound() : m_playMgr("RN_PlayMgr") {

}

//========================================
// デストラクタ
//========================================
CSound::~CSound() {

}

//========================================
// 初期化処理
//========================================
void CSound::Init(void) {

	m_datas = NULL;
	for (int cntCategory = 0; cntCategory < (int)CATEGORY::MAX; m_categoryStates[cntCategory] = {}, cntCategory++);
	m_mic3DPos = INITPOS3D;
	m_XAudio2 = NULL;
	m_masteringVoice = NULL;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	if (FAILED(XAudio2Create(&m_XAudio2, 0))) {

		// エラーメッセージ
		RNLib::Window().Message_ERROR("XAudio2オブジェクトの作成に失敗しました。");

		// COMライブラリの終了処理
		CoUninitialize();

		return;
	}

	// マスターボイスの生成
	if (FAILED(m_XAudio2->CreateMasteringVoice(&m_masteringVoice))) {

		// エラーメッセージ
		RNLib::Window().Message_ERROR("マスターボイスの生成に失敗しました。");

		// XAudio2オブジェクトの開放
		m_XAudio2->Release();
		m_XAudio2 = NULL;

		// COMライブラリの終了処理
		CoUninitialize();

		return;
	}
}

//========================================
// 終了処理
//========================================
void CSound::Uninit(void) {

	// 再生マネージャーの全解放
	m_playMgr.ReleaseAll();

	// データの解放
	CMemory::ReleaseDouble(&m_datas, m_num);

	// マスターボイスの破棄
	if (m_masteringVoice != NULL) {
		m_masteringVoice->DestroyVoice();
		m_masteringVoice = NULL;
	}

	// XAudio2オブジェクトの開放
	if (m_XAudio2 != NULL) {
		m_XAudio2->Release();
		m_XAudio2 = NULL;
	}

	// COMライブラリの終了処理
	CoUninitialize();
}

//========================================
// 更新処理
//========================================
void CSound::Update(void) {

}

//========================================
// 読み込み処理
//========================================
short CSound::Load(const char* loadPath, short idx) {
	
	const UShort oldNum = m_num;
	const short  idxOld = idx;

	if (CRegist::Load(loadPath, idx)) 
	{// 読み込み成功
		// データのメモリ再確保
		CMemory::ReAllocDouble(&m_datas, oldNum, m_num);

		// データの破棄(番号指定の場合)
		if (idxOld != NONEDATA)
			m_datas[idx]->Release();

		DWORD shunkSize    = 0;
		DWORD chunkPos     = 0;
		DWORD fileCategory = 0;

		// サウンドデータファイルの生成
		HANDLE fileHandle = CreateFile(loadPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (fileHandle == INVALID_HANDLE_VALUE) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("サウンドデータファイルの生成に失敗しました。(1)\n%s", loadPath));
			
			return NONEDATA;
		}

		// ファイルポインタを先頭に移動
		if (SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("サウンドデータファイルの生成に失敗しました。(2)\n%s", loadPath));
			
			return NONEDATA;
		}

		// WAVEファイルのチェック
		if (FAILED(CheckChunk(fileHandle, 'FFIR', &shunkSize, &chunkPos))) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("WAVEファイルのチェックに失敗しました。(1)\n%s", loadPath));

			return NONEDATA;
		}

		// WAVEファイルのチェック
		if (FAILED(ReadChunkData(fileHandle, &fileCategory, sizeof(DWORD), chunkPos))) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("WAVEファイルのチェックに失敗しました。(2)\n%s", loadPath));

			return NONEDATA;
		}

		// WAVEファイルのチェック
		if (fileCategory != 'EVAW') {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("WAVEファイルのチェックに失敗しました。(3)\n%s", loadPath));

			return NONEDATA;
		}

		// フォーマットチェック
		if (FAILED(CheckChunk(fileHandle, ' tmf', &shunkSize, &chunkPos))) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("フォーマットのチェックに失敗しました。(1)\n%s", loadPath));

			return NONEDATA;
		}

		// フォーマットチェック
		if (FAILED(ReadChunkData(fileHandle, &m_datas[idx]->m_wfx, shunkSize, chunkPos))) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("フォーマットのチェックに失敗しました。(2)\n%s", loadPath));

			return NONEDATA;
		}

		// オーディオデータ読み込み
		if (FAILED(CheckChunk(fileHandle, 'atad', &m_datas[idx]->m_audioDataSize, &chunkPos))) {

			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("オーディオデータ読み込みに失敗しました。(1)\n%s", loadPath));
			
			return NONEDATA;
		}

		// オーディオデータ読み込み
		m_datas[idx]->m_audioData = (BYTE*)malloc(m_datas[idx]->m_audioDataSize);
		if (FAILED(ReadChunkData(fileHandle, m_datas[idx]->m_audioData, m_datas[idx]->m_audioDataSize, chunkPos))) {
			
			// エラーメッセージ
			RNLib::Window().Message_ERROR(CreateText("オーディオデータ読み込みに失敗しました。(2)\n%s", loadPath));

			return NONEDATA;
		}

		// バッファの値設定
		m_datas[idx]->m_audioBuffer.AudioBytes = m_datas[idx]->m_audioDataSize;
		m_datas[idx]->m_audioBuffer.pAudioData = m_datas[idx]->m_audioData;
		m_datas[idx]->m_audioBuffer.Flags      = XAUDIO2_END_OF_STREAM;
		m_datas[idx]->m_audioBuffer.LoopCount  = 0;

		// ファイルを閉じる
		CloseHandle(fileHandle);
	}

	return idx;
}

//========================================
// セグメント再生(再生中なら停止)
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
// 停止(分類指定)
//========================================
void CSound::StopCategory(const CATEGORY& category) {

	CSound::CPlay* play = NULL;
	while (m_playMgr.ListLoop((CObject**)&play)) {

		// 分類が一致している時、削除
		if (play->GetCategory() == category)
			play->Delete();
	}
}

//========================================
// 停止(全て)
//========================================
void CSound::StopAll(void) {
	
	CSound::CPlay* play = NULL;
	while (m_playMgr.ListLoop((CObject**)&play)) {

		// 削除
		play->Delete();
	}
}

//========================================
// 分類毎に音量変更
//========================================
void CSound::ChangeCategoryVolume(const CATEGORY& category, float& volume) {

	// 音量を制御
	if (volume < 0.0f)
		volume = 0.0f;
	else if (volume > 1.0f)
		volume = 1.0f;

	// 音量を設定
	m_categoryStates[(int)category].volume = volume * m_categoryStates[(int)category].settingVolume;
}

//========================================
// 設定音量変更
//========================================
void CSound::ChangeSetVolume(const CATEGORY& category, float& volume) {

	// 設定音量を制御
	if (volume < 0.0f)
		volume = 0.0f;
	else if (volume > 1.0f)
		volume = 1.0f;

	// 設定音量を設定
	m_categoryStates[(int)category].settingVolume = volume;
}

//========================================
// プレイオブジェクト取得
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
//==========| [非公開]サウンドクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// チャンクのチェック
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

	// ファイルポインタを先頭に移動
	if (SetFilePointer(fileHandle, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	while (hr == S_OK) {

		// チャンクの読み込み
		if (ReadFile(fileHandle, &chunkType, sizeof(DWORD), &read, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());

		// チャンクデータの読み込み
		if (ReadFile(fileHandle, &chunkDataSize, sizeof(DWORD), &read, NULL) == 0)
			hr = HRESULT_FROM_WIN32(GetLastError());

		switch (chunkType) {
		case 'FFIR':
			RIFFDataSize  = chunkDataSize;
			chunkDataSize = 4;

			// ファイルタイプの読み込み
			if (ReadFile(fileHandle, &fileType, sizeof(DWORD), &read, NULL) == 0)
				hr = HRESULT_FROM_WIN32(GetLastError());

			break;

		default:

			// ファイルポインタをチャンクデータ分移動
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
// チャンクデータの読み込み
//========================================
HRESULT CSound::ReadChunkData(HANDLE fileHandle, void *buffer, DWORD bufferSize, DWORD bufferOffset) {
	
	DWORD read;

	// ファイルポインタを指定位置まで移動
	if (SetFilePointer(fileHandle, bufferOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return HRESULT_FROM_WIN32(GetLastError());

	// データの読み込み
	if (ReadFile(fileHandle, buffer, bufferSize, &read, NULL) == 0)
		return HRESULT_FROM_WIN32(GetLastError());

	return S_OK;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]データクラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CSound::CData::CData() {

	m_audioData     = NULL;
	m_audioDataSize = 0;
	memset(&m_audioBuffer, 0, sizeof(XAUDIO2_BUFFER));
	memset(&m_wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
}

//========================================
// デストラクタ
//========================================
CSound::CData::~CData() {

	// 解放処理
	Release();
}

//========================================
// 解放処理
//========================================
void CSound::CData::Release(void) {

	// オーディオデータの開放
	if (m_audioData != NULL) {
		CMemory::Release(&m_audioData);
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]再生クラスのメンバ関数
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// 静的変数定義
//****************************************
UShort CSound::CPlay::ms_IDCount = 0;

//========================================
// コンストラクタ
//========================================
CSound::CPlay::CPlay(const short& sountIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos2D* pos2D, const Pos3D* pos3D, const float& dist) {

	// リストに追加
	RNLib::Sound().GetPlayMgr().AddList(this);

	// IDを設定
	m_ID = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % USHRT_MAX;

	m_soundIdx = sountIdx;
	m_volume   = volume;
	m_category = category;
	m_isLoop   = isLoop;
	m_dist     = dist;

	// 位置情報の確保
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

	// ソースボイスの生成
	if (SUCCEEDED(RNLib::Sound().GetXAudio2().CreateSourceVoice(&m_sourceVoice, &(RNLib::Sound().GetData(m_soundIdx).m_wfx.Format)))) {

		// オーディオバッファの登録
		m_sourceVoice->SubmitSourceBuffer(&RNLib::Sound().GetData(m_soundIdx).m_audioBuffer);

		// 音量を0にしておく
		m_sourceVoice->SetVolume(0.0f);

		// 再生
		m_sourceVoice->Start(0);
	}
	else {
		m_sourceVoice = NULL;
		Delete();
	}
}

//========================================
// デストラクタ
//========================================
CSound::CPlay::~CPlay() {
	
	// 位置情報の解放
	CMemory::Release(&m_pos2D);
	CMemory::Release(&m_pos3D);

	// リストから削除
	RNLib::Sound().GetPlayMgr().SubList(this);

	// 停止し、オーディオバッファの削除
	if (m_sourceVoice != NULL) {
		m_sourceVoice->Stop(0);
		m_sourceVoice->FlushSourceBuffers();
	}
}

//========================================
// 更新処理
//========================================
void CSound::CPlay::Update(void) {

	// ソースボイスがNULLであれば、自身を破棄して終了
	if (m_sourceVoice == NULL) {
		Delete();
		return;
	}

	// 状態を取得
	XAUDIO2_VOICE_STATE xa2state;
	m_sourceVoice->GetState(&xa2state);

	if (xa2state.BuffersQueued == 0)
	{// 再生中でない時、

		if (m_isLoop) 
		{// ループフラグが真なら再び再生
			m_sourceVoice->SubmitSourceBuffer(&RNLib::Sound().GetData(m_soundIdx).m_audioBuffer);	// オーディオバッファの登録
			m_sourceVoice->Start(0);
		}
		else 
		{// ループフラグが偽なら自身を削除
			Delete();
			return;
		}
	}

	if (m_pos2D != NULL) {

	}
	else if (m_pos3D != NULL) {

		// 距離の割合を求める
		float distRateOpp = 1.0f - (CGeometry::FindDistance(*m_pos3D, RNLib::Sound().GetMic3DPos()) / m_dist);
		if (distRateOpp <= 0.0f) {
			m_sourceVoice->SetVolume(0.0f);
		}
		else {
			// 音量を反映させる
			m_sourceVoice->SetVolume(m_volume * RNLib::Sound().GetCategoryState(m_category).volume * RNLib::Sound().GetCategoryState(m_category).settingVolume * distRateOpp);
		}
	}
	else {
		// 音量を反映させる
		m_sourceVoice->SetVolume(m_volume * RNLib::Sound().GetCategoryState(m_category).volume * RNLib::Sound().GetCategoryState(m_category).settingVolume);
	}
}