//========================================
// 
// RN���C�u�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNmode.h"
#include "RNobject.h"
#include "Demo/demo.h"
#include "Editor/setup3D-editor.h"
#include "Editor/light3D-editor.h"
#include "Editor/rail3D-editor.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//****************************************
// �������
//****************************************
namespace {

	//========== [[[ �֐��錾 ]]]
	void Init(HINSTANCE& instanceHandle, const char*& settingsPath, const char*& optionsDataPath, const UShort& priorityMax, const RNSystem::MODE& mode);
	void Uninit(void);
	void EndUninit(void);
	void Update(void);
	void Draw(void);
	void EndDraw(void);

	//========== [[[ �ϐ��錾 ]]]
	RNSystem::SIGNAL signal = RNSystem::SIGNAL::NONE;
	RNSystem::MODE   nowMode;
	DWORD            execLastTime;
	DWORD            lastTime;
	DWORD            FPSCount;
	int              FPS;
	bool             isSpace3DStop;
	bool             isSpace3DStopReserve;
	bool             isSceneSwap;
	bool             isBeginScene;

	// RN�I�u�W�F�N�g
	C3DObject    _3DObject;
	CCalculation calculation;
	CDraw        draw;
	CMechanical  mechanical;
	COther       other;
	void*        modeObject;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RN���C�u����
//----------|---------------------------------------------------------------------
//================================================================================
_RNC_Doll3DMgr&        RNLib::Doll3DMgr       (void) { return _3DObject.m_doll3DMgr;        }
_RNC_Motion3D&         RNLib::Motion3D        (void) { return _3DObject.m_motion3D;         }
_RNC_SetUp3D&          RNLib::SetUp3D         (void) { return _3DObject.m_setUp3D;          }
_RNC_Effect3D&         RNLib::Effect3D        (void) { return _3DObject.m_effect3D;         }
_RNC_StandardEffect3D& RNLib::StandardEffect3D(void) { return _3DObject.m_standardEffect3D; }
_RNC_Ease&             RNLib::Ease            (void) { return calculation.m_ease;           }
_RNC_Geometry&         RNLib::Geometry        (void) { return calculation.m_geometry;       }
_RNC_HitTest&          RNLib::HitTest         (void) { return calculation.m_hitTest;        }
_RNC_Matrix&           RNLib::Matrix          (void) { return calculation.m_matrix;         }
_RNC_Number&           RNLib::Number          (void) { return calculation.m_number;         }
_RNC_StaticMesh&       RNLib::StaticMesh      (void) { return draw.m_staticMesh;            }
_RNC_Model&            RNLib::Model           (void) { return draw.m_model;                 }
_RNC_Polygon2D&        RNLib::Polygon2D       (void) { return draw.m_polygon2D;             }
_RNC_Polygon3D&        RNLib::Polygon3D       (void) { return draw.m_polygon3D;             }
_RNC_Text2D&           RNLib::Text2D          (void) { return draw.m_text2D;                }
_RNC_Text3D&           RNLib::Text3D          (void) { return draw.m_text3D;                }
_RNC_DrawMgr&          RNLib::DrawMgr         (void) { return draw.m_drawMgr;               }
_RNC_DrawState&        RNLib::DrawStateMgr    (void) { return draw.m_drawState;             }
_RNC_Text&             RNLib::Text            (void) { return draw.m_text;                  }
_RNC_Texture&          RNLib::Texture         (void) { return draw.m_texture;               }
_RNC_Count&            RNLib::Count           (void) { return mechanical.m_count;           }
_RNC_File&             RNLib::File            (void) { return mechanical.m_file;            }
_RNC_Input&            RNLib::Input           (void) { return mechanical.m_input;           }
_RNC_Memory            RNLib::Memory          (void) { return _RNC_Memory();                }
_RNC_Sound&            RNLib::Sound           (void) { return mechanical.m_sound;           }
_RNC_Window&           RNLib::Window          (void) { return mechanical.m_window;          }
_RNC_DefaultData&      RNLib::DefaultData     (void) { return other.m_defaultData;          }
_RNC_Options&          RNLib::Options         (void) { return other.m_options;              }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RN�V�X�e��
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ݒ�/�擾�n�֐�
//========================================
RNSystem::SIGNAL  RNSystem::GetSignal      (void)               { return (nowMode == RNSystem::MODE::EXECUTION || nowMode == RNSystem::MODE::DEBUG) ? signal : SIGNAL::NONE; }
RNSystem::MODE    RNSystem::GetMode        (void)               { return nowMode; }
int               RNSystem::GetFPS         (void)               { return FPS; }
void              RNSystem::SetSpace3DStop (const bool& isStop) { isSpace3DStopReserve = isStop; }
bool              RNSystem::GetSpace3DStop (void)               { return isSpace3DStop; }
bool              RNSystem::GetSceneSwap   (void)               { return isSceneSwap; }
_RNC_Effect3DMgr& RNSystem::GetEffet3DMgr  (void)               { return _3DObject.m_effect3DMgr; }
_RNC_CameraMgr&   RNSystem::GetCameraMgr   (void)               { return draw.m_cameraMgr; }
_RNC_Light3DMgr&  RNSystem::GetLight3DMgr  (void)               { return draw.m_light3DMgr; }

//========================================
// ���C�����[�v
//========================================
bool RNSystem::MainLoop(HINSTANCE& instanceHandle, const char* settingsPath, const char* optionsDataPath, const UShort& priorityMax, const MODE& mode) {

	static bool isMessageLoop = false;
	static MSG  msg;

	// [[[ ���b�Z�[�W���[�v ]]]
	if (isMessageLoop) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0) 
		{// Windows�̏���
			if (msg.message == WM_QUIT) {
				signal = SIGNAL::UNINIT_WAIT;
				isMessageLoop = false;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			return true;
		}
		else {

			// ���ݎ������擾
			DWORD currentTime = timeGetTime();

			if ((currentTime - lastTime) >= 500)
			{// 0.5�b�o��
				FPS      = (FPSCount     * 1000) / (currentTime - lastTime);
				FPSCount = 0;
				lastTime = currentTime;	// FPS���莞����ۑ�
			}

			// 60/1�b�o�߂��Ă��Ȃ����͔�����
			if ((currentTime - execLastTime) <= (1000 / 60))
				return true;

			// ���ݎ�����ۑ�
			execLastTime = currentTime;
			FPSCount++;
		}
	}

