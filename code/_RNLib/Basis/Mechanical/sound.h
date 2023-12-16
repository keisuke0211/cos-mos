//========================================
// 
// サウンドの処理
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "object.h"
#include "object-manager.h"

//****************************************
// クラス定義
//****************************************
// サウンドクラス
class _RNC_Sound :public CRegist {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class CATEGORY { BGM, SE, VOICE, MAX, };

	//========== [[[ クラス定義 ]]]
	// データクラス
	class CData {
	public:
		// [[[ 関数宣言 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ 変数宣言 ]]]
		BYTE*                m_audioData;
		DWORD                m_audioDataSize;
		XAUDIO2_BUFFER       m_audioBuffer;
		WAVEFORMATEXTENSIBLE m_wfx;
	};

	// 再生クラス
	class CPlay : public CObject {
	public:
		// [[[ 関数宣言 ]]]
		CPlay(const short& soundIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos2D* pos2D, const Pos3D* pos3D, const float& dist);
		~CPlay();
		void Update(void);
		void                 SetVolume     (const float& volume) { m_volume = volume; }
		short&               GetID         (void) { return m_ID; }
		CATEGORY&            GetCategory   (void) { return m_category; }
		UShort&              GetCount      (void) { return m_count; }
		IXAudio2SourceVoice& GetSourceVoice(void) { return *m_sourceVoice; }

	private:
		// [[[ 変数宣言 ]]]
		static short         ms_IDCount;
		short                m_ID;
		short                m_soundIdx;
		CATEGORY             m_category;
		float                m_volume;
		UShort               m_count;
		bool                 m_isLoop;
		Pos2D*               m_pos2D;
		Pos3D*               m_pos3D;
		float                m_dist;
		IXAudio2SourceVoice* m_sourceVoice;
	};

	//========== [[[ 関数宣言 ]]]
	_RNC_Sound();
	~_RNC_Sound();
	void Init(void);
	void Uninit(void);
	void Update(void);
	short          Load                (const char* loadPath, short idx = NONEDATA);
	short          Play                (const short& soundIdx, const CATEGORY& category, const float& volume, const bool& isLoop);
	short          Play                (const short& soundIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos2D& pos, const float& dist);
	short          Play                (const short& soundIdx, const CATEGORY& category, const float& volume, const bool& isLoop, const Pos3D& pos, const float& dist);
	void           StopCategory        (const CATEGORY& category);
	void           StopAll             (void);
	void           ChangeCategoryVolume(const CATEGORY& category, float& volume);
	void           SetMic3DPos         (const Pos3D& pos) { m_mic3DPos = pos; }
	CData&         GetData             (const short& idx) { return *m_datas[idx]; }
	CPlay&         GetPlay             (const short& ID);
	CObjectMgr&    GetPlayMgr          (void) { return m_playMgr; }
	float&         GetCategoryVolume   (const CATEGORY& category) { return m_categoryVolumes[(int)category]; }
	Pos3D&         GetMic3DPos         (void) { return m_mic3DPos; }
	IXAudio2&      GetXAudio2          (void) { return *m_XAudio2; }

private:
	//========== [[[ 関数宣言 ]]]
	HRESULT CheckChunk   (HANDLE fileHandle, DWORD format, DWORD* chunkSize, DWORD* chunkDataPosition);
	HRESULT ReadChunkData(HANDLE fileHandle, void* buffer, DWORD bufferSize, DWORD bufferOffset);

	//========== [[[ 変数宣言 ]]]
	CData**                 m_datas;
	CObjectMgr              m_playMgr;
	float                   m_categoryVolumes[(int)CATEGORY::MAX];
	Pos3D                   m_mic3DPos;
	IXAudio2*               m_XAudio2;
	IXAudio2MasteringVoice* m_masteringVoice;
};