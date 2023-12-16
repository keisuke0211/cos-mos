//========================================
// 
// �E�B���h�E�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "../../RNlib.h"
#include "../../RNsettings.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [���J]�E�B���h�E�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �R���X�g���N�^
//========================================
_RNC_Window::_RNC_Window(void) {

	m_pos        = INITPOS2D;
	m_className  = NULL;
	m_width      = 0.0f;
	m_height     = 0.0f;
	m_areaWidth  = 0.0f;
	m_areaHeight = 0.0f;
	m_resolution = 0.0f;
	m_D3D9       = NULL;
	m_D3D9Device = NULL;
}

//========================================
// �f�X�g���N�^
//========================================
_RNC_Window::~_RNC_Window(void) {

}

//========================================
// ����������
//========================================
void _RNC_Window::Init(void) {

}

//========================================
// �I������
//========================================
void _RNC_Window::Uninit(void) {

	// Direct3D�f�o�C�X�̔j��
	if (m_D3D9Device != NULL) {
		m_D3D9Device->Release();
		m_D3D9Device = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_D3D9 != NULL) {
		m_D3D9->Release();
		m_D3D9 = NULL;
	}

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(m_className, m_instanceHandle);

	// �^�X�N�o�[��\��
	ShowTaskBar(true);
}

//========================================
// �X�V����
//========================================
void _RNC_Window::Update(void) {

	// �E�B���h�E�ʒu���擾
	RECT rec;
	GetWindowRect(m_hWnd, &rec);
	m_pos = Pos2D(rec.left, rec.top);
}

//========================================
// ��������
//========================================
int _RNC_Window::Create(HINSTANCE& instanceHandle, WNDPROC lpfnWndProc) {

	// �Z�b�e�B���O�����擾
	const RNSettings::Info& settingsInfo = RNSettings::GetInfo();

	// �E�C���h�E�N���X�̍\����
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						// �E�C���h�E�̃X�^�C��
		lpfnWndProc,					// �E�C���h�E�v���V�[�W��
		0,								// 0�ɂ���
		0,								// 0�ɂ���
		instanceHandle,					// �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),		// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),		// �N���C�A���g�̈�̔w�i�F
		NULL,							// ���j���[�o�[
		settingsInfo.projectName,		// �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �t�@�C���̃A�C�R��
	};

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�C���h�E�𐶐�
	m_hWnd = CreateWindowEx(
		0,								// �g���E�C���h�E�X�^�C��
		settingsInfo.projectName,		// �E�C���h�E�N���X�̖��O
		settingsInfo.projectName,		// �E�C���h�E�̖��O
		WS_SYSMENU | WS_MINIMIZEBOX,	// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,					// �E�C���h�E�̍���X���W
		CW_USEDEFAULT,					// �E�C���h�E�̍���Y���W
		settingsInfo.windowWidth,		// �E�C���h�E�̕�
		settingsInfo.windowHeight,		// �E�C���h�E�̍���
		NULL,							// �e�E�C���h�E�̃n���h��
		NULL,							// ���j���[�n���h���܂���
		instanceHandle,						// �C���X�^���X�n���h��
		NULL);							// �E�C���h�E�쐬�f�[�^

	// Direct3D�f�o�C�X�̐���
	if (FAILED(CreateD3DDevice(instanceHandle))) {
		return -1;
	}

	// �t���X�N���[�����[�h��ݒ�
	SetIsFullScreen(RNLib::Options().GetIsFullScreen());

	return 0;
}

//========================================
// �폜����
//========================================
void _RNC_Window::Delete(void) {

	// �E�B���h�E��j�󂷂�
	DestroyWindow(m_hWnd);
}

//========================================
// �^�X�N�o�[�̕\��/��\����؂�ւ�
//========================================
void _RNC_Window::ShowTaskBar(const bool& isShow) {

	// �^�X�N�o�[�̃n���h�����擾
	HWND taskBarHandle;
	taskBarHandle = FindWindow((LPCSTR)"Shell_TrayWnd", NULL);

	if (isShow) {
		ShowWindow(taskBarHandle, SW_SHOW);
		SetFocus(m_hWnd);	// ���̃E�B���h�E�Ƀt�H�[�J�X�𓖂Ă�
	}
	else {
		ShowWindow(taskBarHandle, SW_HIDE);
	}
}

//========================================
// �t���X�N���[���ݒ�
//========================================
void _RNC_Window::SetIsFullScreen(const bool& isFullScreen) {

	if (!RNSettings::GetInfo().isFullScreen && isFullScreen)
		return;

	// �f�X�N�g�b�v�̏����擾
	HDC desctopHandle = GetDC(GetDesktopWindow());
	int screenSizeX   = GetDeviceCaps(desctopHandle, HORZRES);
	int screenSizeY   = GetDeviceCaps(desctopHandle, VERTRES);

	// �f�X�N�g�b�v�̃n���h���������
	ReleaseDC(GetDesktopWindow(), desctopHandle);

	if (isFullScreen) {
		SetWindowPos(m_hWnd, HWND_TOP, 0, 0, screenSizeX, screenSizeY, SWP_SHOWWINDOW);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_POPUP);
	}
	else {
		SetWindowPos(m_hWnd, HWND_TOP, (screenSizeX * 0.5f) - (m_width * 0.5f), (screenSizeY * 0.5f) - (m_height * 0.5f), m_width, m_height, SWP_SHOWWINDOW);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	}

	// �E�C���h�E�̕\����Ԃ�ݒ�
	ShowWindow(m_hWnd, SW_NORMAL);

	// �N���C�A���g�̈���X�V
	UpdateWindow(m_hWnd);

	// �^�X�N�o�[��\��/��\��
	ShowTaskBar(!isFullScreen);
}

