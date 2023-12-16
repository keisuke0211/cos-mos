//========================================
// 
// �e�L�X�g2D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include <locale.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �e�L�X�g2D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Text2D::_RNC_Text2D() {

	m_isShowDebugLog        = true;
	m_isShowDebugLogOptions = false;
	m_debugLogLine          = 0;
	m_debugCount            = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Text2D::~_RNC_Text2D() {

}

//========================================
// ����������
//========================================
void _RNC_Text2D::Init(void) {

	// ���s���[�h�ł���΃f�o�b�O�\�����s��Ȃ�
	m_isShowDebugLog = (RNSystem::GetMode() != RNSystem::MODE::EXECUTION);
}

//========================================
// �I������
//========================================
void _RNC_Text2D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void _RNC_Text2D::Update(void) {

	m_debugLogLine = 0;

	// ���s���[�h�łȂ����A
	if (RNSystem::GetMode() != RNSystem::MODE::EXECUTION){

		// �f�o�b�O�\����ON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_F1))
			m_isShowDebugLog = !m_isShowDebugLog;

		// �e�f�o�b�O�\���I�v�V������ON/OFF
		if (RNLib::Input().GetKeyTrigger(DIK_F2))
			m_isShowDebugLogOptions = !m_isShowDebugLogOptions;

		// �e�f�o�b�O�\���I�v�V����
		RNLib::Text2D().PutDebugLog("----------StanderdInformation");
		RNLib::Text2D().PutDebugLog("HideDebugLog      [F1]:TRUE");
		RNLib::Text2D().PutDebugLog(CreateText("DefaultDebugOption[F2]:%s", m_isShowDebugLogOptions ? "TRUE" : "FALSE"));
		if (m_isShowDebugLogOptions) {
			RNLib::Text2D().PutDebugLog("----------Performance");
			RNLib::Text2D().PutDebugLog(CreateText("FPS    :%d", RNSystem::GetFPS()));
			RNLib::Text2D().PutDebugLog(CreateText("RunTime:%02d:%02d.%02d", RNLib::Count().GetCount() / 3600, (RNLib::Count().GetCount() / 60) % 60, RNLib::Count().GetCount() % 60));
			
			RNLib::Text2D().PutDebugLog("----------DrawState");
			RNLib::Text2D().PutDebugLog(CreateText("Polygon2DNum:%d", RNLib::DrawMgr().GetPolygon2DNum()));
			RNLib::Text2D().PutDebugLog(CreateText("Polygon3DNum:%d", RNLib::DrawMgr().GetPolygon3DNum()));
			RNLib::Text2D().PutDebugLog(CreateText("ModelNum    :%d", RNLib::DrawMgr().GetModelNum()));
			
			RNLib::Text2D().PutDebugLog("----------ObjectNumInObjectMgr");
			CObjectMgr**& objectMgrs   = CObjectMgr::GetObjectMgrs  ();
			UShort&       objectMgrNum = CObjectMgr::GetObjectMgrNum();
			for (int cntObjectMgr = 0; cntObjectMgr < objectMgrNum; cntObjectMgr++)
				PutDebugLog(CreateText("%d < %s", objectMgrs[cntObjectMgr]->GetNum(), objectMgrs[cntObjectMgr]->GetName()));

			RNLib::Text2D().PutDebugLog("----------Other");
			RNLib::Text2D().PutDebugLog(CreateText("DebugCount:%d", m_debugCount));
		}
		RNLib::Text2D().PutDebugLog("----------");
		RNLib::Text2D().PutDebugLog("");
	}
}

