//=============================================================================
//
// 木処理 [tree.cpp]
// Author : 潘澤泓
//
//=============================================================================
#include "tree.h"
#include "camera.h"
#include "shadow.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	TEXTURE_TREE		"data/TEXTURE/tree000.png"		// 読み込むテクスチャファイル名
#define	TREE_WIDTH			(50.0f)							// 弾の半径幅
#define	TREE_HEIGHT			(80.0f)							// 弾の半径幅
#define	VALUE_MOVE_TREE		(3.00f)							// 移動速度

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT MakeVertexTree(LPDIRECT3DDEVICE9 pDevice);
void SetVertexTree(int nIdxTree, float fWidth, float fHeight);
void SetColorTree(int nIdxTree, D3DXCOLOR col);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECT3DTEXTURE9		g_apD3DTextureTree[5] = {};		// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffTree = NULL;		// 頂点バッファインターフェースへのポインタ

TREE					g_aTree[MAX_TREE];				// 木ワーク
bool					g_bAlpaTest;					// アルファテストON/OFF
int						g_nAlpha;						// アルファテストの閾値

char *g_aFileName[] =
{
	"data/TEXTURE/tree001.png",
	"data/TEXTURE/tree002.png",
	"data/TEXTURE/tree003.png",
	"data/TEXTURE/tree004.png",
	"data/TEXTURE/tree005.png",
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitTree(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点情報の作成
	MakeVertexTree(pDevice);

	// テクスチャの読み込み
	for(int nCntTex = 0; nCntTex < 5; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,							// デバイスへのポインタ
									g_aFileName[nCntTex],			// ファイルの名前
									&g_apD3DTextureTree[nCntTex]);	// 読み込むメモリー
	}

	for(int nCntTree = 0; nCntTree < MAX_TREE; nCntTree++)
	{
		g_aTree[nCntTree].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTree[nCntTree].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aTree[nCntTree].fWidth = TREE_WIDTH;
		g_aTree[nCntTree].fHeight = TREE_HEIGHT;
		g_aTree[nCntTree].bUse = false;
	}

	g_bAlpaTest = false;
	g_nAlpha = 0x0;

	// 木の設定
	SetTree(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 60.0f, 90.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTree(D3DXVECTOR3(200.0f, 0.0f, 0.0f), 60.0f, 90.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTree(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), 60.0f, 90.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTree(D3DXVECTOR3(0.0f, 0.0f, 200.0f), 60.0f, 90.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	SetTree(D3DXVECTOR3(0.0f, 0.0f, -200.0f), 60.0f, 90.0f, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitTree(void)
{
	for(int nCntTex = 0; nCntTex < 5; nCntTex++)
	{
		if(g_apD3DTextureTree[nCntTex] != NULL)
		{// テクスチャの開放
			g_apD3DTextureTree[nCntTex]->Release();
			g_apD3DTextureTree[nCntTex] = NULL;
		}
	}

	if(g_pD3DVtxBuffTree != NULL)
	{// 頂点バッファの開放
		g_pD3DVtxBuffTree->Release();
		g_pD3DVtxBuffTree = NULL;
	}
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateTree(void)
{
	//D3DXVECTOR3 rotCamera;

	//// カメラの回転を取得
	//rotCamera = GetRotCamera();

	//if(GetKeyboardPress(DIK_LEFT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// 左前移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_TREE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// 左後移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_TREE;
	//	}
	//	else
	//	{// 左移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_TREE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_RIGHT))
	//{
	//	if(GetKeyboardPress(DIK_UP))
	//	{// 右前移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_TREE;
	//	}
	//	else if(GetKeyboardPress(DIK_DOWN))
	//	{// 右後移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_TREE;
	//	}
	//	else
	//	{// 右移動
	//		g_aTree[0].pos.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_TREE;
	//		g_aTree[0].pos.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_TREE;
	//	}
	//}
	//else if(GetKeyboardPress(DIK_UP))
	//{// 前移動
	//	g_aTree[0].pos.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_TREE;
	//	g_aTree[0].pos.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_TREE;
	//}
	//else if(GetKeyboardPress(DIK_DOWN))
	//{// 後移動
	//	g_aTree[0].pos.x -= sinf(rotCamera.y) * VALUE_MOVE_TREE;
	//	g_aTree[0].pos.z -= cosf(rotCamera.y) * VALUE_MOVE_TREE;
	//}

	for(int nCntTree = 0; nCntTree < MAX_TREE; nCntTree++)
	{
		if(g_aTree[nCntTree].bUse)
		{
			// 影の位置設定
			SetPositionShadow(g_aTree[nCntTree].nIdxShadow, D3DXVECTOR3(g_aTree[nCntTree].pos.x, 0.1f, g_aTree[nCntTree].pos.z));
		}
	}

	// アルファテストON/OFF
	if(GetKeyboardTrigger(DIK_F1))
	{
		g_bAlpaTest = g_bAlpaTest ? false: true;
	}

	// アルファテストの閾値変更
	if(GetKeyboardPress(DIK_I))
	{
		g_nAlpha--;
		if(g_nAlpha < 0)
		{
			g_nAlpha = 0;
		}
	}
	if(GetKeyboardPress(DIK_K))
	{
		g_nAlpha++;
		if(g_nAlpha > 255)
		{
			g_nAlpha = 255;
		}
	}

	//PrintDebugProc("*** 木操作 ***\n");
	//PrintDebugProc("前移動   : ↑\n");
	//PrintDebugProc("後移動   : ↓\n");
	//PrintDebugProc("左移動   : ←\n");
	//PrintDebugProc("右移動   : →\n");
	//PrintDebugProc("\n");

	PrintDebugProc("*** アルファテスト ***\n");
	PrintDebugProc("ON / OFF：[F1] ( 現在：");
	if(g_bAlpaTest == true)
	{
		PrintDebugProc("ON");
	}
	else
	{
		PrintDebugProc("OFF");
	}
	PrintDebugProc(" )\n");

	if(g_bAlpaTest == true)
	{
		PrintDebugProc("アルファ閾値：減少[I] / 増加[K] ( 閾値：%d )\n", g_nAlpha);
	}
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawTree(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxView, mtxScale, mtxTranslate;

	// αテスト設定
	if(g_bAlpaTest == true)
	{
		// αテストを有効に
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, g_nAlpha);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	}

	// ラインティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for(int nCntTree = 0; nCntTree < MAX_TREE; nCntTree++)
	{
		if(g_aTree[nCntTree].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aTree[nCntTree].mtxWorld);

			// ビューマトリックスを取得
			mtxView = GetMtxView();

			// ポリゴンを正面に向ける
#if 1
			// 逆行列をもとめる
			D3DXMatrixInverse(&g_aTree[nCntTree].mtxWorld, NULL, &mtxView);
			g_aTree[nCntTree].mtxWorld._41 = 0.0f;
			g_aTree[nCntTree].mtxWorld._42 = 0.0f;
			g_aTree[nCntTree].mtxWorld._43 = 0.0f;
#else
			g_aTree[nCntTree].mtxWorld._11 = mtxView._11;
			g_aTree[nCntTree].mtxWorld._12 = mtxView._21;
			g_aTree[nCntTree].mtxWorld._13 = mtxView._31;
			g_aTree[nCntTree].mtxWorld._21 = mtxView._12;
			g_aTree[nCntTree].mtxWorld._22 = mtxView._22;
			g_aTree[nCntTree].mtxWorld._23 = mtxView._32;
			g_aTree[nCntTree].mtxWorld._31 = mtxView._13;
			g_aTree[nCntTree].mtxWorld._32 = mtxView._23;
			g_aTree[nCntTree].mtxWorld._33 = mtxView._33;
#endif

			// スケールを反映
			D3DXMatrixScaling(&mtxScale, g_aTree[nCntTree].scl.x,
											g_aTree[nCntTree].scl.y,
											g_aTree[nCntTree].scl.z);
			D3DXMatrixMultiply(&g_aTree[nCntTree].mtxWorld, &g_aTree[nCntTree].mtxWorld, &mtxScale);

			// 移動を反映
			D3DXMatrixTranslation(&mtxTranslate, g_aTree[nCntTree].pos.x,
													g_aTree[nCntTree].pos.y,
													g_aTree[nCntTree].pos.z);
			D3DXMatrixMultiply(&g_aTree[nCntTree].mtxWorld, &g_aTree[nCntTree].mtxWorld, &mtxTranslate);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aTree[nCntTree].mtxWorld);

			// 頂点バッファをデバイスのデータストリームにバインド
			pDevice->SetStreamSource(0, g_pD3DVtxBuffTree, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apD3DTextureTree[nCntTree % 5]);

			// ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntTree * 4), NUM_POLYGON);
		}
	}

	// ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// αテストを無効に
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
HRESULT MakeVertexTree(LPDIRECT3DDEVICE9 pDevice)
{
	// オブジェクトの頂点バッファを生成
	if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VERTEX * MAX_TREE,	// 頂点データ用に確保するバッファサイズ(バイト単位)
												D3DUSAGE_WRITEONLY,						// 頂点バッファの使用法　
												FVF_VERTEX_3D,							// 使用する頂点フォーマット
												D3DPOOL_MANAGED,						// リソースのバッファを保持するメモリクラスを指定
												&g_pD3DVtxBuffTree,						// 頂点バッファインターフェースへのポインタ
												NULL)))									// NULLに設定
	{
		return E_FAIL;
	}

	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTree->Lock(0, 0, (void**)&pVtx, 0);

		for(int nCntTree = 0; nCntTree < MAX_TREE; nCntTree++, pVtx += 4)
		{
			// 頂点座標の設定
			pVtx[0].vtx = D3DXVECTOR3(-TREE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[1].vtx = D3DXVECTOR3(-TREE_WIDTH / 2.0f, TREE_HEIGHT, 0.0f);
			pVtx[2].vtx = D3DXVECTOR3(TREE_WIDTH / 2.0f, 0.0f, 0.0f);
			pVtx[3].vtx = D3DXVECTOR3(TREE_WIDTH / 2.0f, TREE_HEIGHT, 0.0f);

			// 反射光の設定
			pVtx[0].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			// テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 0.0f);
		}

		// 頂点データをアンロックする
		g_pD3DVtxBuffTree->Unlock();
	}

	return S_OK;
}

//=============================================================================
// 頂点座標の設定
//=============================================================================
void SetVertexTree(int nIdxTree, float fWidth, float fHeight)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTree->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxTree * 4);

		// 頂点座標の設定
		pVtx[0].vtx = D3DXVECTOR3(-fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[1].vtx = D3DXVECTOR3(-fWidth / 2.0f, fHeight, 0.0f);
		pVtx[2].vtx = D3DXVECTOR3(fWidth / 2.0f, 0.0f, 0.0f);
		pVtx[3].vtx = D3DXVECTOR3(fWidth / 2.0f, fHeight, 0.0f);

		// 頂点データをアンロックする
		g_pD3DVtxBuffTree->Unlock();
	}
}

