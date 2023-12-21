//================================================================================================
// 
// 会話処理[talk.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

#include "../System/words/font-text.h"

//会話クラス
class CTalk {
public:
	//会話イベント
	enum class EVENT {
		NONE,             //イベント無し
		BEFORE_DEPARTURE, // 出発前（タイトル->ステージ選択の合間
		OPENING_1_1,      // 不時着陸（1-1のOPイベント
		ROCKET_FOUND,     // ロケット発見(1-3クリア後
		MAX,
	};

	CTalk();
	~CTalk();
	void Init(EVENT &Event);
	void Uninit(void);
	void Update(void);

	// -- 設定 ------------------------------------------
	void SetPos(const Pos3D &pos) { m_pos = pos; }
	void SetSize(CFloat size)     { m_pFont.fTextSize = size; }

	// -- 取得 ------------------------------------------
	static CTalk *Create(EVENT Event);    // 会話イベント指定
	bool IsTalk(void) { return m_bTalk; } // 会話中かどうか取得

private:
	//会話イベントのファイルパス
	static const char *EVENT_FILE[(int)EVENT::MAX];

	static const int NEXT_POPUP_INTERVAL = 4; //次の文字を表示するインターバル
	static const int NEXT_SPEAK_INTERVAL = 40;//次の発言までのインターバル

	void DeleteLog(void);        //会話ログ削除
	void LoadTalk(EVENT &Event); //会話イベント読込

	void DeleteText(void);//表示するテキストのメモリ確保（引数がNULLなら開放のみ
	void NextSpeak(void); //次にしゃべるテキストの設定

	//会話情報
	struct Talk
	{
		char *pLog;   // 会話内容
		int  nTex;    // テクスチャ番号
		int TalkerID; // 会話しているプレイヤーID
	};

	static Talk *s_pTalk;   //会話内容
	static EVENT s_Event;   //イベント
	bool   m_bTalk;         //会話中かどうか
	CFontText *m_pText;
	FormFont   m_pFont;
	FormShadow m_pShadow;

	Pos3D  m_pos;
	Pos2D  m_size;
	int    m_nTalkNumAll; //最大会話数
	int    m_nTalkID;     //会話番号
	bool   m_bEndSpeak;   //発言終了（会話自体の終了ではない
};