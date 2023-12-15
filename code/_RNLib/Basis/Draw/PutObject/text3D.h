//========================================
// 
// �e�L�X�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../text.h"
#include "../draw-state.h"

//****************************************
// �N���X��`
//****************************************
// �e�L�X�g3D�N���X
class CText3D {
public:
	//========== [[[ �N���X��` ]]]
	// �o�^���
	class CRegistInfo {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		void PutPolygon3D(const UShort& priority, const bool& isOnScreen);
		CRegistInfo* SetMtx              (const Matrix& mtx);
		CRegistInfo* SetString           (const char* string);
		CRegistInfo* SetAlignment        (const CText::ALIGNMENT& alignment);
		CRegistInfo* SetFontIdx          (const short& fontIdx);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetScale            (const Scale2D scale);
		CRegistInfo* SetSize             (const Size2D size);
		CRegistInfo* SetZTest            (const bool& isZTest);
		CRegistInfo* SetLighting         (const bool& isLighting);
		CRegistInfo* SetBillboard        (const bool& isBillboard);
		CRegistInfo* SetInterpolationMode(const CDrawState::INTERPOLATION_MODE& interpolationMode);

	private:
		// [[[ �ϐ��錾 ]]]
		char*                          m_string;
		CText::ALIGNMENT               m_alignment;
		short                          m_fontIdx;
		Matrix                         m_mtx;
		Vector2D                       m_scaleOrSize;
		bool                           m_isScale;
		Color                          m_col;
		bool                           m_isZtest;
		bool                           m_isLighting;
		bool                           m_isBillboard;
		CDrawState::INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ �֐��錾 ]]]
	CText3D();
	~CText3D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Matrix& mtx,                              const bool& isOnScreen = false);
	CRegistInfo* Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const bool& isOnScreen = false);
};