	// [[[ ���݂̐M���ɉ����āA�V���ɐM����ݒ� ]]]
	switch (signal) {
	case RNSystem::SIGNAL::NONE       :signal = RNSystem::SIGNAL::INIT       ; break;
	case RNSystem::SIGNAL::INIT       :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	case RNSystem::SIGNAL::UNINIT     :signal = RNSystem::SIGNAL::END_UNINIT ; break;
	case RNSystem::SIGNAL::END_UNINIT :signal = RNSystem::SIGNAL::END        ; break;
	case RNSystem::SIGNAL::UNINIT_WAIT:signal = RNSystem::SIGNAL::UNINIT     ; break;
	case RNSystem::SIGNAL::UPDATE     :signal = RNSystem::SIGNAL::DRAW       ; break;
	case RNSystem::SIGNAL::DRAW       :signal = RNSystem::SIGNAL::END_DRAW   ; break;
	case RNSystem::SIGNAL::END_DRAW   :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	case RNSystem::SIGNAL::UPDATE_WAIT:signal = RNSystem::SIGNAL::UPDATE     ; break;
	}

	// [[[ �M���ɉ��������� ]]]
	switch (signal) {
	case RNSystem::SIGNAL::INIT: {
		Init(instanceHandle, settingsPath, optionsDataPath, (mode == MODE::EXECUTION || mode == MODE::DEBUG) ? priorityMax : (UShort)RNMode::PRIORITY::MAX, mode);
	}break;
	case RNSystem::SIGNAL::UNINIT: {
		Uninit();
	}break;
	case RNSystem::SIGNAL::END_UNINIT: {
		EndUninit();
	}break;
	case RNSystem::SIGNAL::UPDATE: {
		Update();
		isMessageLoop = false;
	}break;
	case RNSystem::SIGNAL::UPDATE_WAIT: {
		isMessageLoop = true;
	}break;
	case RNSystem::SIGNAL::DRAW: {
		Draw();
	}break;
	case RNSystem::SIGNAL::END_DRAW: {
		EndDraw();
	}break;
	case RNSystem::SIGNAL::END: {
		return false;
	}break;
	}