//=============================================================================
// 頂点カラーの設定
//=============================================================================
void SetColorTree(int nIdxTree, D3DXCOLOR col)
{
	{//頂点バッファの中身を埋める
		VERTEX_3D *pVtx;

		// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
		g_pD3DVtxBuffTree->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += (nIdxTree * 4);

		// 頂点座標の設定
		pVtx[0].diffuse = col;
		pVtx[1].diffuse = col;
		pVtx[2].diffuse = col;
		pVtx[3].diffuse = col;

		// 頂点データをアンロックする
		g_pD3DVtxBuffTree->Unlock();
	}
}

//=============================================================================
// 頂点情報の作成
//=============================================================================
int SetTree(D3DXVECTOR3 pos, float fWidth, float fHeight, D3DXCOLOR col)
{
	int nIdxTree = -1;

	for(int nCntTree = 0; nCntTree < MAX_TREE; nCntTree++)
	{
		if(!g_aTree[nCntTree].bUse)
		{
			g_aTree[nCntTree].pos = pos;
			g_aTree[nCntTree].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			g_aTree[nCntTree].fWidth = fWidth;
			g_aTree[nCntTree].fHeight = fHeight;
			g_aTree[nCntTree].bUse = true;

			// 頂点座標の設定
			SetVertexTree(nCntTree, fWidth, fHeight);

			// 頂点カラーの設定
			SetColorTree(nCntTree, col);

			// 影の設定
			g_aTree[nCntTree].nIdxShadow = CreateShadow(g_aTree[nCntTree].pos, g_aTree[nCntTree].fWidth, g_aTree[nCntTree].fWidth);

			nIdxTree = nCntTree;

			break;
		}
	}

	return nIdxTree;
}

//=============================================================================
// 木を取得
//=============================================================================
TREE *GetTree(void)
{
	return &g_aTree[0];
}

