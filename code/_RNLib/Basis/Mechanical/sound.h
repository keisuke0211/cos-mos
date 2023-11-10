//========================================
// 
// �T�E���h�̏���
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "object.h"
#include "object-manager.h"

//****************************************
// �N���X��`
//****************************************
// �T�E���h�N���X
class CSound :public CRegist {
public:
	//========== [[[ �񋓌^��` ]]]
	enum class CATEGORY { BGM, SE, MAX, };
	enum class SPACE    { NONE, _3D, _2D, };

	//========== [[[ �\���̒�` ]]]
	// ���ޖ��̏�ԍ\����
	struct CategoryState {
		float volume        = 1.0f;
		float settingVolume = 1.0f;
	};

	//========== [[[ �N���X��` ]]]
	// �f�[�^�N���X
	class CData {
	public:
		// [[[ �֐��錾 ]]]
		CData();
		~CData();
		void Release(void);

		// [[[ �ϐ��錾 ]]]
		BYTE*                m_audioData;
		DWORD                m_audioDataSize;
		XAUDIO2_BUFFER       m_audioBuffer;
		WAVEFORMATEXTENSIBLE m_wfx;
	};

	// �Đ��N���X
	class CPlay : public CObject {
	public:
		// [[[ �֐��錾 ]]]
		CPlay(const short& sountIdx, const CATEGORY& category, const bool& isLoop, const SPACE& space, const Pos3D& pos, const float& dist);
		~CPlay();
		void Update(void);
		CATEGORY&            GetCategory   (void) { return m_category; }
		IXAudio2SourceVoice& GetSourceVoice(void) { return *m_sourceVoice; }

	private:
		// [[[ �ϐ��錾 ]]]
		short                m_soundIdx;
		CATEGORY             m_category;
		bool                 m_isLoop;
		SPACE                m_space;
		Pos3D                m_pos;
		float                m_dist;
		IXAudio2SourceVoice* m_sourceVoice;
	};

	//========== [[[ �֐��錾 ]]]
	CSound();
	~CSound();
	void Init(void);
	void Uninit(void);
	void Update(void);
	short         Load                (const char* loadPath, short idx = NONEDATA);
	CPlay*        Play                (const short& sountIdx, const CATEGORY& category, const bool& isLoop, const SPACE& space, const Pos3D& pos, const float& dist);
	void          Stop                (const CATEGORY& category);	// (���ގw��)
	void          Stop                (void);						// (�S��)
	void          ChangeCategoryVolume(const CATEGORY& category, float& volume);
	void          SetMic3DPos         (const Pos3D& pos) { m_mic3DPos = pos; }
	CData&        GetData             (const short& idx) { return *m_datas[idx]; }
	CObjectMgr&   GetPlayMgr          (void) { return *m_playMgr; }
	CategoryState GetCategoryState    (const CATEGORY& category) { return m_categoryStates[(int)category]; }
	Pos3D&        GetMic3DPos         (void) { return m_mic3DPos; }
	IXAudio2&     GetXAudio2          (void) { return *m_XAudio2; }

private:
	//========== [[[ �֐��錾 ]]]
	HRESULT CheckChunk   (HANDLE fileHandle, DWORD format, DWORD* chunkSize, DWORD* chunkDataPosition);
	HRESULT ReadChunkData(HANDLE fileHandle, void* buffer, DWORD bufferSize, DWORD bufferOffset);

	//========== [[[ �ϐ��錾 ]]]
	CData**                 m_datas;
	CObjectMgr*             m_playMgr;
	CategoryState           m_categoryStates[(int)CATEGORY::MAX];
	Pos3D                   m_mic3DPos;
	IXAudio2*               m_XAudio2;
	IXAudio2MasteringVoice* m_masteringVoice;
};