//========================================
// �ݒu����
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::Put(const UShort& priority, const char* string, const _RNC_Text::ALIGNMENT alignment, const short& fontIdx, const Pos2D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText2D(priority, pos, angle, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// �f�o�b�O���O�ݒu����
//========================================
Pos2D _RNC_Text2D::PutDebugLog(const char* string) {

	if (!m_isShowDebugLog)
		return INITPOS2D;

	setlocale(LC_ALL, "");
	size_t    length = strlen(string);
	wchar_t* wstr = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, string, length + 1);
	const int strLen = (int)wcslen(wstr);

	const float top = m_debugLogLine * 16.0f;
	const float bottom = (m_debugLogLine + 1) * 16.0f;
	const float right = strLen * 16.0f;
	free(wstr);

	RNLib::Polygon2D().Put(_RNC_DrawMgr::SCREEN_PRIORITY_DEBUG_LOG, true)
		->SetVtxPos(Pos2D(0.0f, top), Pos2D(right, top), Pos2D(0.0f, bottom), Pos2D(right, bottom))
		->SetVtxCol(Color(0, 0, 0, 100), Color(0, 0, 0, 100), Color(0, 0, 0, 100), Color(0, 0, 0, 100));

	// �ݒu�ʒu
	Pos2D putPos = Pos2D(0.0f, 8.0f + m_debugLogLine * 16.0f);

	// ���ォ�牺�ɂ����ăe�L�X�g2D��ݒu����
	Put(_RNC_DrawMgr::SCREEN_PRIORITY_DEBUG_LOG, string, _RNC_Text::ALIGNMENT::LEFT, NONEDATA, putPos, 0.0f, true)
		->SetSize(Size2D(16.0f, 16.0f));

	// �f�o�b�O���O�̍s�����Z
	m_debugLogLine++;

	return putPos;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �o�^���N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Text2D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Text2D::CRegistInfo::~CRegistInfo() {

	// ������̃��������
	RNLib::Memory().Release(&m_string);
}

//========================================
// �p�����[�^�[�̃N���A����
//========================================
void _RNC_Text2D::CRegistInfo::ClearParameter(void) {

	RNLib::Memory().Release(&m_string);
	m_alignment         = _RNC_Text::ALIGNMENT::CENTER;
	m_fontIdx           = NONEDATA;
	m_pos               = INITPOS2D;
	m_angle             = 0.0f;
	m_scaleOrSize       = INITVECTOR2D;
	m_isScale           = false;
	m_col               = INITCOLOR;
	m_interpolationMode = _RNC_DrawState::INTERPOLATION_MODE::NONE;
}

//========================================
// �ݒu����(�|���S��2D)
//========================================
void _RNC_Text2D::CRegistInfo::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// �t�H���g�f�[�^���擾
	_RNC_Text::FontData fontData = RNLib::Text().GetFontData(m_fontIdx);
	
	//----------------------------------------
	// ��/����/�Ԋu���Z�o
	//----------------------------------------
	float charWidth      = 0.0f;
	float charHeightHalf = 0.0f;
	if (m_isScale) {
		charWidth      = (RNLib::Texture().GetWidth (fontData.texIdx) / fontData.ptnX ) * m_scaleOrSize.x;
		charHeightHalf = (RNLib::Texture().GetHeight(fontData.texIdx) / fontData.ptnY) * m_scaleOrSize.y * 0.5f;
	}
	else {
		charWidth      = m_scaleOrSize.x;
		charHeightHalf = m_scaleOrSize.y * 0.5f;
	}

	//----------------------------------------
	// ���P�[����ݒ肵�ă}���`�o�C�g�����ɑΉ�
	//----------------------------------------
	setlocale(LC_ALL, "");

	//----------------------------------------
	// char�^�̕������wchar_t�^�̕�����ɕϊ�
	//----------------------------------------
	size_t   length = strlen(m_string);
	wchar_t* wstr   = (wchar_t*)malloc((length + 1) * sizeof(wchar_t));
	mbstowcs(wstr, m_string, length + 1);

	//----------------------------------------
	// �ꕶ�����ݒu���Ă���
	//----------------------------------------
	const int   strLen    = (int)wcslen(wstr);
	const float leftShift = strLen * -0.5f;
	const float topY      = m_pos.y - charHeightHalf;
	const float bottomY   = m_pos.y + charHeightHalf;
	const float rotX      = m_angle + D3DX_PI_HALF;
	const float rateXinX  = sinf(rotX);
	const float rateYinX  = cosf(rotX);
	const float rateXinY  = sinf(m_angle);
	const float rateYinY  = cosf(m_angle);
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// �J�E���g�̕������󔒂̎��A�܂�Ԃ�
		if (wstr[cntChar] == ' ') {
			continue;
		}

		// [[[ �\���`���ɉ������ݒ�ʒu�̐ݒ� ]]]
		Pos2D setPos = INITPOS2D;
		switch (m_alignment) {
		case _RNC_Text::ALIGNMENT::CENTER: {
			setPos.x += (leftShift + cntChar) * charWidth;
		}break;
		case _RNC_Text::ALIGNMENT::LEFT: {
			setPos.x += cntChar * charWidth;
		}break;
		case _RNC_Text::ALIGNMENT::RIGHT: {
			setPos.x += (-strLen + cntChar) * charWidth;
		}break;
		}
		
		// �|���S��2D��ݒu
		const float leftX   = m_pos.x + setPos.x;
		const float rightX  = m_pos.x + setPos.x + charWidth;
		const float XLeft   = rateXinX * leftX;
		const float XRight  = rateXinX * rightX;
		const float XTop    = rateXinY * topY;
		const float XBottom = rateXinY * bottomY;
		const float YLeft   = rateYinX * leftX;
		const float YRight  = rateYinX * rightX;
		const float YTop    = rateYinY * topY;
		const float YBottom = rateYinY * bottomY;
		RNLib::DrawMgr().PutPolygon2D(priority, isOnScreen)
			->SetVtxPos(
				Pos2D(XLeft  + XTop   , YTop    + YLeft ),
				Pos2D(XRight + XTop   , YTop    + YRight),
				Pos2D(XLeft  + XBottom, YBottom + YLeft ),
				Pos2D(XRight + XBottom, YBottom + YRight))
			->SetCol(m_col)
			->SetTex(fontData.texIdx, (int)wstr[cntChar] - (int)fontData.startCode, fontData.ptnX, fontData.ptnY)
			->SetInterpolationMode(m_interpolationMode);
	}

	// wchar_t�^������̉��
	free(wstr);
}

//========================================
// �ʒu��ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetPos(const Pos2D& pos) {

	if (this == NULL)
		return NULL;

	m_pos = pos;

	return this;
}

//========================================
// �p�x��ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetAngle(const float& angle) {

	if (this == NULL)
		return NULL;

	m_angle = angle;

	return this;
}

//========================================
// �������ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	StrCpyDynamicMemory(&m_string, string);

	return this;
}

//========================================
// �z�u��ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetAlignment(const _RNC_Text::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// �t�H���g�ԍ���ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// �F��ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// �g��{����ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// �傫����ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}

//========================================
// ��ԃ��[�h��ݒ�
//========================================
_RNC_Text2D::CRegistInfo* _RNC_Text2D::CRegistInfo::SetInterpolationMode(const _RNC_DrawState::INTERPOLATION_MODE& interpolationMode) {

	if (this == NULL)
		return NULL;

	m_interpolationMode = interpolationMode;

	return this;
}