//========================================
// 
// �e�L�X�g2D�̃w�b�_�t�@�C��
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include "../text.h"
#include "../draw-state.h"

//****************************************
// �N���X��`
//****************************************
// �e�L�X�g2D�N���X
class CText2D {
public:
	//========== [[[ �N���X��` ]]]
	// �o�^���
	class CRegistInfo {
	public:
		// [[[ �֐��錾 ]]]
		CRegistInfo();
		~CRegistInfo();
		void ClearParameter(void);
		void PutPolygon2D(const UShort& priority, const bool& isOnScreen);
		CRegistInfo* SetPos              (const Pos2D& pos);
		CRegistInfo* SetAngle            (const Angle& angle);
		CRegistInfo* SetString           (const char* string);
		CRegistInfo* SetAlignment        (const CText::ALIGNMENT& alignment);
		CRegistInfo* SetFontIdx          (const short& fontIdx);
		CRegistInfo* SetCol              (const Color& col);
		CRegistInfo* SetScale            (const Scale2D scale);
		CRegistInfo* SetSize             (const Size2D size);
		CRegistInfo* SetInterpolationMode(const CDrawState::INTERPOLATION_MODE& interpolationMode);

	private:
		// [[[ �ϐ��錾 ]]]
		char*                          m_string;
		CText::ALIGNMENT               m_alignment;
		short                          m_fontIdx;
		Pos2D                          m_pos;
		Angle                          m_angle;
		Vector2D                       m_scaleOrSize;
		bool                           m_isScale;
		Color                          m_col;
		CDrawState::INTERPOLATION_MODE m_interpolationMode;
	};

	//========== [[[ �֐��錾 ]]]
	CText2D();
	~CText2D();
	void Init(void);
	void Uninit(void);
	void Update(void);
	CRegistInfo* Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Pos2D& pos, const Angle& angle, const bool& isOnScreen = false);
	Pos2D PutDebugLog(const char* string);
	void IncrementDebugCount(void) { m_debugCount++; }
	void DecrementDebugCount(void) { m_debugCount--; }

private:
	//========== [[[ �񋓌^��` ]]]
	enum class DEBUG_LOG_TYPE {
		PERFORMANCE,
		DRAW_STATE,
		OBJECT_NUM_IN_OBJECTMGR,
		OTHER,
		MAX,
	};

	//========== [[[ �ϐ��錾 ]]]
	bool   m_isShowDebugLog;
	bool   m_isShowDebugLogOptions;
	int    m_debugCount;
	UShort m_debugLogLine;
};