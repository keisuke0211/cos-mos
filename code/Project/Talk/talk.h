//================================================================================================
// 
// 会話処理[talk.h]
// Author:Hirasawa Shion
// 
//================================================================================================
#pragma once

//会話クラス
class CTalk {
public:
	//会話イベント
	enum class EVENT {
		NONE,             //イベント無し
		BEFORE_DEPARTURE, // 出発前（タイトル->ステージ選択の合間？
		EMERGENCY_LANDING,// 不時着陸（1-1の開始イベント
		ROCKET_FOUND,     // ロケット発見(1-3クリア後
		MAX,
	};

	CTalk();
	~CTalk();
	void Init(EVENT &Event);
	void Uninit(void);
	void Update(void);

	static CTalk *Create(EVENT Event);    // 会話イベント指定
	bool IsTalk(void) { return m_bTalk; } // 会話中かどうか取得

private:
	//会話イベントのファイルパス
	static const char *EVENT_FILE[(int)EVENT::MAX];

	static const int NEXT_INTERVAL = 20;//次の文字を表示するインターバル

	void DeleteLog(void);        //会話ログ削除
	void LoadTalk(EVENT &Event); //会話イベント読込

	void PopUpText(void); //文章表示
	void NextChar(void);  //次の文字を表示

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
	int    m_nTalkNumAll;   //最大会話数
	int    m_nTalkID;       //会話番号
	int    m_nNextInterval; //次の文字・会話を表示するまでのインターバル
	int    m_nStringNumAll; //現在の会話の最大文字数
	int    m_nStringNum;    //現在の会話の表示文字数
	int    m_bEndSpeak;     //発言終了（会話自体の終了ではない
	char   *m_pPopText;     //表示するテキスト
};