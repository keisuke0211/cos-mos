//========================================
// 
// �`�揈��
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�`��}�l�[�W���[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_DrawMgr::_RNC_DrawMgr() {

	m_reAllocCount     = 0;
	for(int cnt = 0;cnt < 2;cnt++){
		m_isDrawPolygon2D [cnt] = true;
		m_isDrawPolygon3D [cnt] = true;
		m_isDrawModel     [cnt] = true;
		m_isDrawStaticMesh[cnt] = true;
	}
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_DrawMgr::~_RNC_DrawMgr() {

}

//========================================
// ����������
//========================================
void _RNC_DrawMgr::Init(const UShort& priorityMax) {

	// �ő�D��x��ۑ�
	if (priorityMax <= 0) {
		assert(false);
		m_priorityMax = 1;
	}
	else {
		m_priorityMax = priorityMax;
	}

	// �o�^/�`����̃������m��
	RNLib::Memory().Alloc(&m_drawInfoSum		 , m_priorityMax);
	RNLib::Memory().Alloc(&m_drawInfoSumOvr	     , m_priorityMax);
	RNLib::Memory().Alloc(&m_drawInfoSumScreen   , SCREEN_PRIORITY_MAX);
	RNLib::Memory().Alloc(&m_drawInfoSumScreenOvr, SCREEN_PRIORITY_MAX);
	RNLib::Memory().Alloc(&m_resistInfoSum	     , m_priorityMax);
	RNLib::Memory().Alloc(&m_resistInfoSumScreen , SCREEN_PRIORITY_MAX);

	// �o�^���̏����������m��
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
		m_resistInfoSum[cntPriority].InitAlloc();

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
		m_resistInfoSumScreen[cntPriority].InitAlloc();

	// ���_�o�b�t�@�̏�������
	_RNC_Polygon2D::CDrawInfo::InitCreateVertexBuffer();
	_RNC_Polygon3D::CDrawInfo::InitCreateVertexBuffer();
}

//========================================
// �I������
//========================================
void _RNC_DrawMgr::Uninit(void) {

	// �������
	Release();
}

//========================================
// �X�V����
//========================================
void _RNC_DrawMgr::Update(void) {

}

