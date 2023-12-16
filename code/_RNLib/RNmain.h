//========================================
// 
// RNライブラリのメインヘッダファイル
// Author:RIKU NISHIMURA
// 
//========================================
#pragma once

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <thread>
#include <mutex>
#include <time.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION	(0x0800)	// ビルド時の警告対処用マクロ
#include "dinput.h"						// 入力処理に必要
#include "Xinput.h"						// 入力処理に必要
#include "xaudio2.h"					// サウンド処理に必要

//****************************************
// ライブラリのリンク
//****************************************
// 描画処理に必要
#pragma comment(lib,"d3d9.lib")
// [d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"d3dx9.lib")
// DirectXコンポーネントを使用するのに必要
#pragma comment(lib,"dxguid.lib")
// DirectSoundを使用するのに必要
#pragma comment(lib,"dsound.lib")
// システム時刻取得に必要
#pragma comment(lib,"winmm.lib")
// 入力処理に必要
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")
// 警告無視
#pragma warning(disable:4091)
#pragma warning(disable:4100)
#pragma warning(disable:4244)

//****************************************
// マクロ定義
//****************************************
// 頂点フォーマット
#define FVF_VERTEX_2D         (D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define FVF_VERTEX_3D         (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)
// 初期値 (※D3D系列)
#define INITMatrix            Matrix       (1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)
#define INITViewport          Viewport     {0,0,0,0,0.0f,0.0f}
#define INITD3DCOLOR          D3DCOLOR_RGBA(255,255,255,255)
#define INITD3DXVECTOR3       D3DXVECTOR3  (0.0f,0.0f,0.0f)
#define INITD3DXVECTOR2       D3DXVECTOR2  (0.0f,0.0f)
// 初期値
#define INITMATRIX            Matrix       (1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,1.0f)
#define INITPOS2D             Pos2D        (0.0f,0.0f)
#define INITROT2D             Rot2D        (0.0f,0.0f)
#define INITVECTOR2D          Vector2D     (0.0f,0.0f)
#define INITNORMAL2D          Normal2D     (0.0f,0.0f)
#define INITSCALE2D           Scale2D      (1.0f,1.0f)
#define INITPOS3D             Pos3D        (0.0f,0.0f,0.0f)
#define INITROT3D             Rot3D        (0.0f,0.0f,0.0f)
#define INITVECTOR3D          Vector3D     (0.0f,0.0f,0.0f)
#define INITNORMAL3D          Normal3D     (0.0f,0.0f,0.0f)
#define INITSCALE3D           Scale3D      (1.0f,1.0f,1.0f)
#define INITBGCOLOR           Color        {0,0,0,255}
#define INITCOLOR             Color        {255,255,255,255}
// 円周率
#define D3DX_PI_HALF          (D3DX_PI*0.5f)
#define D3DX_PI_DOUBLE        (D3DX_PI*2.0f)
#define D3DX_PI_UP            (0.0f)
#define D3DX_PI_DOWN          (D3DX_PI)
#define D3DX_PI_RIGHT         (D3DX_PI*0.5f)
#define D3DX_PI_RIGHT_UP      (D3DX_PI*0.75f)
#define D3DX_PI_RIGHT_DOWN    (D3DX_PI*0.25f)
#define D3DX_PI_LEFT          (D3DX_PI*-0.5f)
#define D3DX_PI_LEFT_UP       (D3DX_PI*-0.25f)
#define D3DX_PI_LEFT_DOWN     (D3DX_PI*-0.75f)
// データ無し
#define NONEDATA              ((short)-1)
#define EDITDATA              ((short)-2)
#define NONECOLOR             Color{-1,-1,-1,-1}
// 関数形式
#define ARRAY_SIZE(a)         (sizeof(a) / sizeof(*a))
#define CHECK_ARRAY_SIZE(a,l) static_assert(ARRAY_SIZE(a) == (l),"aho")
// その他
#define TXT_MAX               (512)

//****************************************
// 列挙型定義
//****************************************
enum class PROCESS { INIT, UNINIT, UPDATE, };

//****************************************
// 構造体定義
//****************************************
// 頂点情報2Dの構造体
struct Vertex2D {
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 頂点座標
	float       rhw = 0.0f;				// 座標変換用係数(1.0fで固定)
	D3DCOLOR    col = INITD3DCOLOR;		// 頂点カラー
	D3DXVECTOR2 tex = INITD3DXVECTOR2;	// テクスチャ座標
};

// 頂点情報3Dの構造体
struct Vertex3D {
	D3DXVECTOR3 pos = INITD3DXVECTOR3;	// 頂点座標
	D3DXVECTOR3 nor = INITD3DXVECTOR3;	// 法線ベクトル
	D3DCOLOR    col = INITD3DCOLOR;		// 頂点カラー
	D3DXVECTOR2 tex = INITD3DXVECTOR2;	// テクスチャ座標
};

//****************************************
// 構造体変換
//****************************************
// Num
typedef unsigned char           UChar;
typedef unsigned short          UShort;
typedef unsigned int            UInt;
typedef unsigned long           ULong;
typedef const char              CChar;
typedef const short             CShort;
typedef const int               CInt;
typedef const long              CLong;
typedef const float             CFloat;
typedef const unsigned char     CUChar;
typedef const unsigned short    CUShort;
typedef const unsigned int      CUInt;
typedef const unsigned long     CULong;
// Vector2
typedef D3DXVECTOR2             Vector2D;
typedef D3DXVECTOR2             Pos2D;
typedef D3DXVECTOR2             Rot2D;
typedef D3DXVECTOR2             Normal2D;
typedef D3DXVECTOR2             Scale2D;
typedef D3DXVECTOR2             Size2D;
// Vector3
typedef D3DXVECTOR3             Vector3D;
typedef D3DXVECTOR3             Pos3D;
typedef D3DXVECTOR3             Rot3D;
typedef D3DXVECTOR3             Normal3D;
typedef D3DXVECTOR3             Scale3D;
typedef D3DXVECTOR3             Size3D;
// Other
typedef float                   Angle;
typedef D3DXMATRIX              Matrix;
typedef D3DMATERIAL9            Material;
typedef LPDIRECT3DDEVICE9       Device;
typedef LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
typedef LPDIRECT3DINDEXBUFFER9  IndexBuffer;
typedef LPD3DXMESH              Mesh;
typedef LPD3DXBUFFER            Buffer;
typedef LPDIRECT3DTEXTURE9      Texture;
typedef LPDIRECT3DSURFACE9      Surface;
typedef D3DVIEWPORT9            Viewport;