//========================================
// 
// �G�t�F�N�g3D(�V�����_�[)�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�G�t�F�N�g3D(�V�����_�[)�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
CEffect3D_Cylinder::CEffect3D_Cylinder(const short& priority, const Pos3D& pos, const Rot3D& rot, const short& life, const EASE_TYPE& easeType, const _RNC_DrawState::ALPHA_BLEND_MODE& alphaBlendType) : CEffect3D_Base(priority, pos, rot, life, easeType, alphaBlendType) {

	m_fadeOutStartRate = 0.0f;
	m_spinZ			   = 0.0f;
	m_divisionNum	   = 3;
	m_texIdx		   = NONEDATA;
	m_texXNum          = 1;
	m_circleFront      = {};
	m_circleBack       = {};
}

//========================================
// �f�X�g���N�^
//========================================
CEffect3D_Cylinder::~CEffect3D_Cylinder() {

}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�G�t�F�N�g3D(�V�����_�[)�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �X�V����
//========================================
void CEffect3D_Cylinder::Update(void) {

	// �������擾
	const float rate = CountLife();
	const float rateOpp = 1.0f - rate;

	// ��]
	m_rot.z += m_spinZ * rateOpp;

	// �}�g���b�N�X���Z�o
	const Matrix mtx = RNLib::Matrix().ConvPosRotToMtx(m_pos, m_rot);

	// ���a/���s�����擾
	const float radiusA = (rate * m_circleFront.endRadius) + (rateOpp * m_circleFront.startRadius);
	const float radiusB = (rate * m_circleBack.endRadius) + (rateOpp * m_circleBack.startRadius);
	const float depthA = (rate * m_circleFront.endDepth) + (rateOpp * m_circleFront.startDepth);
	const float depthB = (rate * m_circleBack.endDepth) + (rateOpp * m_circleBack.startDepth);

	// ���_�̖@��/�ʒu�����쐬
	Normal3D* vtxNors = NULL;
	Pos3D* vtxPosesA = NULL;
	Pos3D* vtxPosesB = NULL;
	float* vtxTexX = NULL;
	RNLib::Memory().Alloc(&vtxNors, m_divisionNum);
	RNLib::Memory().Alloc(&vtxPosesA, m_divisionNum);
	RNLib::Memory().Alloc(&vtxPosesB, m_divisionNum);
	RNLib::Memory().Alloc(&vtxTexX, m_divisionNum);
	for (int cntDiv = 0; cntDiv < m_divisionNum; cntDiv++) {
		const float rate = (float)cntDiv / m_divisionNum;
		const float angle = -D3DX_PI + (rate * D3DX_PI_DOUBLE);
		vtxNors[cntDiv].x = sinf(angle);
		vtxNors[cntDiv].y = cosf(angle);
		vtxNors[cntDiv].z = 0.0f;
		vtxPosesA[cntDiv] = vtxNors[cntDiv] * radiusA;
		vtxPosesA[cntDiv].z = depthA;
		vtxPosesB[cntDiv] = vtxNors[cntDiv] * radiusB;
		vtxPosesB[cntDiv].z = depthB;
		vtxTexX[cntDiv] = rate * m_texXNum;
	}

	// �F���Z�o
	Color colA = m_circleFront.col;
	Color colB = m_circleBack.col;
	colA.a *= rateOpp;
	colB.a *= rateOpp;

	// �|���S��3D�ݒu����
	for (int cntDiv = 0; cntDiv < m_divisionNum; cntDiv++) {

		const int rightIdx = (cntDiv + 1) % m_divisionNum;
		float rightTexX = vtxTexX[rightIdx];
		if (rightTexX <= vtxTexX[cntDiv])
			rightTexX = (float)m_texXNum;

		RNLib::Polygon3D().Put(m_priority, mtx)
			->SetVtxPos(vtxPosesA[cntDiv], vtxPosesA[rightIdx], vtxPosesB[cntDiv], vtxPosesB[rightIdx])
			->SetVtxNor(vtxNors[cntDiv], vtxNors[rightIdx], vtxNors[cntDiv], vtxNors[rightIdx])
			->SetVtxCol(colA, colA, colB, colB)
			->SetZTest(false)
			->SetTex(m_texIdx, Pos2D(vtxTexX[cntDiv], 0.0f), Pos2D(rightTexX, 0.0f), Pos2D(vtxTexX[cntDiv], 1.0f), Pos2D(rightTexX, 1.0f))
			->SetCullingMode(_RNC_DrawState::CULLING_MODE::BOTH_SIDES)
			->SetAlphaBlendMode(m_alphaBlendMode);
	}

	// ���_�̖@��/�ʒu�������
	RNLib::Memory().Release(&vtxNors);
	RNLib::Memory().Release(&vtxPosesA);
	RNLib::Memory().Release(&vtxPosesB);
	RNLib::Memory().Release(&vtxTexX);
}