//========================================
// �`��J�n����
//========================================
void _RNC_DrawMgr::StartDraw(Device& device) {

	//----------------------------------------
	// �o�^����K�p
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// �o�^�������ɐݒu����
		PutBasedRegistInfo(m_resistInfoSum[cntPriority], cntPriority, false);

		// �o�^����`����ɕϊ�����
		ConvRegistInfoToDrawInfo(m_resistInfoSum[cntPriority], m_drawInfoSumOvr[cntPriority], device);
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// �o�^�������ɐݒu����
		PutBasedRegistInfo(m_resistInfoSumScreen[cntPriority], cntPriority, true);

		// �o�^����`����ɕϊ�����
		ConvRegistInfoToDrawInfo(m_resistInfoSumScreen[cntPriority], m_drawInfoSumScreenOvr[cntPriority], device);
	}

	//----------------------------------------
	// �`��������ւ���
	//----------------------------------------
	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// �`������㏑������
		m_drawInfoSum[cntPriority].Overwrite(&m_drawInfoSumOvr[cntPriority]);

		// �s�v�������j���ׁ̈A�Ċm��
		m_resistInfoSum[cntPriority].ReAlloc();
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// �`������㏑������
		m_drawInfoSumScreen[cntPriority].Overwrite(&m_drawInfoSumScreenOvr[cntPriority]);

		// �s�v�������j���ׁ̈A�Ċm��
		m_resistInfoSumScreen[cntPriority].ReAlloc();
	}

	// �|���S��2D�̒��_�o�b�t�@�Ċm��
	if (_RNC_Polygon2D::CDrawInfo::ms_allocPower > POLYGON2D_ALLOC_BASE_POWER) {

		for (int cntAlloc = POLYGON2D_ALLOC_BASE_POWER; cntAlloc < _RNC_Polygon2D::CDrawInfo::ms_allocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (_RNC_Polygon2D::CDrawInfo::ms_idxCount < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
				_RNC_Polygon2D::CDrawInfo::CreateVertexBuffer(allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				_RNC_Polygon2D::CDrawInfo::ms_allocNum = allocLine;
				_RNC_Polygon2D::CDrawInfo::ms_allocPower = cntAlloc;

				break;
			}
		}
	}

	// �|���S��3D�̒��_�o�b�t�@�Ċm��
	if (_RNC_Polygon3D::CDrawInfo::ms_allocPower > POLYGON3D_ALLOC_BASE_POWER) {

		for (int cntAlloc = POLYGON3D_ALLOC_BASE_POWER; cntAlloc < _RNC_Polygon3D::CDrawInfo::ms_allocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (_RNC_Polygon3D::CDrawInfo::ms_idxCount < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
				_RNC_Polygon3D::CDrawInfo::CreateVertexBuffer(allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				_RNC_Polygon3D::CDrawInfo::ms_allocNum = allocLine;
				_RNC_Polygon3D::CDrawInfo::ms_allocPower = cntAlloc;

				break;
			}
		}
	}

	// �ԍ��J�E���g������
	_RNC_Polygon2D::CDrawInfo::ms_idxCount = 0;
	_RNC_Polygon3D::CDrawInfo::ms_idxCount = 0;

	// ���_������
	AssignVertexInfo();

	//----------------------------------------
	// �`��
	//----------------------------------------
	// �`����J�n
	device->BeginScene();

	// [[[ �X�N���[���`�� ]]]
	CCamera::StartRenderingScreen(device);
	Draw(device, NULL, true);
}

//========================================
// �`��I������
//========================================
void _RNC_DrawMgr::EndDraw(Device& device) {

	{// [[[ �J�����`�� ]]]
		// �����_�����O�^�[�Q�b�g/Z�o�b�t�@/�r���[�|�[�g��ۑ�
		Surface  renderDef;
		Surface  ZBuffDef;
		Viewport viewPortDef;
		device->GetRenderTarget(0, &renderDef);
		device->GetDepthStencilSurface(&ZBuffDef);
		device->GetViewport(&viewPortDef);

		// �J����1��1�ɑ΂��`�悵�Ă���
		CCamera* camera = NULL;
		_RNC_CameraMgr& cameraMgr = RNSystem::GetCameraMgr();
		while (cameraMgr.ListLoop((CObject**)&camera)) {
			if (!camera->GetIsDraw())
				continue;
			camera->StartRendering(device);
			Draw(device, camera, false);
			camera->EndRendering(device);
			camera->SetIsDraw(false);
		}

		// �����_�����O�^�[�Q�b�g/Z�o�b�t�@/�r���[�|�[�g�����ɖ߂�
		device->SetRenderTarget(0, renderDef);
		device->SetDepthStencilSurface(ZBuffDef);
		device->SetViewport(&viewPortDef);
	}

	// �`��I��
	device->EndScene();

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	device->Present(NULL, NULL, NULL, NULL);
}

//========================================
// �������
//========================================
void _RNC_DrawMgr::Release(void) {

	for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++) {

		// �`��������
		m_drawInfoSum   [cntPriority].Release();
		m_drawInfoSumOvr[cntPriority].Release();
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSum   [cntPriority].m_polygon2D);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumOvr[cntPriority].m_polygon2D);

		// �o�^�������
		m_resistInfoSum      [cntPriority].Release();
	}

	for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++) {

		// �`��������
		m_drawInfoSumScreen   [cntPriority].Release();
		m_drawInfoSumScreenOvr[cntPriority].Release();
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumScreen   [cntPriority].m_polygon2D);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_model);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_polygon3D);
		RNLib::Memory().Release(&m_drawInfoSumScreenOvr[cntPriority].m_polygon2D);

		// �o�^�������
		m_resistInfoSumScreen[cntPriority].Release();
	}

	// �o�^/�`����̃��������
	RNLib::Memory().Release(&m_drawInfoSum);
	RNLib::Memory().Release(&m_drawInfoSumOvr);
	RNLib::Memory().Release(&m_drawInfoSumScreen);
	RNLib::Memory().Release(&m_drawInfoSumScreenOvr);
	RNLib::Memory().Release(&m_resistInfoSum);
	RNLib::Memory().Release(&m_resistInfoSumScreen);

	// ���_�o�b�t�@���������
	_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
	_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
}

//========================================
// �ݒu����(�|���S��2D)
//========================================
_RNC_Polygon2D::CRegistInfo* _RNC_DrawMgr::PutPolygon2D(const UShort& priority, const bool& isOnScreen) {

	// �D��x����`�F�b�N
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// �ԍ��J�E���g���ő吔�ɒB�������A���_�o�b�t�@���Đ�������
	if (_RNC_Polygon2D::CDrawInfo::ms_idxCount == _RNC_Polygon2D::CDrawInfo::ms_allocNum) {
		_RNC_Polygon2D::CDrawInfo::ms_allocPower++;
		_RNC_Polygon2D::CDrawInfo::ms_allocNum = pow(2, _RNC_Polygon2D::CDrawInfo::ms_allocPower);
		_RNC_Polygon2D::CDrawInfo::ReleaseVertexBuffer();
		_RNC_Polygon2D::CDrawInfo::CreateVertexBuffer(_RNC_Polygon2D::CDrawInfo::ms_allocNum);
	}

	// �o�^���
	_RNC_Polygon2D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon2D(m_resistInfoSumScreen[priority]) :
		RegistPolygon2D(m_resistInfoSum[priority]);

	// �ԍ�����
	registInfo->SetIdx(_RNC_Polygon2D::CDrawInfo::ms_idxCount++);

	return registInfo;
}

