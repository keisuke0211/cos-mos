//========================================
// 
// テキスト3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../text.h"

//****************************************
// クラス定義
//****************************************
// テキスト3Dクラス
class CText3D {
public:
	//========== [[[ クラス定義 ]]]
	// 登録情報
	class CRegistInfo {
	public:
		// [[[ 関数宣言 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		void PutPolygon3D(const bool& isOnScreen);
		CRegistInfo* SetMtx              (const Matrix& mtx);
		CRegistInfo* SetString           (const char* string);
		CRegistInfo* SetAlignment        (const CText::ALIGNMENT& alignment);
		CRegistInfo* SetFontIdx          (const short& fontIdx);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetSize             (const float& width, const float& height);
		CRegistInfo* SetSize_TexBaseScale(const float& scaleX, const float& scaleY);
		CRegistInfo* ExtendFixedTexX     (const float& rateX);
		CRegistInfo* ExtendFixedTexY     (const float& rateY);
		CRegistInfo* SetZTest            (const bool& isZTest);
		CRegistInfo* SetLighting         (const bool& isLighting);
		CRegistInfo* SetBillboard        (const bool& isBillboard);
		CRegistInfo* SetPriority         (const short& priority);

	private:
		// [[[ 変数宣言 ]]]
		char*            m_string;
		CText::ALIGNMENT m_alignment;
		short            m_fontIdx;
		float            m_scaleX;
		float            m_scaleY;
		bool             m_isFactScale;
		Matrix       m_mtx;
		Color            m_col;
		bool             m_isZtest;
		bool             m_isLighting;
		bool             m_isBillboard;
		short            m_priority;
	};

	//========== [[[ 関数宣言 ]]]
	CText3D();
	~CText3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const Matrix& mtx, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen = false);
	CRegistInfo* Put(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const bool& isOnScreen = false);
};