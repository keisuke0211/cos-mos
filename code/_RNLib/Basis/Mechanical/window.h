//========================================
// 
// �E�B���h�E�̏���
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// �N���X��`
//****************************************
// �E�B���h�E�N���X
class _RNC_Window {
public:
	//========== [[[ �֐��錾 ]]]
	_RNC_Window         ();
	~_RNC_Window        ();
	void Init           (void);
	void Uninit         (void);
	void Update         (void);
	int  Create         (HINSTANCE& hInstance, WNDPROC lpfnWndProc);
	void Delete         (void);
	void ShowTaskBar    (const bool& isShow);
	void SetIsFullScreen(const bool& isFullScreen);
	bool FindFocused    (const HWND& windowHandle);
	// ���b�Z�[�W����
	void Message         (const char* pText, const char* pCaption);
	void Message_ERROR   (const char* pText);
	bool Message_OKCANCEL(const char* pText, const char* pCaption);
	// �ݒ�擾
	Pos2D&  GetPos       (void) { return m_pos; }
	float&  GetWidth     (void) { return m_width; }
	float&  GetHeight    (void) { return m_height; }
	Size2D  GetSize      (void) { return Pos2D(m_width, m_height); }
	float   GetCenterX   (void) { return m_width * 0.5f; }
	float   GetCenterY   (void) { return m_height * 0.5f; }
	Pos2D   GetCenterPos (void) { return Pos2D(m_width * 0.5f, m_height * 0.5f); }
	float&  GetAreaWidth (void) { return m_areaWidth; }
	float&  GetAreaHeight(void) { return m_areaHeight; }
	float&  GetResolution(void) { return m_resolution; }
	Device& GetD3DDevice (void) { return m_D3D9Device; }
	HWND&   GetHandle    (void) { return m_hWnd; }

private:
	//========== [[[ �֐��錾 ]]]
	int CreateD3DDevice(HINSTANCE& hInstance);
	int GetTargetGPUAdapterIdx(void);

	//========== [[[ �ϐ��錾 ]]]
	Pos2D       m_pos;
	HINSTANCE   m_instanceHandle;
	const char* m_className;
	float       m_width;
	float       m_height;
	float       m_areaWidth;	// �̈�̕�
	float       m_areaHeight;	// �̈�̍���
	float       m_resolution;	// �𑜓x
	LPDIRECT3D9 m_D3D9;
	Device      m_D3D9Device;
	HWND        m_hWnd;
};
