//========================================
// 
// 文字設定
// 
//========================================
// *** words.h ***
//========================================
#ifndef _WORDS_H_
#define _WORDS_H_

#include "../../../_RNLib/RNlib.h"
#include "object\font-object.h"
#include "font.h"

//****************************************
// クラス
//****************************************
class CWords : public CFontObject
{
	// 各頂点の加減値
	const D3DXVECTOR3 CWords::m_Vtx[4] =
	{
		D3DXVECTOR3(-1.0f, -1.0f, 0.0f),
		D3DXVECTOR3(+1.0f, -1.0f, 0.0f),
		D3DXVECTOR3(-1.0f, +1.0f, 0.0f),
		D3DXVECTOR3(+1.0f, +1.0f, 0.0f),
	};

public:

	// ***** 構造体 *****

	//頂点情報【 2D 】
	struct VERTEX_2D
	{
		D3DXVECTOR3 pos;	//頂点座標
		float rhw;			//座標変換係数（1.0fで固定）
		D3DCOLOR col;		//頂点カラー
		D3DXVECTOR2 tex;	//テクスチャ座標
	};

	// ***** 関数 *****
	CWords(int nPriority = PRIO_TEXT);
	~CWords() override;

	// -- メイン ------------------------------------------
	/* 初期化	*/HRESULT Init(void);
	/* 終了		*/void Uninit(void);
	/* 更新		*/void Update(void);

	/* 描画		*/void Draw(void);

	// 目標位置まで移動
	void TargetMove(void);

	//--------------------------------------------------
	// 文字生成
	// 引数1  : const char *Text    / 文字
	// 引数2  : D3DXVECTOR3 pos     / 位置
	// 引数3  : D3DXVECTOR2 size    / サイズ
	// 引数4  : CFont::FONT Type　　/ フォント種類
	// 引数5  : Color col			/ 文字の色
	//--------------------------------------------------
	static CWords *CWords::Create(const char *Text, D3DXVECTOR3 pos, D3DXVECTOR3 Size, CFont::FONT Type, D3DXCOLOR col);

	// -- 設定 ------------------------------------------
	/* 位置		*/virtual void SetPos(const D3DXVECTOR3 &pos);
	/* 目標位置	*/void SetTargetPos(D3DXVECTOR3 pos, int Time = 0);
	/* 移動量	*/virtual void SetMove(const D3DXVECTOR3 &move);
	/* 文字色	*/void SetColor(D3DXCOLOR Collar);
	/* サイズ	*/void SetSize(D3DXVECTOR3 Size) { m_Info.size = Size; };
	/* 向き		*/void SetRot(D3DXVECTOR3 Rot) { m_Info.rot = Rot; };
	/* 移動量	*/void SetMoveRot(D3DXVECTOR3 inMoveRot) { m_Info.moveRot = inMoveRot; };
	/* 何番目の文字か	*/void SetLetterPop(int nData) { m_Info.nLetterPop = nData; }

	// -- 取得 ------------------------------------------
	/* 位置			*/virtual D3DXVECTOR3 GetPos() { return m_Info.pos; }
	/* 文字色		*/D3DXCOLOR GetColor() { return m_Info.col; };
	/* サイズ		*/D3DXVECTOR3 GetSize() { return m_Info.size; };
	/* 移動量		*/D3DXVECTOR3 &GetMove() { return m_Info.move; };
	/* 頂点バッファ	*/LPDIRECT3DVERTEXBUFFER9 &GetVtx() { return m_pVtxBuff; };
	/* 移動フラグ	*/bool IsMove() { return m_Info.bMove; }
	/* 移動終了		*/bool ISMoveEnd() { return m_Info.bMoveEnd; }

private:
	// ***** 構造体 *****

	// 文字情報
	struct Info
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 OldPos;
		D3DXVECTOR3 TargetPos;
		D3DXVECTOR3 DifferencePos;
		D3DXVECTOR3 rot;
		D3DXVECTOR3 size;
		D3DXVECTOR3 move;
		D3DXVECTOR3 moveRot;
		D3DXCOLOR col;

		int nCntMove;	// 移動のカウント
		int nMoveTime;	// 移動時間
		int nLetterPop;	// 何番目の文字か

		bool bMove;		// 移動中か
		bool bMoveEnd;	// 移動終了
	};

	// ***** 関数 *****
	/* 文字の設定 */ void SetWords(const char*Text, CFont::FONT Type);
	/* 頂点座標	  */ void SetVtxPos(void);

	/* 変数	*/
	Info m_Info;
	LPDIRECT3DTEXTURE9      m_pTex;     // テクスチャ情報
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; // 頂点バッファ
};

#endif