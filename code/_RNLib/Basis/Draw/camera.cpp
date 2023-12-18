//========================================
// 
// �J�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"

//****************************************
// �}�N����`
//****************************************
#define VIB_DAMPING                  (0.75f)
#define VIB_MIN	                     (0.01f)
#define CONTROLLER_VIB_TIME_DIAMETER (5)
#define CONTROLLER_VIB_MAX    	     (4.0f)
#define INIT_RANS_RATE               (0.25f)
#define INIT_RANS_RATE_OPP           (0.75f)
#define SPIN_DAMP                    (0.15f)
#define ROT_X_MAX                    (D3DX_PI * 0.499999f)
#define ROT_X_MIN                    (D3DX_PI * -0.499999f)
#define INIT_RADIAN                  (45.0f)

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�J�����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//****************************************
// �ÓI�ϐ���`
//****************************************
short CCamera::ms_IDCount = 0;

//========================================
// �R���X�g���N�^
//========================================
CCamera::CCamera(const Size2D& scale2D) {

	// ���X�g�ɒǉ�
	RNSystem::GetCameraMgr().AddList(this);

	// ID��ݒ�
	m_ID       = ms_IDCount;
	ms_IDCount = (ms_IDCount + 1) % SHRT_MAX;

	// �ϐ���������
	m_posV            = INITPOS3D;
	m_posR            = INITPOS3D;
	m_posVib          = INITPOS3D;
	m_rot             = INITROT3D;
	m_spin            = INITVECTOR3D;
	m_size            = scale2D;
	m_dist            = 1.0f;
	m_radian          = INIT_RADIAN;
	m_radianGoal      = INIT_RADIAN;
	m_vibForce        = 0.0f;
	m_isPivotToPosV   = true;
	m_isFixed         = false;
	m_state           = STATE::NONE;
	m_stateInfo       = NULL;
	m_isDraw          = false;
	m_lightID         = NONEDATA;
	m_BGCol           = Color{0,0,0,0};
	m_isClipping      = false;
	m_motionBlur      = {};

	// �f�o�C�X���擾
	Device device = RNLib::Window().GetD3DDevice();

	// [[[ �̈�T�C�Y���Z�o ]]]
	float areaWidth;
	float areaHeight;
	{
		const float resolution = RNLib::Window().GetResolution();
		areaWidth  = scale2D.x * resolution;
		areaHeight = scale2D.y * resolution;
	}

	{// [[[ �����_�����O�ݒ� ]]]
		for (int cnt = 0; cnt < 2; cnt++) {

			// �����_�����O�^�[�Q�b�g�p�e�N�X�`���̐���
			device->CreateTexture(areaWidth, areaHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_MTInfo.textures[cnt], NULL);

			// �e�N�X�`�������_�����O�p�C���^�[�t�F�[�X�̐���
			m_MTInfo.textures[cnt]->GetSurfaceLevel(0, &m_MTInfo.renders[cnt]);
		}

		// �e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
		device->CreateDepthStencilSurface(areaWidth, areaHeight, D3DFMT_D16, D3DMULTISAMPLE_NONE, 0, TRUE, &m_MTInfo.ZBuff, NULL);

		// �r���[�|�[�g�̐ݒ�
		m_MTInfo.viewport.X      = 0;
		m_MTInfo.viewport.Y      = 0;
		m_MTInfo.viewport.Width  = areaWidth;
		m_MTInfo.viewport.Height = areaHeight;
		m_MTInfo.viewport.MinZ   = 0.0f;
		m_MTInfo.viewport.MaxZ   = 1.0f;
	}

	//----------------------------------------
	// ���_�o�b�t�@�̐ݒ�
	//----------------------------------------
	// ���_�o�b�t�@�̐���
	device->CreateVertexBuffer(sizeof(Vertex2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_MTInfo.vtxBuff, NULL);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	Vertex2D* vtxs;
	m_MTInfo.vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

	// [[[ ���_�ʒu�̐ݒ� ]]]
	RNLib::Polygon2D().SetVtxPos_TopLeft(vtxs, INITPOS2D, scale2D.x, scale2D.y);

	// [[[ ���_�J���[�̐ݒ� ]]]
	RNLib::Polygon2D().SetVtxCol(vtxs, Color{ 255,255,255,0 });

	// [[[ �e�N�X�`�����W�̐ݒ� ]]]
	RNLib::Polygon2D().SetVtxTex(vtxs);

	// [[[ RHW�̐ݒ� ]]]
	RNLib::Polygon2D().SetVtxRHW(vtxs);

	// ���_���W���A�����b�N����
	m_MTInfo.vtxBuff->Unlock();
}

//========================================
// �f�X�g���N�^
//========================================
CCamera::~CCamera() {

	// ���X�g����폜
	RNSystem::GetCameraMgr().SubList(this);

	// ��ԏI������
	ProcessState(PROCESS::UNINIT);

	// �����_�����O�^�[�Q�b�g�p�e�N�X�`�����
	for (int cnt = 0; cnt < 2; cnt++) {
		if (m_MTInfo.textures[cnt] != NULL) {
			m_MTInfo.textures[cnt]->Release();
			m_MTInfo.textures[cnt] = NULL;
		}
	}

	// �t�B�[�h�o�b�N�p�|���S���̒��_�o�b�t�@�̔j��
	if (m_MTInfo.vtxBuff != NULL) {
		m_MTInfo.vtxBuff->Release();
		m_MTInfo.vtxBuff = NULL;
	}
}

//========================================
// �X�V����
//========================================
void CCamera::Update(void) {

	// [[[ �U�� ]]]
	if (m_vibForce >= VIB_MIN) {
		if (RNLib::Count().GetBlinkF2()) {
			float fAngle = -D3DX_PI + RNLib::Number().GetRandomFloat(1.0f) * D3DX_PI_DOUBLE;
			m_posVib.x = sinf(fAngle) * m_vibForce;
			m_posVib.y = cosf(fAngle) * m_vibForce;
			m_vibForce *= VIB_DAMPING;
		}
	}
	else {
		m_posVib = INITPOS3D;
	}

	// [[[ �Œ�Ȃ�I�� ]]]
	if (m_isFixed)
		return;

	// [[[ ��ԍX�V���� ]]]
	ProcessState(PROCESS::UPDATE);

	// [[[ ��] & ��]�ʌ��� ]]]
	m_rot  += m_spin;
	m_spin += -m_spin * SPIN_DAMP;

	// [[[ �����𐧌� ]]]
	RNLib::Number().Clamp(&m_rot.x, ROT_X_MAX, ROT_X_MIN);
	RNLib::Number().LoopClamp(&m_rot.y, D3DX_PI, -D3DX_PI);

	// [[[ ���_/�����_�ʒu���Z�o ]]]
	// ��]�������_   > �����_�ʒu���Z�o
	// ��]���������_ > ���_�ʒu  ���Z�o
	m_isPivotToPosV ?
		m_posR = m_posV + RNLib::Geometry().FindRotVec(m_rot) * m_dist :
		m_posV = m_posR - RNLib::Geometry().FindRotVec(m_rot) * m_dist;

	// [[[ ���W�A������ ]]]
	m_radian = (m_radian * INIT_RANS_RATE_OPP) + (m_radianGoal * INIT_RANS_RATE);

	//----------------------------------------
	// ���_�o�b�t�@�̌�i
	//----------------------------------------
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	Vertex2D* vtxs;
	m_MTInfo.vtxBuff->Lock(0, 0, (void**)&vtxs, 0);

	{// [[[ ���_�ʒu�̐ݒ� ]]]
		const float width  = (m_MTInfo.viewport.Width  / RNLib::Window().GetResolution());
		const float height = m_MTInfo.viewport.Height / RNLib::Window().GetResolution();

		RNLib::Polygon2D().SetVtxPos(vtxs, Pos2D(width * 0.5f, height * 0.5f), m_motionBlur.angle, width * m_motionBlur.scale, height * m_motionBlur.scale * 0.99f);
	}

	// [[[ ���_�J���[�̐ݒ� ]]]
	RNLib::Polygon2D().SetVtxCol(vtxs, Color{ m_motionBlur.col.r,m_motionBlur.col.g,m_motionBlur.col.b,(UShort)(m_motionBlur.col.a * m_motionBlur.power) });

	// ���_���W���A�����b�N����
	m_MTInfo.vtxBuff->Unlock();
}

//========================================
// [�ÓI]�X�N���[���̃����_�����O�J�n����
//========================================
void CCamera::StartRenderingScreen(Device& device) {

	// [[[ ��ʂ��N���A ]]]
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0);

	// [[[ �r���[�}�g���b�N�X�̍쐬 ]]]
	Matrix mtxView = INITMATRIX; {
		Pos3D posV = Pos3D(0.0f, 0.0f, 0.0f);
		Pos3D posR = Pos3D(0.0f, 0.0f, 1.0f);
		Pos3D posU = Pos3D(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}

	// [[[ �r���[�}�g���b�N�X�̐ݒ� ]]]
	device->SetTransform(D3DTS_VIEW, &mtxView);

	// [[[ �v���W�F�N�V�����}�g���b�N�X�̃Z�b�g�A�b�v ]]]
	SetUpProjectionMatrix(device, Scale2D(RNLib::Window().GetAreaWidth(), RNLib::Window().GetAreaHeight()), INIT_RADIAN);
}

//========================================
// �����_�����O�J�n����
//========================================
void CCamera::StartRendering(Device& device) {

	// [[[ �����_�����O�ݒ� ]]]
	// �����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	device->SetRenderTarget(0, m_MTInfo.renders[0]);

	// Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	device->SetDepthStencilSurface(m_MTInfo.ZBuff);

	// �r���[�|�[�g��ݒ�
	device->SetViewport(&m_MTInfo.viewport);

	// [[[ ��ʂ��N���A ]]]
	device->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), m_BGCol, 1.0f, 0);

	// [[[ �r���[�}�g���b�N�X�̍쐬 ]]]
	Matrix mtxView = INITMATRIX; {
		Pos3D posV = m_posV;
		Pos3D posR = m_posR;

		// �U���ʒu�ɕϓ������鎞�A
		if (m_posVib != INITD3DXVECTOR3) {
			float angle = RNLib::Geometry().FindAngleXZ(m_posV, m_posR) - D3DX_PI_HALF;
			Pos3D addPos;
			addPos.x = sinf(angle) * m_posVib.x;
			addPos.z = cosf(angle) * m_posVib.x;
			addPos.y = m_posVib.y;

			// ���_/�����_�ɉ��Z�ʒu��K�p
			posV += addPos;
			posR += addPos;
		}

		// �r���[�}�g���b�N�X�̍쐬
		Pos3D posU = Pos3D(0.0f, 1.0f, 0.0f);
		D3DXMatrixLookAtLH(&mtxView, &posV, &posR, &posU);
	}
	
	// [[[ �r���[�}�g���b�N�X�̐ݒ� ]]]
	device->SetTransform(D3DTS_VIEW, &mtxView);

	// [[[ �v���W�F�N�V�����}�g���b�N�X�̃Z�b�g�A�b�v ]]]
	SetUpProjectionMatrix(device, Scale2D(m_MTInfo.viewport.Width, m_MTInfo.viewport.Height), m_radian);
}

