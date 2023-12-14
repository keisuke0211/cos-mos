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
#include "Basis/Mechanical/print.h"
#include "Basis/Mechanical/regist.h"
#include "Basis/Mechanical/sound.h"
#include "Basis/Mechanical/window.h"
//========== [[[ Other... ]]]
#include "Basis/default-data.h"
#include "Basis/setting.h"

//****************************************
// 名前空間
//****************************************
// RNシステム
namespace RNSystem {

	//========== [[[ 列挙型定義 ]]]
	enum class SIGNAL { NONE, INIT, UNINIT, END_UNINIT, UNINIT_WAIT, UPDATE, UPDATE_WAIT, DRAW, END_DRAW, END, };
	enum class MODE   { EXECUTION, DEBUG, DEMO, SETUP3D_EDITOR };

	//========== [[[ 関数宣言 ]]]
	bool          MainLoop      (HINSTANCE& instanceHandle, const char* settingsPath, const UShort& priorityMax, const MODE& mode);
	SIGNAL        GetSignal     (void);
	MODE          GetMode       (void);
	void          EndScene      (void);
	int           GetFPS        (void);
	void          SetSpace3DStop(const bool& isStop);
	bool          GetSpace3DStop(void);
	bool          GetSceneSwap  (void);
	CEffect3DMgr& GetEffet3DMgr (void);
	CCameraMgr&   GetCameraMgr  (void);
}

// RNライブラリ
namespace RNLib {

	// 3DObject
	CDoll3DMgr&        Doll3DMgr       (void);
	CMotion3D&         Motion3D        (void);
	CSetUp3D&          SetUp3D         (void);
	CEffect3D&         Effect3D        (void);
	CStandardEffect3D& StandardEffect3D(void);
	// Calculation
	CEase&             Ease            (void);
	CGeometry&         Geometry        (void);
	CHitTest&          HitTest         (void);
	CMatrix&           Matrix          (void);
	// Draw						       
	CStaticMesh&       StaticMesh      (void);
	CModel&            Model           (void);
	CPolygon2D&        Polygon2D       (void);
	CPolygon3D&        Polygon3D       (void);
	CText2D&           Text2D          (void);
	CText3D&           Text3D          (void);
	CDrawMgr&          DrawMgr         (void);
	CDrawState&        DrawStateMgr    (void);
	CLight3D&          Light3D         (void);
	CText&             Text            (void);
	CTexture&          Texture         (void);
	// Mechanical
	CCount&            Count           (void);
	CFile&             File            (void);
	CInput&            Input           (void);
	CMemory&           Memory          (void);
	CSound&            Sound           (void);
	CWindow&           Window          (void);
	// Other...
	CDefaultData&      DefaultData     (void);
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