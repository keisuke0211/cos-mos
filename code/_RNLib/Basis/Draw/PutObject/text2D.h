//========================================
// 
// テキスト2Dのヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../text.h"

//****************************************
// クラス定義
//****************************************
// テキスト2Dクラス
class CText2D {
public:
	//========== [[[ クラス定義 ]]]
	// 登録情報
	class CRegistInfo {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		void PutPolygon2D(const UShort& priority, const bool& isOnScreen);
		CRegistInfo* SetPos      (const Pos3D& pos);
		CRegistInfo* SetAngle    (const Angle& angle);
		CRegistInfo* SetString   (const char* string);
		CRegistInfo* SetAlignment(const CText::ALIGNMENT& alignment);
		CRegistInfo* SetFontIdx  (const short& fontIdx);
		CRegistInfo* SetCol      (const Color& col);
		CRegistInfo* SetScale    (const Scale2D scale);
		CRegistInfo* SetSize     (const Size2D size);

	private:
		// [[[ 変数宣言 ]]]
		char*            m_string;
		CText::ALIGNMENT m_alignment;
		short            m_fontIdx;
		Pos3D            m_pos;
		Angle            m_angle;
		Vector2D         m_scaleOrSize;
		bool             m_isScale;
		Color            m_col;
		bool             m_isZtest;
	};

	//========== [[[ 関数宣言 ]]]
	CText2D();
	~CText2D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Pos3D& pos, const Angle& angle, const bool& isOnScreen = false);
	void PutDebugLog(const char* string);

private:
	//========== [[[ 変数宣言 ]]]
	UShort m_debugLogLine;
};