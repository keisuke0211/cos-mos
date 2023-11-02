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
		void PutPolygon2D(const bool& isOnScreen);
		CRegistInfo* SetPos              (const Pos3D& pos);
		CRegistInfo* SetAngle            (const Angle& angle);
		CRegistInfo* SetString           (const char* string);
		CRegistInfo* SetAlignment        (const CText::ALIGNMENT& alignment);
		CRegistInfo* SetFontIdx          (const short& fontIdx);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize_TexBaseScale(const float& scaleX, const float& scaleY);
		CRegistInfo* ExtendFixedTexX     (const float& rateX);
		CRegistInfo* ExtendFixedTexY     (const float& rateY);
		CRegistInfo* SetPriority         (const short& priority);

	private:
		// [[[ 変数宣言 ]]]
		char*            m_string;
		CText::ALIGNMENT m_alignment;
		short            m_fontIdx;
		float            m_scaleX;
		float            m_scaleY;
		bool             m_isFactScale;
		Pos3D            m_pos;
		Angle            m_angle;
		Color            m_col;
		bool             m_isZtest;
		bool             m_isLighting;
		bool             m_isBillboard;
		short            m_priority;
	};

	//========== [[[ 関数宣言 ]]]
	CText2D();
	~CText2D();
	CRegistInfo* Put(const Pos3D& pos, const Angle& angle, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen = false);
	void PutDebugLog(const char* string);
	void ClearDebugLog(void) { m_debugLogLine = 0; }

private:
	//========== [[[ 変数宣言 ]]]
	UShort m_debugLogLine;
};