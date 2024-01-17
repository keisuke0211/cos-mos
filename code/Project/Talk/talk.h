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

	//会話表示の種類
	enum class SHOWTYPE {
		Under = -1,// 画面中央下部に表示
		Wipe,      // モデルとセリフを表示
		Curtain,   // 画面上下に暗幕を用意してその上にセリフを表示
		MAX
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

	//テキストボックスの画面下部位置
	static const Pos3D TEXTBOX_UNDER_POS; 

	static CFloat CURTAIN_WIDTH;              // カーテンの幅
	static CFloat CURTAIN_HEIGHT;             // カーテンの高さ
	static CFloat CURTAIN_OVER_BEHIND_POS_Y;  // 上部カーテン位置
	static CFloat CURTAIN_BOTTOM_BEHIND_POS_Y;// 下部カーテン位置
	static CInt CURTAIN_COUNTER = 20;         // カーテンの表示ON/OFFのアニメーションカウンター
	static int s_CurtainCounter;              // アニメーションカウンター

	static CInt NEXT_POPUP_INTERVAL = 4; //次の文字を表示するインターバル
	static CInt NEXT_SPEAK_INTERVAL = 40;//次の発言までのインターバル
	static CInt AUTO_COUNTER = 100;       //発言終了から自動進行するまでのカウンター

	void DeleteLog(void);        //会話ログ削除
	void LoadTalk(EVENT &Event); //会話イベント読込

	void SetFontOption(const SHOWTYPE& type);

	void ShowType(void);  //表示方法を適用する
	void Auto(void);      //自動進行
	void DeleteText(void);//表示するテキストのメモリ確保（引数がNULLなら開放のみ
	void NextSpeak(void); //次にしゃべるテキストの設定
	void Skip(void);      //会話スキップ

	void SetCurtain(void);//暗幕設定

	//会話情報
	struct Talk
	{
		char *pLog;   // 会話内容
		int TalkerID; // 会話しているプレイヤーID
		SHOWTYPE type;// 描画方法
	};

	static Talk *s_pTalk; //会話内容
	static EVENT s_Event; //イベント
	bool   m_bTalk;       //会話中かどうか
	CFontText *m_pText;
	FormFont   m_pFont;

	Pos3D  m_pos;
	Pos2D  m_size;
	int    m_nTalkNumAll; //最大会話数
	int    m_nTalkID;     //会話番号
	bool   m_bAuto;       //自動進行フラグ
	int    m_nAutoCounter;//自動進行のカウンター
};