//========================================
// �ݒu����(�|���S��3D)
//========================================
_RNC_Polygon3D::CRegistInfo* _RNC_DrawMgr::PutPolygon3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {
	
	// �D��x����`�F�b�N
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// �ԍ��J�E���g���ő吔�ɒB�������A���_�o�b�t�@���Đ�������
	if (_RNC_Polygon3D::CDrawInfo::ms_idxCount == _RNC_Polygon3D::CDrawInfo::ms_allocNum) {
		_RNC_Polygon3D::CDrawInfo::ms_allocPower++;
		_RNC_Polygon3D::CDrawInfo::ms_allocNum = pow(2, _RNC_Polygon3D::CDrawInfo::ms_allocPower);
		_RNC_Polygon3D::CDrawInfo::ReleaseVertexBuffer();
		_RNC_Polygon3D::CDrawInfo::CreateVertexBuffer(_RNC_Polygon3D::CDrawInfo::ms_allocNum);
	}

	// �o�^���
	_RNC_Polygon3D::CRegistInfo* registInfo = isOnScreen ?
		RegistPolygon3D(m_resistInfoSumScreen[priority]) :
		RegistPolygon3D(m_resistInfoSum[priority]);

	// ������
	registInfo->SetIdx(_RNC_Polygon3D::CDrawInfo::ms_idxCount++);
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// �ݒu����(�e�L�X�g2D)
//========================================
_RNC_Text2D::CRegistInfo* _RNC_DrawMgr::PutText2D(const UShort& priority, const Pos2D& pos, const float& angle, const bool& isOnScreen) {

	// �D��x����`�F�b�N
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// �o�^���
	_RNC_Text2D::CRegistInfo* registInfo = isOnScreen ?
		RegistText2D(m_resistInfoSumScreen[priority]) :
		RegistText2D(m_resistInfoSum[priority]);

	// ������
	registInfo->SetPos(pos);
	registInfo->SetAngle(angle);

	return registInfo;
}

//========================================
// �ݒu����(�e�L�X�g3D)
//========================================
_RNC_Text3D::CRegistInfo* _RNC_DrawMgr::PutText3D(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// �D��x����`�F�b�N
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// �o�^���
	_RNC_Text3D::CRegistInfo* registInfo = isOnScreen ?
		RegistText3D(m_resistInfoSumScreen[priority]) :
		RegistText3D(m_resistInfoSum[priority]);

	// ������
	registInfo->SetMtx(mtx);

	return registInfo;
}

//========================================
// �ݒu����(���f��)
//========================================
_RNC_Model::CRegistInfo* _RNC_DrawMgr::PutModel(const UShort& priority, const Matrix& mtx, const bool& isOnScreen) {

	// �D��x����`�F�b�N
	if (!CheckPriority(priority, isOnScreen)) {
		assert(false);
		return NULL;
	}

	// �o�^���
	_RNC_Model::CRegistInfo* registInfo = isOnScreen ?
		RegistModel(m_resistInfoSumScreen[priority]) :
		RegistModel(m_resistInfoSum[priority]);

	// ������
	registInfo->SetMtx(mtx);

	return registInfo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�`��}�l�[�W���[�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �`�揈��
//========================================
void _RNC_DrawMgr::Draw(Device& device, CCamera* camera, const bool& isOnScreen) {

	// �r���[�}�g���b�N�X���擾
	Matrix viewMtx;
	device->GetTransform(D3DTS_VIEW, &viewMtx);

	// �`�悵�Ă���
	if (isOnScreen) {
		ExecutionDraw(device, camera, m_drawInfoSumScreen, viewMtx, isOnScreen);
	}
	else {
		ExecutionDraw(device, camera, m_drawInfoSum, viewMtx, isOnScreen);
	}
}

//========================================
// �`����s����
//========================================
void _RNC_DrawMgr::ExecutionDraw(Device& device, CCamera* camera, CDrawInfoSum*& drawInfo, Matrix& viewMtx, const bool& isOnSreen) {

	const short   cameraID         = camera == NULL ? NONEDATA : camera->GetID();
	const bool    isCameraClipping = camera == NULL ? false    : camera->GetIsClipping();
	const Pos3D   cameraPosV       = camera == NULL ? Pos3D(0.0f, 0.0f, 0.0f) : camera->GetPosV();
	const Pos3D   cameraPosR       = camera == NULL ? Pos3D(0.0f, 0.0f, 1.0f) : camera->GetPosR();
	const Scale2D cameraScale      = camera == NULL ? RNLib::Window().GetSize() : camera->GetScale2D();
	Matrix  cameraBillboardMtx;
	D3DXMatrixInverse(&cameraBillboardMtx, NULL, &viewMtx);
	cameraBillboardMtx._41 = 0.0f;
	cameraBillboardMtx._42 = 0.0f;
	cameraBillboardMtx._43 = 0.0f;

	// [[[ ���C�g3D�̐ݒ� ]]]
	bool isFog = false;
	if (camera == NULL) {
		CLight3D::AllDisable(device);
	}
	else {
		const short lightID = camera->GetLightID();
		if (lightID == NONEDATA) {
			CLight3D::AllDisable(device);
		}
		else {
			CLight3D* light = NULL;
			bool      isSetting = false;
			while (RNSystem::GetLight3DMgr().ListLoop((CObject**)&light)) {
				if (light->GetID() == lightID) {
					light->Setting(device, camera->GetOverwriteBGCol());
					isFog = light->GetIsFog();
					isSetting = true;
					break;
				}
			}

			if (!isSetting) {
				CLight3D::AllDisable(device);
			}
		}
	}

	const UShort priorityMax = isOnSreen ? SCREEN_PRIORITY_MAX : m_priorityMax;
	for (int cntPriority = 0; cntPriority < priorityMax; cntPriority++) {

		//----------------------------------------
		// ���f���`��
		//----------------------------------------
		if (m_isDrawModel[isOnSreen]) {
			for (int cntModel = 0; cntModel < drawInfo[cntPriority].m_modelNum; cntModel++) {

				if (drawInfo[cntPriority].m_model[cntModel] == NULL)
					continue;

				// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
				if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_model[cntModel]->m_clippingID != cameraID)
						continue;

				/*if (!CHitTest::XYZ::InPointToCameraView(CMatrix::ConvMtxToPos(drawInfo[cntPriority].m_model[cntModel]->m_mtx), cameraPosV, cameraPosR, cameraScale.x, cameraScale.y, D3DXToRadian(45.0f)))
					continue;*/

				// �{�̂̃��[���h�}�g���b�N�X�̐ݒ�
				device->SetTransform(D3DTS_WORLD, &drawInfo[cntPriority].m_model[cntModel]->m_mtx);

				// [[[ Z�e�X�g�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetIsZTest(device, drawInfo[cntPriority].m_model[cntModel]->m_isZTest);

				// [[[ ���C�e�B���O��L��/�����ɂ��� ]]]
				RNLib::DrawStateMgr().SetIsLighting(device, drawInfo[cntPriority].m_model[cntModel]->m_isLighting);

				// [[[ ��ԃ��[�h�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_model[cntModel]->m_interpolationMode);

				// �`��
				for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++) {

					// �}�e���A���̐ݒ�
					device->SetMaterial(&drawInfo[cntPriority].m_model[cntModel]->m_mats[cntMat]);

					// [[[ �e�N�X�`���̐ݒ� ]]]
					device->SetTexture(0, drawInfo[cntPriority].m_model[cntModel]->m_texes[cntMat]);

					// �`��
					drawInfo[cntPriority].m_model[cntModel]->m_mesh->DrawSubset(cntMat);
				}

				// �֊s���̕`��
				if (drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh != NULL) {

					// �}�e���A���̐ݒ�
					device->SetMaterial(&_RNC_Model::CDrawInfo::ms_outLineMat);

					// ����
					RNLib::DrawStateMgr().SetCullingMode(device, _RNC_DrawState::CULLING_MODE::BACK_SIDE);

					for (int cntMat = 0; cntMat < drawInfo[cntPriority].m_model[cntModel]->m_matNum; cntMat++)
						drawInfo[cntPriority].m_model[cntModel]->m_outLineMesh->DrawSubset(cntMat);

					// �\��
					RNLib::DrawStateMgr().SetCullingMode(device, _RNC_DrawState::CULLING_MODE::FRONT_SIDE);
				}
			}

			// �ϐݒ�̃��Z�b�g
			RNLib::DrawStateMgr().ResetVariableSetting(device);

			{// �}�e���A���̏�����
				const Material initMat = INITMATERIAL;
				device->SetMaterial(&initMat);
			}
		}

		//----------------------------------------
		// ���_�t�H�[�}�b�g�̐ݒ�
		//----------------------------------------
		device->SetFVF(FVF_VERTEX_3D);

		//----------------------------------------
		// �X�^�e�B�b�N���b�V���`��
		//----------------------------------------
		if (m_isDrawStaticMesh[isOnSreen]) {

			// ���[���h�}�g���b�N�X�̐ݒ�
			device->SetTransform(D3DTS_WORLD, &INITMATRIX);

			RNLib::StaticMesh().Draw(device, cntPriority, cameraID, isCameraClipping, isOnSreen);
		}

		//----------------------------------------
		// �|���S��3D
		//----------------------------------------
		if (_RNC_Polygon3D::CDrawInfo::ms_vtxBuff != NULL && m_isDrawPolygon3D[isOnSreen]) {

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			device->SetStreamSource(0, _RNC_Polygon3D::CDrawInfo::ms_vtxBuff, 0, sizeof(Vertex3D));

			for (int cntPolygon3D = 0; cntPolygon3D < drawInfo[cntPriority].m_polygon3DNum; cntPolygon3D++) {

				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D] == NULL)
					continue;

				// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_clippingID != cameraID)
						continue;

				// [[[ Z�e�X�g�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetIsZTest(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isZTest);

				// [[[ ���C�e�B���O��L��/�����ɂ��� ]]]
				RNLib::DrawStateMgr().SetIsLighting(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isLighting);

				// [[[ ���Z������L��/�����ɂ��� ]]]
				RNLib::DrawStateMgr().SetAlphaBlendMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_alphaBlendMode);

				// [[[ �J�����O�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetCullingMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_cullingMode);

				// [[[ ��ԃ��[�h�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_interpolationMode);

				// [[[ �r���{�[�h�t���O�ɉ����ă}�g���b�N�X��ݒ� ]]]
				if (drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_isBillboard)
					device->SetTransform(D3DTS_WORLD, &RNLib::Matrix().MultiplyMtx(cameraBillboardMtx, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_mtx));
				else
					device->SetTransform(D3DTS_WORLD, &drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_mtx);

				// [[[ �e�N�X�`���̐ݒ� ]]]
				Polygon2DAnd3D::SetTexture(device, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_tex, drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_texType);

				// �`��
				device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * drawInfo[cntPriority].m_polygon3D[cntPolygon3D]->m_idx, 2);
			}
		}

		// �ϐݒ�̃��Z�b�g
		RNLib::DrawStateMgr().ResetVariableSetting(device);

		//----------------------------------------
		// �|���S��2D
		//----------------------------------------
		if (_RNC_Polygon2D::CDrawInfo::ms_vtxBuff != NULL && m_isDrawPolygon2D[isOnSreen]) {

			// Z�e�X�g��OFF��
			RNLib::DrawStateMgr().SetIsZTest(device, false);

			// �t�H�O������
			if (isFog)
				RNLib::DrawStateMgr().SetIsFog(device, false);

			// ���_�t�H�[�}�b�g�̐ݒ�
			device->SetFVF(FVF_VERTEX_2D);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			device->SetStreamSource(0, _RNC_Polygon2D::CDrawInfo::ms_vtxBuff, 0, sizeof(Vertex2D));

			for (int cntPolygon2D = 0; cntPolygon2D < drawInfo[cntPriority].m_polygon2DNum; cntPolygon2D++) {

				if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D] == NULL)
					continue;

				// �N���b�s���OID���ΏۊO�ł���ΐ܂�Ԃ�
				if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != NONEDATA || isCameraClipping)
					if (drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_clippingID != cameraID)
						continue;

				// [[[ ��ԃ��[�h�̐ݒ� ]]]
				RNLib::DrawStateMgr().SetInterpolationMode(device, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_interpolationMode);

				// [[[ �e�N�X�`���̐ݒ� ]]]
				Polygon2DAnd3D::SetTexture(device, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_tex, drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_texType);

				// �|���S���̕`��
				device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * drawInfo[cntPriority].m_polygon2D[cntPolygon2D]->m_idx, 2);
			}

			// �t�H�O��߂�
			if (isFog)
				RNLib::DrawStateMgr().SetIsFog(device, true);
		}

		// �ϐݒ�̃��Z�b�g
		RNLib::DrawStateMgr().ResetVariableSetting(device);
	}
}