//========================================
// �X�N���[���̃����_�����O�I������
//========================================
void CCamera::EndRendering(Device& device) {

	// ���_�t�H�[�}�b�g�̐ݒ�
	device->SetFVF(FVF_VERTEX_2D);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	device->SetStreamSource(0, m_MTInfo.vtxBuff, 0, sizeof(Vertex2D));

	// �e�N�X�`���̐ݒ�
	device->SetTexture(0, m_MTInfo.textures[1]);

	// �|���S���̕`��
	device->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// �e�N�X�`��0��1�����ւ���
	Texture m_texturesWk = m_MTInfo.textures[0];
	m_MTInfo.textures[0] = m_MTInfo.textures[1];
	m_MTInfo.textures[1] = m_texturesWk;

	Surface m_rendersWk = m_MTInfo.renders[0];
	m_MTInfo.renders[0] = m_MTInfo.renders[1];
	m_MTInfo.renders[1] = m_rendersWk;
}

//========================================
// ��������ݒ�
//========================================
void CCamera::SetPosVAndPosR(const Pos3D& posV, const Pos3D& posR) {

	m_posV = posV;
	m_posR = posR;
	m_dist = RNLib::Geometry().FindDistance(m_posV, m_posR);
	m_rot  = m_isPivotToPosV ? RNLib::Geometry().FindRot(m_posV, m_posR) : RNLib::Geometry().FindRot(m_posR, m_posV);
}

//========================================
// �U����ݒ�
//========================================
void CCamera::SetVib(const float& vibPower) {

	// ���݂̐U���ȉ��̎��A�I��
	if (m_vibForce >= vibPower)
		return;

	// �U���̋�����ݒ�
	m_vibForce = vibPower;

	// �R���g���[���[�ɐU����`����
	RNLib::Input().SetVibration(
		m_vibForce / CONTROLLER_VIB_MAX,
		m_vibForce * CONTROLLER_VIB_TIME_DIAMETER);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�J�����N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// [�ÓI]�v���W�F�N�V�����}�g���b�N�X�̃Z�b�g�A�b�v
//========================================
void CCamera::SetUpProjectionMatrix(Device& device, const Scale2D& scale, const float& radian) {

	Matrix mtxProjection = INITMATRIX;
	D3DXMatrixPerspectiveFovLH(&mtxProjection, D3DXToRadian(radian), scale.x / scale.y, 10.0f, 4000.0f);

	device->SetTransform(D3DTS_PROJECTION, &mtxProjection);
}