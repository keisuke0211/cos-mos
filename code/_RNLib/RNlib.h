//========================================
// 
// RNライブラリの処理
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

//****************************************
// ヘッダファイルのインクルード
//****************************************
#include "RNmain.h"
//========== [[[ 3DObject ]]]
#include "Basis/3DObject/Body/doll3D.h"
#include "Basis/3DObject/Body/doll3D-manager.h"
#include "Basis/3DObject/Body/motion3D.h"
#include "Basis/3DObject/Body/setup3D.h"
#include "Basis/3DObject/Effect/effect3D.h"
#include "Basis/3DObject/Effect/effect3D_cylinder.h"
#include "Basis/3DObject/Effect/effect3D_model.h"
#include "Basis/3DObject/Effect/effect3D-base.h"
#include "Basis/3DObject/Effect/effect3D-manager.h"
#include "Basis/3DObject/Effect/standard-effect3D.h"
//========== [[[ Calculation ]]]
#include "Basis/Calculation/ease.h"
#include "Basis/Calculation/geometry.h"
#include "Basis/Calculation/hit-test.h"
#include "Basis/Calculation/matrix.h"
#include "Basis/Calculation/number.h"
//========== [[[ Draw ]]]
#include "Basis/Draw/PutObject/static-mesh.h"
#include "Basis/Draw/PutObject/model.h"
#include "Basis/Draw/PutObject/polygon2D.h"
#include "Basis/Draw/PutObject/polygon3D.h"
#include "Basis/Draw/PutObject/text2D.h"
#include "Basis/Draw/PutObject/text3D.h"
#include "Basis/Draw/camera.h"
#include "Basis/Draw/camera-manager.h"
#include "Basis/Draw/color.h"
#include "Basis/Draw/draw-manager.h"
#include "Basis/Draw/draw-state.h"
#include "Basis/Draw/light3D.h"
#include "Basis/Draw/light3D-manager.h"
#include "Basis/Draw/regist-info.h"
#include "Basis/Draw/text.h"
#include "Basis/Draw/texture.h"
//========== [[[ Mechanical ]]]
#include "Basis/Mechanical/count.h"
#include "Basis/Mechanical/file.h"
#include "Basis/Mechanical/input.h"
#include "Basis/Mechanical/memory.h"
#include "Basis/Mechanical/object.h"
#include "Basis/Mechanical/object-manager.h"
#include "Basis/Mechanical/string.h"
#include "Basis/Mechanical/regist.h"
#include "Basis/Mechanical/sound.h"
#include "Basis/Mechanical/window.h"
//========== [[[ Other... ]]]
#include "Basis/default-data.h"
#include "Basis/options.h"

//****************************************
// 名前空間
//****************************************
// RNシステム
namespace RNSystem {

	//========== [[[ 列挙型定義 ]]]
	enum class SIGNAL { NONE, INIT, UNINIT, END_UNINIT, UNINIT_WAIT, UPDATE, UPDATE_WAIT, DRAW, END_DRAW, END, };
	enum class MODE   { EXECUTION, DEBUG, DEMO, SETUP3D_EDITOR, LIGHT3D_EDITOR };

	//========== [[[ 関数宣言 ]]]
	bool              MainLoop      (HINSTANCE& instanceHandle, const char* settingsPath, const char* optionsDataPath, const UShort& priorityMax, const MODE& mode);
	SIGNAL            GetSignal     (void);
	MODE              GetMode       (void);
	void              EndScene      (void);
	int               GetFPS        (void);
	void              SetSpace3DStop(const bool& isStop);
	bool              GetSpace3DStop(void);
	bool              GetSceneSwap  (void);
	_RNC_Effect3DMgr& GetEffet3DMgr (void);
	_RNC_CameraMgr&   GetCameraMgr  (void);
	_RNC_Light3DMgr&  GetLight3DMgr (void);
}

// RNライブラリ
namespace RNLib {

	// 3DObject
	_RNC_Doll3DMgr&        Doll3DMgr       (void);
	_RNC_Motion3D&         Motion3D        (void);
	_RNC_SetUp3D&          SetUp3D         (void);
	_RNC_Effect3D&         Effect3D        (void);
	_RNC_StandardEffect3D& StandardEffect3D(void);
	// Calculation
	_RNC_Ease&             Ease            (void);
	_RNC_Geometry&         Geometry        (void);
	_RNC_HitTest&          HitTest         (void);
	_RNC_Matrix&           Matrix          (void);
	_RNC_Number&           Number          (void);
	// Draw						       
	_RNC_StaticMesh&       StaticMesh      (void);
	_RNC_Model&            Model           (void);
	_RNC_Polygon2D&        Polygon2D       (void);
	_RNC_Polygon3D&        Polygon3D       (void);
	_RNC_Text2D&           Text2D          (void);
	_RNC_Text3D&           Text3D          (void);
	_RNC_DrawMgr&          DrawMgr         (void);
	_RNC_DrawState&        DrawStateMgr    (void);
	_RNC_Text&             Text            (void);
	_RNC_Texture&          Texture         (void);
	// Mechanical
	_RNC_Count&            Count           (void);
	_RNC_File&             File            (void);
	_RNC_Input&            Input           (void);
	_RNC_Memory            Memory          (void);
	_RNC_Sound&            Sound           (void);
	_RNC_Window&           Window          (void);
	// Other...
	_RNC_DefaultData&      DefaultData     (void);
	_RNC_Options&          Options         (void);
}

//****************************************
// メモリリークチェックの定義
//****************************************
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#ifdef _DEBUG
#define CHECK_MEM_LEAK
#endif // _DEBUG

#ifdef CHECK_MEM_LEAK
#ifdef new
#undef new
#endif
#define new new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#endif // CHECK_MEM_LEAK