//========================================
// ���_���������
//========================================
void _RNC_DrawMgr::AssignVertexInfo(void) {

	// ���_2D���ɕϊ�
	if (_RNC_Polygon2D::CDrawInfo::ms_vtxBuff != NULL) {

		// ���_�o�b�t�@�����b�N
		Vertex2D* vtxs = NULL;
		_RNC_Polygon2D::CDrawInfo::ms_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
			ConvDrawInfoToVertex2DInfo(vtxs, m_drawInfoSum[cntPriority]);
		
		for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
			ConvDrawInfoToVertex2DInfo(vtxs, m_drawInfoSumScreen[cntPriority]);

		// ���_�o�b�t�@���A�����b�N
		_RNC_Polygon2D::CDrawInfo::ms_vtxBuff->Unlock();
	}

	// ���_3D���ɕϊ�
	if (_RNC_Polygon3D::CDrawInfo::ms_vtxBuff != NULL) {

		// ���_�o�b�t�@�����b�N
		Vertex3D* vtxs = NULL;
		_RNC_Polygon3D::CDrawInfo::ms_vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

		for (int cntPriority = 0; cntPriority < m_priorityMax; cntPriority++)
			ConvDrawInfoToVertex3DInfo(vtxs, m_drawInfoSum[cntPriority]);

		for (int cntPriority = 0; cntPriority < SCREEN_PRIORITY_MAX; cntPriority++)
			ConvDrawInfoToVertex3DInfo(vtxs, m_drawInfoSumScreen[cntPriority]);

		// ���_�o�b�t�@���A�����b�N
		_RNC_Polygon3D::CDrawInfo::ms_vtxBuff->Unlock();
	}
}

