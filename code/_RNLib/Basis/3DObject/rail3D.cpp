//========================================
// 
// ���[��3D�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]���[��3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CRail3D::CRail3D(const String& loadPath) {

    // �N���A����
	m_points = NULL;
    Clear();

	// �ǂݍ��ݏ���
	Load(loadPath);
}

//========================================
// �f�X�g���N�^
//========================================
CRail3D::~CRail3D() {

    // �N���A����
    Clear();
}

//========================================
// �N���A����
//========================================
void CRail3D::Clear(void) {

    RNLib::Memory().Release(&m_points);
    m_pointNum = 0;
}

//========================================
// �ǂݍ��ݏ���
//========================================
void CRail3D::Load(const String& loadPath) {

	// �t�@�C�����J��
	if (RNLib::File().OpenLoadFile(loadPath, "Rail3DFile")) {
        while (RNLib::File().SearchLoop("END")) {
            if (RNLib::File().CheckIdentifier("points{")) {
                RNLib::File().Scan(_RNC_File::SCAN::USHORT, &m_pointNum);
                RNLib::Memory().Alloc(&m_points, m_pointNum);

                for (UShort cntPoint = 0; cntPoint < m_pointNum; cntPoint++) {
                    RNLib::File().Scan(_RNC_File::SCAN::POS3D, &m_points[cntPoint]);
                }
            }
        }

		// �t�@�C�������
		RNLib::File().CloseFile();
	}
}

//========================================
// �������ݏ���
//========================================
void CRail3D::Save(const String& savePath) {

    // �t�@�C�����J��
    if (RNLib::File().OpenSaveFile(savePath)) {
        fprintf(RNLib::File().GetFile(), "Rail3DFile\n");
        fprintf(RNLib::File().GetFile(), "points{ %d\n", m_pointNum);
        for (UShort cntPoint = 0; cntPoint < m_pointNum; cntPoint++) {
            fprintf(RNLib::File().GetFile(), "  %f %f %f\n", m_points[cntPoint].x, m_points[cntPoint].y, m_points[cntPoint].z);
        }
        fprintf(RNLib::File().GetFile(), "}\n");
        fprintf(RNLib::File().GetFile(), "END\n");

        // �t�@�C�������
        RNLib::File().CloseFile();
    }
}

//========================================
// �����Ɋ�Â��ă}�g���b�N�X���v�Z
//========================================
Matrix CRail3D::GetMtx(float rate) {
    
    if (m_pointNum < 3)
        return INITMATRIX;

    // �������[�v����
    RNLib::Number().LoopClamp(&rate, 1.0f, 0.0f);

    // �����Ɋ�Â���Catmull-Rom�X�v���C���Ȑ���̈ʒu�Ɛi�s�������v�Z
    Pos3D pos;
    Vector3D tangent;
    CalculateCatmullRomSpline(rate, pos, tangent);

    return RNLib::Matrix().ConvPosRotToMtx(pos, RNLib::Geometry().FindVecRot(tangent));
}

//========================================
// �|�C���g�����Z
//========================================
void CRail3D::AddPoint(void) {

    const UShort oldNum = m_pointNum++;
    RNLib::Memory().ReAlloc(&m_points, oldNum, m_pointNum);
    m_points[oldNum] = INITPOS3D;
}

//========================================
// �|�C���g�����Z
//========================================
void CRail3D::SubPoint(const UShort& idx) {

    if (idx < 0 || idx >= m_pointNum)
        return;

    for (UShort cntPoint = idx; cntPoint < m_pointNum - 1; cntPoint++) {
        m_points[cntPoint] = m_points[cntPoint + 1];
    }
    const UShort oldNum = m_pointNum--;
    RNLib::Memory().ReAlloc(&m_points, oldNum, m_pointNum);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]���[��3D�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// Catmull-Rom�X�v���C���Ȑ���̈ʒu���v�Z
//========================================
void CRail3D::CalculateCatmullRomSpline(float rate, Pos3D& pos, Vector3D& tangent) {

    const int    segment     = (int)((m_pointNum - 1) * rate); // �Z�O�����g�̑I��
    const float  segmentRate = (m_pointNum - 1) * rate - segment; // �Z�O�����g���̊���

    // Catmull-Rom�X�v���C���Ȑ��̌v�Z
    const Pos3D p0 = m_points[(segment > 0) ? (segment - 1) : 0];
    const Pos3D p1 = m_points[segment];
    const Pos3D p2 = m_points[(segment < m_pointNum - 1) ? (segment + 1) : (m_pointNum - 1)];
    const Pos3D p3 = m_points[(segment < m_pointNum - 2) ? (segment + 2) : (m_pointNum - 1)];

    const float t = segmentRate;
    const float t2 = t * t;
    const float t3 = t2 * t;

    pos = 0.5f * (
        (2.0f * p1) +
        (-p0 + p2) * t +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

    tangent = 0.5f * (
        (-p0 + p2) +
        (2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t +
        (-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t2);
}