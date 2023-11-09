//========================================
// 
// RN���C�u�����̏���
// Author:RIKU NISHIMURA
// 
//========================================
#include "RNlib.h"
#include "RNobject.h"
#include "RNsettings.h"
#include "Demo/demo.h"
#include "../Project/../Project/System/words/object/font-object.h"

//****************************************
// �v���g�^�C�v�錾
//****************************************
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//****************************************
// �������
//****************************************
namespace {

	//========== [[[ �֐��錾 ]]]
	void Init(HINSTANCE& instanceHandle, const char* settingsPath, const bool& demo);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//========== [[[ �ϐ��錾 ]]]
	RNSystem::SIGNAL signal = RNSystem::SIGNAL::NONE;
	DWORD            currentTime;
	DWORD            execLastTime;
	DWORD            frameCount;
	DWORD            FPSLastTime;
	int              FPSCount;
	bool             isSpace3DStop;
	bool             isSpace3DStopReserve;
	bool             isSceneSwap;

	// RN�I�u�W�F�N�g
	C3DObject    _3DObject;
	CCalculation calculation;
	CDraw        draw;
	CMechanical  mechanical;
	COther       other;
	CDemo*       demo;
}

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RN���C�u����
//----------|---------------------------------------------------------------------
//================================================================================
CMotion3D&    RNLib::Motion3D      (void) { return _3DObject.m_motion3D;   }
CSetUp3D&     RNLib::SetUp3D       (void) { return _3DObject.m_setUp3D;    }
CEase&        RNLib::Ease          (void) { return calculation.m_ease;     }
CGeometry&    RNLib::Geometry      (void) { return calculation.m_geometry; }
CMatrix&      RNLib::Matrix        (void) { return calculation.m_matrix;   }
CModel&       RNLib::Model         (void) { return draw.m_model;           }
CPolygon2D&   RNLib::Polygon2D     (void) { return draw.m_polygon2D;       }
CPolygon3D&   RNLib::Polygon3D     (void) { return draw.m_polygon3D;       }
CText2D&      RNLib::Text2D        (void) { return draw.m_text2D;          }
CText3D&      RNLib::Text3D        (void) { return draw.m_text3D;          }
CCameraMgr&   RNLib::CameraMgr     (void) { return draw.m_cameraMgr;       }
CDrawMgr&     RNLib::DrawMgr       (void) { return draw.m_drawMgr;         }
CDrawState&   RNLib::DrawStateMgr  (void) { return draw.m_drawState;       }
CLight3D&     RNLib::Light3D       (void) { return draw.m_light3D;         }
CText&        RNLib::Text          (void) { return draw.m_text;            }
CTexture&     RNLib::Texture       (void) { return draw.m_texture;         }
CTransition&  RNLib::Transition    (void) { return draw.m_transition;      }
CCount&       RNLib::Count         (void) { return mechanical.m_count;     }
CFile&        RNLib::File          (void) { return mechanical.m_file;      }
CInput&       RNLib::Input         (void) { return mechanical.m_input;     }
CMemory&      RNLib::Memory        (void) { return mechanical.m_memory;    }
CSound&       RNLib::Sound         (void) { return mechanical.m_sound;     }
CWindow&      RNLib::Window        (void) { return mechanical.m_window;    }
CDefaultData& RNLib::DefaultData   (void) { return other.m_defaultData;    }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| RN�V�X�e��
//----------|---------------------------------------------------------------------
//================================================================================

//========================================
// �ݒ�/�擾�n�֐�
//========================================
RNSystem::SIGNAL RNSystem::GetSignal     (void)               { return demo == NULL ? signal : SIGNAL::NONE; }
int              RNSystem::GetFPSCount   (void)               { return FPSCount; }
void             RNSystem::SetSpace3DStop(const bool& isStop) { isSpace3DStopReserve = isStop; }
bool             RNSystem::GetSpace3DStop(void)               { return isSpace3DStop; }
bool             RNSystem::GetSceneSwap  (void)               { return isSceneSwap; }