//========================================
// �`����𒸓_2D���ɕϊ�
//========================================
void _RNC_DrawMgr::ConvDrawInfoToVertex2DInfo(Vertex2D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntDrawInfo = 0; cntDrawInfo < drawInfoSum.m_polygon2DNum; cntDrawInfo++) {

		// ���_������
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon2D[cntDrawInfo]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < _RNC_Polygon2D::CDrawInfo::ms_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon2D[cntDrawInfo]->m_vtxs[cntVtx];
		}
	}
}

//========================================
// �`����𒸓_3D���ɕϊ�
//========================================
void _RNC_DrawMgr::ConvDrawInfoToVertex3DInfo(Vertex3D*& vtxs, CDrawInfoSum& drawInfoSum) {

	for (int cntPolygon3D = 0; cntPolygon3D < drawInfoSum.m_polygon3DNum; cntPolygon3D++) {

		// ���_������
		const int vtxStartIdx = 4 * drawInfoSum.m_polygon3D[cntPolygon3D]->m_idx;

		for (int cntVtx = 0; cntVtx < 4; cntVtx++) {
			const int vtxIdx = vtxStartIdx + cntVtx;
			assert(vtxIdx < _RNC_Polygon3D::CDrawInfo::ms_allocNum * 4);
			vtxs[vtxIdx] = drawInfoSum.m_polygon3D[cntPolygon3D]->m_vtxs[cntVtx];
		}
	}
}

