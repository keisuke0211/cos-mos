//========================================
// 
// サウンドのヘッダファイル
// Author :AKIRA TANAKA
// Arrange:RIKU NISHIMURA
// 
//========================================
// [[[ sound.h ]]]
//========================================
#ifndef _SOUND_H_
#define _SOUND_H_

//****************************************
// クラス定義
//****************************************
// サウンドクラス
class CSound {
public:
	//========== [[[ 列挙型定義 ]]]
	enum class CATEGORY{
		BGM,
		SE,
		MAX,
	};

	typedef enum {
		SPACE_NONE,
		SPACE_3D,
		SPACE_MAX,
	}SPACE;

	typedef enum {
		DISTANCE_NONE,
		DISTANCE_5M,
		DISTANCE_25M,
		DISTANCE_50M,
		DISTANCE_100M,
		DISTANCE_MAX,
	}DISTANCE;

	typedef enum {
		// BGM
		BGM_DIGBUG,			// ディグバグ
		BGM_DIGBUG_BOSS,	// ディグバグ(ボス)
		// システムSE
		SE_MENU_SELECT,		// メニュー選択
		SE_MENU_DECIDE,		// メニュー決定
		SE_MENU_BACK,		// メニューを戻る
		SE_SCORE_COUNT,		// スコアカウント
		SE_GOLD_COUNT,		// ゴールドカウント
		SE_ITEM_GET,		// アイテム取得
		SE_GAMECLEAR,		// ゲームクリア
		SE_GAMEOVER,		// ゲームオーバー
		SE_TAKE_AIM,		// 狙いをつける
		// 戦闘SE
		SE_BLOW_DAMAGE_LOW,		// 打撃ダメージ(弱)
		SE_BLOW_DAMAGE_MID,		// 打撃ダメージ(中)
		SE_BLOW_DAMAGE_HIGH,	// 打撃ダメージ(強)
		SE_SLASH_DAMAGE_LOW,	// 斬撃ダメージ(弱)
		SE_SLASH_DAMAGE_MID,	// 斬撃ダメージ(中)
		SE_SLASH_DAMAGE_HIGH,	// 斬撃ダメージ(強)
		SE_SPC_DAMAGE_LOW,		// 特殊ダメージ(弱)
		SE_SPC_DAMAGE_MID,		// 特殊ダメージ(中)
		SE_ELEM_DAMAGE_FIRE,	// 火属性ダメージ
		SE_ELEM_DAMAGE_WATER,	// 水属性ダメージ
		SE_ELEM_DAMAGE_ICE,		// 氷属性ダメージ
		SE_ELEM_DAMAGE_WIND,	// 風属性ダメージ
		SE_ELEM_DAMAGE_ELEC,	// 雷属性ダメージ
		SE_ELEM_DAMAGE_SOIL,	// 地属性ダメージ
		SE_ELEM_DAMAGE_DARK,	// 闇属性ダメージ
		SE_ELEM_DAMAGE_LIGHT,	// 光属性ダメージ
		SE_IMPACT_MID,			// 衝撃(中)
		SE_BREATH_LOW,			// ブレス(弱)
		SE_BREATH_MID,			// ブレス(中)
		SE_BREATH_HIGH,			// ブレス(強)
		SE_BREATH_SHOT,			// ブレスショット
		// 環境SE
		SE_LANDING_GRASS,	// 着地(草)
		SE_LANDING_SOIL,	// 着地(土)
		SE_LANDING_STONE,	// 着地(石)
		SE_LANDING_SAND,	// 着地(砂)
		SE_LANDING_IMPACT,	// 着地の衝撃
		SE_EARTHQUAKE,		// 地震
		SE_BREAK_SOIL,		// 破壊(土)
		SE_BREAK_STONE,		// 破壊(石)
		SE_EXPLOSION_MID,	// 爆発(中)
		SE_SWING_LOW,		// 振り(弱)
		SE_SWING_MID,		// 振り(中)
		SE_SWING_HIGH,		// 振り(強)
		SE_METAL_STRIKE,	// 金属衝突
		SE_MOTOR,			// モーター
		MAX,
	}TYPE;

	//========== [[[ 構造体定義 ]]]
	typedef struct Data_ {
		char     aFileName[TXT_MAX];
		int      nCntLoop;
		float    fVolume;
		CATEGORY category;
	}Data;

	typedef struct State_ {
		bool        bPlay = false;
		SPACE       space = SPACE_NONE;
		D3DXVECTOR3 pos   = INITD3DXVECTOR3;
		DISTANCE    dist  = DISTANCE_NONE;
	}State;

	//========== [[[ 関数宣言 ]]]
	CSound();
	~CSound();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Load(void);
	int  Play(int nSoundIdx, SPACE space = SPACE_NONE, D3DXVECTOR3 pos = INITD3DXVECTOR3, DISTANCE dist = DISTANCE_NONE);
	int  Stop(int nID);				// (※ラベル指定)
	void Stop(TYPE type);			// (※種類指定)
	void Stop(CATEGORY category);	// (※カテゴリー指定)
	void Stop(void);				// (※全て)
	void SetPos(int nID, D3DXVECTOR3 pos);
	void SetMic3DPos(D3DXVECTOR3 pos) { m_mic3DPos = pos; }
	void SetSettingVolume(CATEGORY category);
	void TypeVolumeChange(CATEGORY category, float fVolume);

private:
	//========== [[[ 定数定義 ]]]
	static const int PLAY_MAX = 32;

	//========== [[[ 関数宣言 ]]]
	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

	//========== [[[ 変数宣言 ]]]
	Data  m_aData[MAX];
	float m_aCategoryVolume[(int)CATEGORY::MAX];	// 分類毎の音量
	float m_aSettingVolume [(int)CATEGORY::MAX];	// 分類毎の設定の音量
	State m_aState[MAX][PLAY_MAX];

	D3DXVECTOR3 m_mic3DPos;

	IXAudio2*               m_pXAudio2;	// XAudio2オブジェクトへのインターフェイス
	IXAudio2MasteringVoice* m_pMasteringVoice;
	IXAudio2SourceVoice*    m_apSourceVoice[MAX][PLAY_MAX];
	BYTE* m_apDataAudio[MAX];	// オーディオデータ
	DWORD m_aSizeAudio[MAX];	// オーディオデータサイズ
};

#endif