//========================================
// ���C�����[�v
//========================================
bool RNSystem::MainLoop(HINSTANCE& instanceHandle, const char* settingsPath, const bool& isDemo) {

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
			currentTime = timeGetTime();

			if ((currentTime - FPSLastTime) >= 500)
			{// 0.5�b�o��
				FPSCount    = (frameCount * 1000) / (currentTime - FPSLastTime);
				FPSLastTime = currentTime;	// FPS���莞����ۑ�
				frameCount  = 0;
			}

			// 60/1�b�o�߂��Ă��Ȃ����͔�����
			if ((currentTime - execLastTime) <= (1000 / 60))
				return true;

			// ���ݎ�����ۑ�
			execLastTime = currentTime;
			frameCount++;
		}
	}

	// [[[ ���݂̐M���ɉ����āA�V���ɐM����ݒ� ]]]
	switch (signal) {
	case RNSystem::SIGNAL::NONE       :signal = RNSystem::SIGNAL::INIT       ; break;
	case RNSystem::SIGNAL::INIT       :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	case RNSystem::SIGNAL::UNINIT     :signal = RNSystem::SIGNAL::END        ; break;
	case RNSystem::SIGNAL::UNINIT_WAIT:signal = RNSystem::SIGNAL::UNINIT     ; break;
	case RNSystem::SIGNAL::UPDATE     :signal = RNSystem::SIGNAL::DRAW       ; break;
	case RNSystem::SIGNAL::UPDATE_WAIT:signal = RNSystem::SIGNAL::UPDATE     ; break;
	case RNSystem::SIGNAL::DRAW       :signal = RNSystem::SIGNAL::UPDATE_WAIT; break;
	}

	// [[[ �M���ɉ��������� ]]]
	switch (signal) {
	case RNSystem::SIGNAL::INIT: {
		Init(instanceHandle, settingsPath, isDemo);
	}break;
	case RNSystem::SIGNAL::UNINIT: {
		Uninit();
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
CDemo& RNDemo::Get(void) { return *demo; }

//================================================================================
//----------|---------------------------------------------------------------------
//==========| ������Ԃ̊֐�
//----------|---------------------------------------------------------------------
//================================================================================
namespace {
	//========================================
	// ��������
	//========================================
	void Init(HINSTANCE& instanceHandle, const char* settingsPath, const bool& isDemo) {

		// �I����Ƀ��������[�N���o��
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
		// �����̎���擾
		srand((unsigned int)time(0));

		// ����\��ݒ�
		timeBeginPeriod(1);
		
		// �ϐ���������
		currentTime          = 0;
		execLastTime         = timeGetTime();
		frameCount           = 0;
		FPSLastTime          = timeGetTime();
		isSpace3DStop        = false;
		isSpace3DStopReserve = false;
		isSceneSwap          = false;

		// �ݒ�t�@�C����ǂݍ���&�����o��
		if (RNSettings::LoadAndSave(settingsPath))
		{// �����������A
			// �ݒ�������ɃE�B���h�E�𐶐�
			RNSettings::Info settingsInfo = RNSettings::GetInfo();
			mechanical.m_window.Create(
				instanceHandle,
				{
					WindowProc,						// �E�B���h�E�v���V�[�W��
					settingsInfo.projectName,		// �E�B���h�E�N���X�̖��O
					settingsInfo.projectName,		// �E�B���h�E�̖��O
					settingsInfo.windowWidth,		// �E�B���h�E�̕�
					settingsInfo.windowHeight,		// �E�B���h�E�̍���
					settingsInfo.isFullScreen,		// �E�B���h�E���[�h
					settingsInfo.resolution,		// �𑜓x�{��
				}
			);
		}
		else
		{// ���s�������A�V�O�i�����I���ɂ���
			signal = RNSystem::SIGNAL::UNINIT;
		}

		// RN�I�u�W�F�N�g�̏�����
		_3DObject  .Init();
		calculation.Init();
		draw	   .Init(mechanical.m_window.GetD3DDevice());
		mechanical .Init(instanceHandle);
		other	   .Init();
		if (isDemo) {
			demo = NULL;
			CMemory::Alloc(&demo);
			demo->Init();
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
		if (demo != NULL) {
			demo->Uninit();
			CMemory::Release(&demo);
		}

		// �v�����g�̏I������
		UninitPrint();
		
		// �ݒ���̉������
		RNSettings::Release();

		// ����\��߂�
		timeEndPeriod(1);
	}
	
	//========================================
	// �X�V����
	//========================================
	void Update(void) {

		// 3D��Ԓ�~�\���K�p����
		isSpace3DStop = isSpace3DStopReserve;

		// �S�I�u�W�F�N�g�}�l�[�W���[�̍X�V����
		CObjectMgr::UpdateAllMgrs();

		// RN�I�u�W�F�N�g
		_3DObject  .Update();
		calculation.Update();
		other	   .Update();
		draw	   .Update();
		mechanical .Update();
		if (demo != NULL)
			demo ->Update();
		
		// �ݒ�̍X�V����
		UpdateSetting();
	}

	//========================================
	// �`�揈��
	//========================================
	void Draw(void) {

		// �f�o�C�X���擾
		Device device = mechanical.m_window.GetD3DDevice();

		if (SUCCEEDED(device->BeginScene())) 
		{// �f�o�C�X�̕`��J�n����
			if (draw.m_drawMgr.StartDraw()) 
			{// �`��J�n����
				{// [[[ �X�N���[���`�� ]]]
					CCamera::StartRenderingScreen(device);
					draw.m_drawMgr.Draw(device, NONEDATA, false, true);
				}

				{// [[[ �J�����`�� ]]]
					// �����_�����O�^�[�Q�b�g/Z�o�b�t�@/�r���[�|�[�g��ۑ�
					Surface  renderDef;
					Surface  ZBuffDef;
					Viewport viewPortDef;
					device->GetRenderTarget       (0, &renderDef);
					device->GetDepthStencilSurface(&ZBuffDef);
					device->GetViewport           (&viewPortDef);

					// �J����1��1�ɑ΂��`�悵�Ă���
					CCamera* camera = NULL;
					while (draw.m_cameraMgr.ListLoop((CObject**)&camera)) {
						camera->StartRendering(device);
						draw.m_drawMgr.Draw(device, camera->GetID(), camera->GetClipping(), false);
						CFontObject::DrawAll();
						camera->EndRendering(device);
					}

					// �����_�����O�^�[�Q�b�g/Z�o�b�t�@/�r���[�|�[�g�����ɖ߂�
					device->SetRenderTarget       (0, renderDef);
					device->SetDepthStencilSurface(ZBuffDef);
					device->SetViewport           (&viewPortDef);
				}

				// �`��I��
				device->EndScene();

				// �o�b�N�o�b�t�@���t�����g�o�b�t�@�Ɠ���ւ�
				device->Present(NULL, NULL, NULL, NULL);
			}
			else 
			{// �`��J�n���s
				// �`��I��
				device->EndScene();
			}
		}
	
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
		mechanical.m_input.SetWheelSpin(HIWORD(wParam) == WHEEL_DELTA ? CInput::WHEELSPIN::FRONT : CInput::WHEELSPIN::BACK);

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