//========================================
// �o�^����(�|���S��2D)
//========================================
_RNC_Polygon2D::CRegistInfo* _RNC_DrawMgr::RegistPolygon2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon2DRegistInfoNum == resistInfo.m_polygon2DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_polygon2DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_polygon2DRegistInfoAllocNum;
		resistInfo.m_polygon2DRegistInfoAllocNum = pow(2, resistInfo.m_polygon2DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		RNLib::Memory().ReAllocDouble(&resistInfo.m_polygon2DRegistInfos, oldAllocNum, resistInfo.m_polygon2DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon2DRegistInfos[resistInfo.m_polygon2DRegistInfoNum++];
}

//========================================
// �o�^����(�|���S��3D)
//========================================
_RNC_Polygon3D::CRegistInfo* _RNC_DrawMgr::RegistPolygon3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_polygon3DRegistInfoNum == resistInfo.m_polygon3DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_polygon3DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_polygon3DRegistInfoAllocNum;
		resistInfo.m_polygon3DRegistInfoAllocNum = pow(2, resistInfo.m_polygon3DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		RNLib::Memory().ReAllocDouble(&resistInfo.m_polygon3DRegistInfos, oldAllocNum, resistInfo.m_polygon3DRegistInfoAllocNum, true);
	}

	resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_polygon3DRegistInfos[resistInfo.m_polygon3DRegistInfoNum++];
}

//========================================
// �o�^����(�e�L�X�g2D)
//========================================
_RNC_Text2D::CRegistInfo* _RNC_DrawMgr::RegistText2D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text2DRegistInfoNum == resistInfo.m_text2DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_text2DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_text2DRegistInfoAllocNum;
		resistInfo.m_text2DRegistInfoAllocNum = pow(2, resistInfo.m_text2DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		RNLib::Memory().ReAllocDouble(&resistInfo.m_text2DRegistInfos, oldAllocNum, resistInfo.m_text2DRegistInfoAllocNum, true);
	}

	resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text2DRegistInfos[resistInfo.m_text2DRegistInfoNum++];
}

//========================================
// �o�^����(�e�L�X�g3D)
//========================================
_RNC_Text3D::CRegistInfo* _RNC_DrawMgr::RegistText3D(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_text3DRegistInfoNum == resistInfo.m_text3DRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_text3DRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_text3DRegistInfoAllocNum;
		resistInfo.m_text3DRegistInfoAllocNum = pow(2, resistInfo.m_text3DRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		RNLib::Memory().ReAllocDouble(&resistInfo.m_text3DRegistInfos, oldAllocNum, resistInfo.m_text3DRegistInfoAllocNum, true);
	}

	resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum]->ClearParameter();
	return resistInfo.m_text3DRegistInfos[resistInfo.m_text3DRegistInfoNum++];
}

