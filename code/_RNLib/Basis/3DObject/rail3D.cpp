//========================================
// 
// レール3Dの処理
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [公開]レール3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// コンストラクタ
//========================================
CRail3D::CRail3D(const String& loadPath) {

    // クリア処理
	m_points = NULL;
    Clear();

	// 読み込み処理
	Load(loadPath);
}

//========================================
// デストラクタ
//========================================
CRail3D::~CRail3D() {

    // クリア処理
    Clear();
}

//========================================
// クリア処理
//========================================
void CRail3D::Clear(void) {

    RNLib::Memory().Release(&m_points);
    m_pointNum = 0;
}

//========================================
// 読み込み処理
//========================================
void CRail3D::Load(const String& loadPath) {

	// ファイルを開く
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

		// ファイルを閉じる
		RNLib::File().CloseFile();
	}
}

//========================================
// 書き込み処理
//========================================
void CRail3D::Save(const String& savePath) {

    // ファイルを開く
    if (RNLib::File().OpenSaveFile(savePath)) {
        fprintf(RNLib::File().GetFile(), "Rail3DFile\n");
        fprintf(RNLib::File().GetFile(), "points{ %d\n", m_pointNum);
        for (UShort cntPoint = 0; cntPoint < m_pointNum; cntPoint++) {
            fprintf(RNLib::File().GetFile(), "  %f %f %f\n", m_points[cntPoint].x, m_points[cntPoint].y, m_points[cntPoint].z);
        }
        fprintf(RNLib::File().GetFile(), "}\n");
        fprintf(RNLib::File().GetFile(), "END\n");

        // ファイルを閉じる
        RNLib::File().CloseFile();
    }
}

//========================================
// 割合に基づいてマトリックスを計算
//========================================
Matrix CRail3D::GetMtx(float rate) {
    
    if (m_pointNum < 3)
        return INITMATRIX;

    // 割合ループ制御
    RNLib::Number().LoopClamp(&rate, 1.0f, 0.0f);

    // 割合に基づいてCatmull-Romスプライン曲線上の位置と進行方向を計算
    Pos3D pos;
    Vector3D tangent;
    CalculateCatmullRomSpline(rate, pos, tangent);

    return RNLib::Matrix().ConvPosRotToMtx(pos, RNLib::Geometry().FindVecRot(tangent));
}

//========================================
// ポイントを加算
//========================================
void CRail3D::AddPoint(void) {

    const UShort oldNum = m_pointNum++;
    RNLib::Memory().ReAlloc(&m_points, oldNum, m_pointNum);
    m_points[oldNum] = INITPOS3D;
}

//========================================
// ポイントを減算
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
//==========| [非公開]レール3Dクラス
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// Catmull-Romスプライン曲線上の位置を計算
//========================================
void CRail3D::CalculateCatmullRomSpline(float rate, Pos3D& pos, Vector3D& tangent) {

    const int    segment     = (int)((m_pointNum - 1) * rate); // セグメントの選択
    const float  segmentRate = (m_pointNum - 1) * rate - segment; // セグメント内の割合

    // Catmull-Romスプライン曲線の計算
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