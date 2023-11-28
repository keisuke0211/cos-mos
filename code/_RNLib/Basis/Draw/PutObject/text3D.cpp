//========================================
// 
// �e�L�X�g3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"
#include <locale.h>

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �e�L�X�g3D�N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CText3D::CText3D() {

}

//========================================
// �f�X�g���N�^
//========================================
CText3D::~CText3D() {

}

//========================================
// ����������
//========================================
void CText3D::Init(void) {

}

//========================================
// �I������
//========================================
void CText3D::Uninit(void) {

}

//========================================
// �X�V����
//========================================
void CText3D::Update(void) {

}

//========================================
// �ݒu����
//========================================
CText3D::CRegistInfo* CText3D::Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const Matrix& mtx, const bool& isOnScreen) {

	return RNLib::DrawMgr().PutText3D(priority, mtx, isOnScreen)
		->SetString(string)
		->SetAlignment(alignment)
		->SetFontIdx(fontIdx);
}

//========================================
// �ݒu����(�ʒu�ƌ����w��)
//========================================
CText3D::CRegistInfo* CText3D::Put(const UShort& priority, const char* string, const CText::ALIGNMENT alignment, const short& fontIdx, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, const bool& isOnScreen) {

	return Put(priority, string, alignment, fontIdx, CMatrix::ConvPosRotToMtx(pos, rot), isOnScreen);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �o�^���N���X�̃����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CText3D::CRegistInfo::CRegistInfo() {

	m_string = NULL;
	ClearParameter();
}

//========================================
// �f�X�g���N�^
//========================================
CText3D::CRegistInfo::~CRegistInfo() {

	// ������̃��������
	CMemory::Release(&m_string);
}

//========================================
// �p�����[�^�[�̃N���A����
//========================================
void CText3D::CRegistInfo::ClearParameter(void) {

	CMemory::Release(&m_string);
	m_alignment   = CText::ALIGNMENT::CENTER;
	m_fontIdx     = NONEDATA;
	m_mtx         = INITMATRIX;
	m_scaleOrSize = INITVECTOR2D;
	m_isScale     = false;
	m_col         = INITCOLOR;
	m_isZtest     = true;
	m_isLighting  = false;
	m_isBillboard = false;
}

//========================================
// �ݒu����(�|���S��3D)
//========================================
void CText3D::CRegistInfo::PutPolygon3D(const UShort& priority, const bool& isOnScreen) {

	// �t�H���g�w��Ȃ��̎��A�I��
	if (m_fontIdx < 0)
		return;

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
	const float topY      = charHeightHalf;
	const float bottomY   = -charHeightHalf;
	for (int cntChar = 0; cntChar < strLen; cntChar++) {

		// �J�E���g�̕������󔒂̎��A�܂�Ԃ�
		if (wstr[cntChar] == ' ')
			continue;

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

		// �|���S��3D��ݒu
		const float rightX = setPos.x + charWidth;
		RNLib::DrawMgr().PutPolygon3D(priority, m_mtx, isOnScreen)
			->SetVtxPos(
				Pos3D(setPos.x, topY   , 0.0f),
				Pos3D(rightX  , topY   , 0.0f),
				Pos3D(setPos.x, bottomY, 0.0f),
				Pos3D(rightX  , bottomY, 0.0f))
			->SetCol(m_col)
			->SetTex(fontData.nTexIdx, (int)wstr[cntChar] - (int)fontData.nStartCode, fontData.nPtnWidth, fontData.nPtnHeight)
			->SetZTest(m_isZtest)
			->SetLighting(m_isLighting)
			->SetBillboard(m_isBillboard);
	}

	// wchar_t�^������̉��
	free(wstr);
}

//========================================
// �}�g���b�N�X��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetMtx(const Matrix& mtx) {

	if (this == NULL)
		return NULL;

	m_mtx = mtx;

	return this;
}

//========================================
// �������ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetString(const char* string) {

	if (this == NULL)
		return NULL;

	StrCpyDynamicMemory(&m_string, string);

	return this;
}

//========================================
// �z�u��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetAlignment(const CText::ALIGNMENT& alignment) {

	if (this == NULL)
		return NULL;

	m_alignment = alignment;

	return this;
}

//========================================
// �t�H���g�ԍ���ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetFontIdx(const short& fontIdx) {

	if (this == NULL)
		return NULL;

	m_fontIdx = fontIdx;

	return this;
}

//========================================
// �F��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetCol(const Color& col) {

	if (this == NULL)
		return NULL;

	m_col = col;

	return this;
}

//========================================
// �g��{����ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetScale(const Scale2D scale) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = scale;
	m_isScale     = true;

	return this;
}

//========================================
// �傫����ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetSize(const Size2D size) {

	if (this == NULL)
		return NULL;

	m_scaleOrSize = size;
	m_isScale     = false;

	return this;
}

//========================================
// Z�e�X�g��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetZTest(const bool& isZTest) {

	if (this == NULL)
		return NULL;

	m_isZtest = isZTest;

	return this;
}

//========================================
// ���C�e�B���O��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetLighting(const bool& isLighting) {

	if (this == NULL)
		return NULL;

	m_isLighting = isLighting;

	return this;
}

//========================================
// �r���{�[�h��ݒ�
//========================================
CText3D::CRegistInfo* CText3D::CRegistInfo::SetBillboard(const bool& isBillboard) {

	if (this == NULL)
		return NULL;

	m_isBillboard = isBillboard;

	return this;
}