//========================================
// �o�^����(���f��)
//========================================
_RNC_Model::CRegistInfo* _RNC_DrawMgr::RegistModel(CRegistInfoSum& resistInfo) {

	if (resistInfo.m_modelRegistInfoNum == resistInfo.m_modelRegistInfoAllocNum) {

		// �m�ۂׂ��搔�𑝂₷
		resistInfo.m_modelRegistInfoAllocPower++;

		// �m�ۂ��������Z�o
		const UShort oldAllocNum = resistInfo.m_modelRegistInfoAllocNum;
		resistInfo.m_modelRegistInfoAllocNum = pow(2, resistInfo.m_modelRegistInfoAllocPower);

		// ���̊m�ۃ��C�����m�ۂ�����
		RNLib::Memory().ReAllocDouble(&resistInfo.m_modelRegistInfos, oldAllocNum, resistInfo.m_modelRegistInfoAllocNum, true);
	}

	resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum]->ClearParameter();
	return resistInfo.m_modelRegistInfos[resistInfo.m_modelRegistInfoNum++];
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�`��}�l�[�W���[�N���X�̃X���b�h�p�����o�֐�
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]�o�^�������ɐݒu����
//========================================
void _RNC_DrawMgr::PutBasedRegistInfo(CRegistInfoSum& resistInfoSum, const UShort& priority, const bool& isOnScreen) {
	
	//----------------------------------------
	// �e�L�X�g3D����|���S��3D��ݒu
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text3DRegistInfoNum; cnt++) {
		resistInfoSum.m_text3DRegistInfos[cnt]->PutPolygon3D(priority, isOnScreen);
	}

	resistInfoSum.m_text3DRegistInfoNum = 0;

	//----------------------------------------
	// �e�L�X�g2D����|���S��2D��ݒu
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_text2DRegistInfoNum; cnt++) {
		resistInfoSum.m_text2DRegistInfos[cnt]->PutPolygon2D(priority, isOnScreen);
	}

	resistInfoSum.m_text2DRegistInfoNum = 0;
}

//========================================
// [�ÓI]�o�^����`����ɕϊ�����
//========================================
void _RNC_DrawMgr::ConvRegistInfoToDrawInfo(CRegistInfoSum& resistInfoSum, CDrawInfoSum& drawInfoSum, Device& device) {

	{// �`����̃������m��
		drawInfoSum.m_modelNum     = resistInfoSum.m_modelRegistInfoNum;
		drawInfoSum.m_polygon3DNum = resistInfoSum.m_polygon3DRegistInfoNum;
		drawInfoSum.m_polygon2DNum = resistInfoSum.m_polygon2DRegistInfoNum;
		RNLib::Memory().Alloc(&drawInfoSum.m_model    , drawInfoSum.m_modelNum    , true);
		RNLib::Memory().Alloc(&drawInfoSum.m_polygon3D, drawInfoSum.m_polygon3DNum, true);
		RNLib::Memory().Alloc(&drawInfoSum.m_polygon2D, drawInfoSum.m_polygon2DNum, true);
	}

	//----------------------------------------
	// ���f��
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_modelRegistInfoNum; cnt++) {
		drawInfoSum.m_model[cnt] = resistInfoSum.m_modelRegistInfos[cnt]->ConvToDrawInfo(device);
	}

	resistInfoSum.m_modelRegistInfoNum = 0;

	//----------------------------------------
	// �|���S��3D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon3DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon3D[cnt] = resistInfoSum.m_polygon3DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon3DRegistInfoNum = 0;

	//----------------------------------------
	// �|���S��2D
	//----------------------------------------
	for (int cnt = 0; cnt < resistInfoSum.m_polygon2DRegistInfoNum; cnt++) {
		drawInfoSum.m_polygon2D[cnt] = resistInfoSum.m_polygon2DRegistInfos[cnt]->ConvToDrawInfo();
	}

	resistInfoSum.m_polygon2DRegistInfoNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�`���񑍊��N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_DrawMgr::CDrawInfoSum::CDrawInfoSum() {

	m_model        = NULL;
	m_modelNum     = 0;
	m_polygon2D    = NULL;
	m_polygon2DNum = 0;
	m_polygon3D    = NULL;
	m_polygon3DNum = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_DrawMgr::CDrawInfoSum::~CDrawInfoSum() {

}

//========================================
// �������
//========================================
void _RNC_DrawMgr::CDrawInfoSum::Release(void) {

	// �`��������
	if (m_model != NULL) {
		for (int cnt = 0; cnt < m_modelNum; cnt++) {
			RNLib::Memory().Release(&m_model[cnt]);
		}
		RNLib::Memory().Release(&m_model);
	}
	
	if (m_polygon3D != NULL) {
		for (int cnt = 0; cnt < m_polygon3DNum; cnt++) {
			RNLib::Memory().Release(&m_polygon3D[cnt]);
		}
		RNLib::Memory().Release(&m_polygon3D);
	}

	if (m_polygon2D != NULL) {
		for (int cnt = 0; cnt < m_polygon2DNum; cnt++) {
			RNLib::Memory().Release(&m_polygon2D[cnt]);
		}
		RNLib::Memory().Release(&m_polygon2D);
	}
}