	return true;
}

//========================================
// �V�[���I������
//========================================
void RNSystem::EndScene(void) {

	isSceneSwap          = true;
	isSpace3DStopReserve = false;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RN�f���̊֐�
//----------|---------------------------------------------------------------------
//================================================================================
CDemo& RNDemo::Get(void) { return nowMode == RNSystem::MODE::DEMO ? *(CDemo*)modeObject : *(CDemo*)nullptr; }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ������Ԃ̊֐�
//----------|---------------------------------------------------------------------
//================================================================================
namespace {
	//========================================
	// ��������
	//========================================
	void Init(HINSTANCE& instanceHandle, const char*& settingsPath, const char*& optionsDataPath, const UShort& priorityMax, const RNSystem::MODE& mode) {

		// ���[�h��ۑ�
		nowMode = mode;

		// �I����Ƀ��������[�N���o��
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		// �����̎���擾
		srand((unsigned int)time(0));

		// ����\��ݒ�
		timeBeginPeriod(1);
		
		// �ϐ���������
		execLastTime         = timeGetTime();
		FPSCount             = 0;
		lastTime             = timeGetTime();
		isSpace3DStop        = false;
		isSpace3DStopReserve = false;
		isSceneSwap          = false;
		isBeginScene         = false;

		// �I�v�V�����̃p�X��ݒ肵�A�ǂݍ��ݏ���
		other.m_options.SetOptionsDataPath(optionsDataPath);
		other.m_options.Load();
		
		// �ݒ�t�@�C����ǂݍ���&�����o��
		if (RNSettings::LoadAndSave(settingsPath))
		{// �����������A�ݒ�������ɃE�B���h�E�𐶐�
			if (FAILED(mechanical.m_window.Create(instanceHandle, WindowProc))) {
				signal = RNSystem::SIGNAL::UNINIT;
				return;
			}
		}
		else
		{// ���s�������A�V�O�i�����I���ɂ���
			signal = RNSystem::SIGNAL::UNINIT;
			return;
		}

		// RN�I�u�W�F�N�g�̏�����
		_3DObject  .Init();
		calculation.Init();
		draw	   .Init(mechanical.m_window.GetD3DDevice(), priorityMax);
		mechanical .Init(instanceHandle);
		other	   .Init();

		modeObject = NULL;
		switch (mode) {
		case RNSystem::MODE::DEMO: {
			RNLib::Memory().Alloc((CDemo**)&modeObject);
			CDemo* demo = (CDemo*)modeObject;
			demo->Init();
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			RNLib::Memory().Alloc((CSetUp3DEditor**)&modeObject);
			((CSetUp3DEditor*)modeObject)->Init();
		}break;
		case RNSystem::MODE::LIGHT3D_EDITOR: {
			RNLib::Memory().Alloc((CLight3DEditor**)&modeObject);
			((CLight3DEditor*)modeObject)->Init();
		}break;
		case RNSystem::MODE::RAIL3D_EDITOR: {
			RNLib::Memory().Alloc((CRail3DEditor**)&modeObject);
			((CRail3DEditor*)modeObject)->Init();
		}break;
		}
	}
	
	//========================================
	// �I������
	//========================================
	void Uninit(void) {

		// RN�I�u�W�F�N�g�̉��
		_3DObject  .Uninit();
		calculation.Uninit();
		draw	   .Uninit();
		mechanical .Uninit();
		other	   .Uninit();

		switch (nowMode) {
		case RNSystem::MODE::DEMO: {
			((CDemo*)modeObject)->Uninit();
			RNLib::Memory().Release((CDemo**)&modeObject);
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			((CSetUp3DEditor*)modeObject)->Uninit();
			RNLib::Memory().Release((CSetUp3DEditor**)&modeObject);
		}break;
		case RNSystem::MODE::LIGHT3D_EDITOR: {
			((CLight3DEditor*)modeObject)->Uninit();
			RNLib::Memory().Release((CLight3DEditor**)&modeObject);
		}break;
		case RNSystem::MODE::RAIL3D_EDITOR: {
			((CRail3DEditor*)modeObject)->Uninit();
			RNLib::Memory().Release((CRail3DEditor**)&modeObject);
		}break;
		}
		
		// �ݒ���̉������
		RNSettings::Release();

		// ����\��߂�
		timeEndPeriod(1);
	}
	
	//========================================
	// �ŏI�I������
	//========================================
	void EndUninit(void) {

		// �S�I�u�W�F�N�g�}�l�[�W���[�̉������
		CObjectMgr::ReleaseAllMgrs();
	}

	//========================================
	// �X�V����
	//========================================
	void Update(void) {

		// 3D��Ԓ�~�\���K�p����
		isSpace3DStop = isSpace3DStopReserve;

		switch (nowMode) {
		case RNSystem::MODE::DEMO: {
			((CDemo*)modeObject)->Update();
		}break;
		case RNSystem::MODE::SETUP3D_EDITOR: {
			((CSetUp3DEditor*)modeObject)->Update();
		}break;
		case RNSystem::MODE::LIGHT3D_EDITOR: {
			((CLight3DEditor*)modeObject)->Update();
		}break;
		case RNSystem::MODE::RAIL3D_EDITOR: {
			((CRail3DEditor*)modeObject)->Update();
		}break;
		}

		// �S�I�u�W�F�N�g�}�l�[�W���[�̍X�V����
		CObjectMgr::UpdateAllMgrs();

		// RN�I�u�W�F�N�g
		_3DObject  .Update();
		calculation.Update();
		draw       .Update();
		other	   .Update();
		mechanical .Update();
	}

	//========================================
	// �`�揈��
	//========================================
	void Draw(void) {

		// �`��J�n����
		draw.m_drawMgr.StartDraw(mechanical.m_window.GetD3DDevice());
	}

	//========================================
	// �ŏI�`�揈��
	//========================================
	void EndDraw(void) {

		// �`��I������
		draw.m_drawMgr.EndDraw(mechanical.m_window.GetD3DDevice());

		// �V�[������ւ��t���O�����Z�b�g
		isSceneSwap = false;
	}
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| �E�B���h�E�v���V�[�W��
//----------|---------------------------------------------------------------------
//================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	switch (uMsg) {
		// [[[ �j�� ]]]
	case WM_DESTROY: {
		
		PostQuitMessage(0);

	}break;
		// [[[ �L�[���� ]]]
	case WM_KEYDOWN: {
		
		// �G�X�P�[�v�L�[���������A�I���m�F
		if (wParam == VK_ESCAPE) {
			if (MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO) == IDYES)
				DestroyWindow(hWnd);
		}

	}break;
		// [[[ ���� ]]]
	case WM_CLOSE: {

		// �I���m�F
		if (MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO) == IDYES) {
			DestroyWindow(hWnd);
		}
		else {
			// (��0��Ԃ��Ȃ��ƏI�����Ă��܂�)
			return 0;
		}

	}break;
		// [[[ �}�E�X�z�C�[���̉�] ]]]
	case WM_MOUSEWHEEL: {

		// �}�E�X�z�C�[���̑O��]/���] ��Ԑݒ�
		mechanical.m_input.SetWheelSpin(HIWORD(wParam) == WHEEL_DELTA ? _RNC_Input::WHEELSPIN::FRONT : _RNC_Input::WHEELSPIN::BACK);

	}break;
		// [[[ �}�E�X���{�^������ ]]]
	case WM_LBUTTONDOWN: {

		// ���N���b�N�����E�C���h�E�Ƀt�H�[�J�X�����킹�A�A�N�e�B�u��
		SetFocus(hWnd);

	}break;
	}

	// ����̏�����Ԃ�
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}