//========================================
// ���b�Z�[�W����
//========================================
void _RNC_Window::Message(const char *pText, const char *pCaption) {

	MessageBox(m_hWnd, pText, pCaption, MB_OK);
	RNLib::Input().ClearInputInfo();
}

//========================================
// ���b�Z�[�W����(�G���[)
//========================================
void _RNC_Window::Message_ERROR(const char* pText) {

	// ���s���[�h�̎��͏I��
	if (RNSystem::GetMode() == RNSystem::MODE::EXECUTION)
		return;

	MessageBox(m_hWnd, pText, "ERROR", MB_ICONWARNING);
	RNLib::Input().ClearInputInfo();
}

//========================================
// ���b�Z�[�W����(OK or CANCEL)
//========================================
bool _RNC_Window::Message_OKCANCEL(const char* pText, const char* pCaption) {

	switch (MessageBox(m_hWnd, pText, pCaption, MB_OKCANCEL)) {
	case IDOK    :return true;
	case IDCANCEL:return false;
	}

	return false;
}

//========================================
// �t�H�[�J�X���������Ă��邩���ׂ�
//========================================
bool _RNC_Window::FindFocused(const HWND& windowHandle) {

	// �f�o�C�X�������Ă���ꍇ�̓t�H�[�J�X���������Ă��Ȃ��Ƃ݂Ȃ�
	HRESULT hr = m_D3D9Device->TestCooperativeLevel();
	if (hr == D3DERR_DEVICELOST || hr == D3DERR_DEVICENOTRESET)
		return false;

	return (GetForegroundWindow() == windowHandle);
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| [����J]�E�B���h�E�N���X
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// Direct3D�f�o�C�X�̐���
//========================================
int _RNC_Window::CreateD3DDevice(HINSTANCE& instanceHandle) {

	// �Z�b�e�B���O�����擾
	const RNSettings::Info& settingsInfo = RNSettings::GetInfo();

	// �E�B���h�E�̏���ݒ�
	m_instanceHandle = instanceHandle;
	m_className      = settingsInfo.projectName;
	m_width          = settingsInfo.windowWidth;
	m_height         = settingsInfo.windowHeight;
	m_areaWidth      = m_width  * settingsInfo.resolution;
	m_areaHeight     = m_height * settingsInfo.resolution;
	m_resolution     = settingsInfo.resolution;

	// Direct3D�I�u�W�F�N�g�̐���
	m_D3D9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_D3D9 == NULL)
		return -1;

	// GPU�̃A�_�v�^�ԍ����擾
	const int GPUAdapterIdx = GetTargetGPUAdapterIdx();

	// ���݂̃f�B�X�v���C���[�h���擾
	D3DDISPLAYMODE d3ddm;
	if (FAILED(m_D3D9->GetAdapterDisplayMode(GPUAdapterIdx, &d3ddm)))
		return E_FAIL;

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^���[���N���A
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth            = m_areaWidth;
	d3dpp.BackBufferHeight           = m_areaHeight;
	d3dpp.BackBufferFormat           = d3ddm.Format;				// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount            = 1;							// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD;		// �_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil     = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed                   = !settingsInfo.isFullScreen ? RNLib::Options().GetIsFullScreen() : settingsInfo.isFullScreen;		// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// ���t���b�V�����[�g
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���
	if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
		D3DDEVTYPE_HAL,
		m_hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_D3D9Device))) {

		if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
			D3DDEVTYPE_HAL,
			m_hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_D3D9Device))) {

			if (FAILED(m_D3D9->CreateDevice(GPUAdapterIdx,
				D3DDEVTYPE_REF,
				m_hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_D3D9Device))) {
				return -1;
			}
		}
	}

	return 0;
}

//========================================
// �ړI��GPU�̃A�_�v�^�[�ԍ���Ԃ�
//========================================
int _RNC_Window::GetTargetGPUAdapterIdx(void) {

	// �A�_�v�^�J�E���g���擾
	const int adapterCount = m_D3D9->GetAdapterCount();

	for (int cnt = 0; cnt < adapterCount; cnt++) {

		D3DADAPTER_IDENTIFIER9 adapterIdentifier;
		if (SUCCEEDED(m_D3D9->GetAdapterIdentifier(cnt, 0, &adapterIdentifier))) {

			// �ړI��GPU�Ɩ��̂���v�������A�ԍ���Ԃ�
			// ������ADirectX9�ł�GeForce RTX 3050��F���ł��Ȃ��̂Ŏg�����ɂȂ�Ȃ�
			if (strstr(adapterIdentifier.Description, "GeForce RTX 3050") != NULL)
				return cnt;
		}
	}

	// �ړI��GPU��������Ȃ��ꍇ�́A�f�t�H���g�̃A�_�v�^�[��Ԃ�
	return D3DADAPTER_DEFAULT;
}