//========================================
// �㏑������
//========================================
void _RNC_DrawMgr::CDrawInfoSum::Overwrite(CDrawInfoSum* pOvr) {

	// �������
	Release();

	// �㏑��
	m_model        = pOvr->m_model;
	m_modelNum     = pOvr->m_modelNum;
	m_polygon3D    = pOvr->m_polygon3D;
	m_polygon3DNum = pOvr->m_polygon3DNum;
	m_polygon2D    = pOvr->m_polygon2D;
	m_polygon2DNum = pOvr->m_polygon2DNum;
	pOvr->m_model		 = NULL;
	pOvr->m_modelNum	 = 0;
	pOvr->m_polygon3D	 = NULL;
	pOvr->m_polygon3DNum = 0;
	pOvr->m_polygon2D	 = NULL;
	pOvr->m_polygon2DNum = 0;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�o�^����񑍊��N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_DrawMgr::CRegistInfoSum::CRegistInfoSum() {

	m_polygon2DRegistInfos          = NULL;
	m_polygon2DRegistInfoNum        = 0;
	m_polygon2DRegistInfoAllocPower = 0;
	m_polygon2DRegistInfoAllocNum   = 0;
	m_polygon3DRegistInfos          = NULL;
	m_polygon3DRegistInfoNum        = 0;
	m_polygon3DRegistInfoAllocPower = 0;
	m_polygon3DRegistInfoAllocNum   = 0;
	m_text2DRegistInfos             = NULL;
	m_text2DRegistInfoNum           = 0;
	m_text2DRegistInfoAllocPower    = 0;
	m_text2DRegistInfoAllocNum      = 0;
	m_text3DRegistInfos             = NULL;
	m_text3DRegistInfoNum           = 0;
	m_text3DRegistInfoAllocPower    = 0;
	m_text3DRegistInfoAllocNum      = 0;
	m_modelRegistInfos              = NULL;
	m_modelRegistInfoNum            = 0;
	m_modelRegistInfoAllocPower     = 0;
	m_modelRegistInfoAllocNum       = 0;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_DrawMgr::CRegistInfoSum::~CRegistInfoSum() {

}

//========================================
// �����m�ۏ���
//========================================
void _RNC_DrawMgr::CRegistInfoSum::InitAlloc(void) {

	m_polygon2DRegistInfos			= NULL;
	m_polygon3DRegistInfos			= NULL;
	m_text2DRegistInfos   			= NULL;
	m_text3DRegistInfos   			= NULL;
	m_modelRegistInfos    			= NULL;

	m_polygon2DRegistInfoAllocNum   = 
	m_polygon3DRegistInfoAllocNum   = 
	m_text2DRegistInfoAllocNum      = 
	m_text3DRegistInfoAllocNum      = 
	m_modelRegistInfoAllocNum       = 0;

	m_polygon2DRegistInfoAllocPower = 
	m_polygon3DRegistInfoAllocPower = 
	m_text2DRegistInfoAllocPower    = 
	m_text3DRegistInfoAllocPower    = 
	m_modelRegistInfoAllocPower     = 0;
}

//========================================
// �Ċm�ۏ���
//========================================
void _RNC_DrawMgr::CRegistInfoSum::ReAlloc(void) {

	//----------------------------------------
	// �|���S��2D
	//----------------------------------------
	if (m_polygon2DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_polygon2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon2DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				RNLib::Memory().ReAllocDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_polygon2DRegistInfoAllocNum   = allocLine;
				m_polygon2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �|���S��3D
	//----------------------------------------
	if (m_polygon3DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_polygon3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_polygon3DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				RNLib::Memory().ReAllocDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_polygon3DRegistInfoAllocNum   = allocLine;
				m_polygon3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �e�L�X�g2D
	//----------------------------------------
	if (m_text2DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_text2DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text2DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				RNLib::Memory().ReAllocDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_text2DRegistInfoAllocNum   = allocLine;
				m_text2DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// �e�L�X�g3D
	//----------------------------------------
	if (m_text3DRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_text3DRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_text3DRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				RNLib::Memory().ReAllocDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_text3DRegistInfoAllocNum   = allocLine;
				m_text3DRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}

	//----------------------------------------
	// ���f��
	//----------------------------------------
	if (m_modelRegistInfoAllocPower > 0) {

		for (int cntAlloc = 0; cntAlloc < m_modelRegistInfoAllocPower; cntAlloc++) {
			const UShort allocLine = pow(2, cntAlloc);

			if (m_modelRegistInfoNum < allocLine)
			{// �m�ۃ��C�������������Ȃ����A
				// ���̊m�ۃ��C�����m�ۂ�����
				RNLib::Memory().ReAllocDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum, allocLine);

				// �m�ۂ�����/�ׂ��搔��ۑ�
				m_modelRegistInfoAllocNum   = allocLine;
				m_modelRegistInfoAllocPower = cntAlloc;

				break;
			}
		}
	}
}

//========================================
// �������
//========================================
void _RNC_DrawMgr::CRegistInfoSum::Release() {

	RNLib::Memory().ReleaseDouble(&m_polygon2DRegistInfos, m_polygon2DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_polygon3DRegistInfos, m_polygon3DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_text2DRegistInfos, m_text2DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_text3DRegistInfos, m_text3DRegistInfoAllocNum);
	RNLib::Memory().ReleaseDouble(&m_modelRegistInfos, m_modelRegistInfoAllocNum);
}