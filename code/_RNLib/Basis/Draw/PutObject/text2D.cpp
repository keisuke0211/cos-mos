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
//==========| �e�L�X�g2D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CText2D::CText2D() {

	m_debugLogLine = 0;
}

//========================================
// �f�X�g���N�^
//========================================
CText2D::~CText2D() {

}

//========================================
// ����������
//========================================
void CText2D::Init(void) {

}

//========================================
// �I������
//========================================
void CText2D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CText2D::Update(void) {

	m_debugLogLine = 0;
}

//========================================
// �ݒu����
//========================================
CText2D::CRegistInfo* CText2D::Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Pos3D& pos, const Angle& angle, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText2D(0, pos, angle, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// �f�o�b�O���O�ݒu����
//========================================
void CText2D::PutDebugLog(const char* string) {

	// ���ォ�牺�ɂ����ăe�L�X�g2D��ݒu����
	Put(0, string, CText::ALIGNMENT::LEFT, 0, Pos3D(0.0f, 8.0f + m_debugLogLine * 16.0f, 0.0f), 0.0f, true)
		->SetSize(Size2D(16.0f, 16.0f));

	// �f�o�b�O���O�̍s�����Z
	m_debugLogLine++;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �o�^���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CText2D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
CText2D::CRegistInfo::~CRegistInfo() {

	// ������̃��������
	CMemory::Release(&m_string);
}

//========================================
// �p�����[�^�[�̃N���A����
//========================================
void CText2D::CRegistInfo::ClearParameter(void) {

	CMemory::Release(&m_string);
	m_alignment   = CText::ALIGNMENT::CENTER;
	m_fontIdx     = NONEDATA;
	m_pos         = INITD3DXVECTOR3;
	m_angle       = 0.0f;
	m_scaleOrSize = INITVECTOR2D;
	m_isScale     = false;
	m_col         = INITCOLOR;
	m_isZtest     = true;
}

//========================================
// �ݒu����(�|���S��2D)
//========================================
void CText2D::CRegistInfo::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// �t�H���g�f�[�^���擾
	CText::FontData fontData = RNLib::Text().GetFont(m_fontIdx);
	
	//----------------------------------------
	// ��/����/�Ԋu���Z�o
	//----------------------------------------
	float charWidth      = 0.0f;
	float charHeightHalf = 0.0f;
	if (m_isScale) {
		charWidth      = (RNLib::Texture().GetWidth (fontData.nTexIdx) / fontData.nPtnWidth ) * m_scaleOrSize.x;
		charHeightHalf = (RNLib::Texture().GetHeight(fontData.nTexIdx) / fontData.nPtnHeight) * m_scaleOrSize.y * 0.5f;
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
		case CText::ALIGNMENT::CENTER: {
			setPos.x += (leftShift + cntChar) * charWidth;
		}break;
		case CText::ALIGNMENT::LEFT: {
			setPos.x += cntChar * charWidth;
		}break;
		case CText::ALIGNMENT::RIGHT: {
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
			->SetTex(fontData.nTexIdx, (int)wstr[cntChar] - (int)fontData.nStartCode, fontData.nPtnWidth, fontData.nPtnHeight)
			->SetZTest(m_isZtest);
	}

	// wchar_t�^������̉��
	free(wstr);
}

//========================================
// �ʒu��ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetPos(const Pos3D& pos) {

	if (this == NULL)
		return NULL;

	m_pos = pos;

	return this;
}

//========================================
// �p�x��ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetAngle(const float& angle) {

	if (this == NULL)
		return NULL;

	m_angle = angle;

	return this;
}

//========================================
// �������ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	StrCpyDynamicMemory(&m_string, string);

	return this;
}

//========================================
// �z�u��ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetAlignment(const CText::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// �t�H���g�ԍ���ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// �F��ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// �g��{����ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// �傫����ݒ�
//========================================
CText2D::CRegistInfo* CText2D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}