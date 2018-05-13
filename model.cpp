//=============================================================================
//
// ���f������ [model.cpp]
// Author : �N�V��
//
//=============================================================================
#include "model.h"
#include "camera.h"
#include "input.h"
#include "shadow.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	MODEL_AIRPLANE		"data/MODEL/airplane000.x"	// �ǂݍ��ރ��f����
#define	VALUE_MOVE_MODEL	(0.50f)						// �ړ����x
#define	RATE_MOVE_MODEL		(0.20f)						// �ړ������W��
#define	VALUE_ROTATE_MODEL	(D3DX_PI * 0.05f)			// ��]���x
#define	RATE_ROTATE_MODEL	(0.20f)						// ��]�����W��

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9	g_pD3DTextureModel;			// �e�N�X�`���ǂݍ��ݏꏊ
LPD3DXMESH			g_pD3DXMeshModel;			// ID3DXMesh�C���^�[�t�F�C�X�ւ̃|�C���^
LPD3DXBUFFER		g_pD3DXBuffMatModel;		// ���b�V���̃}�e���A�������i�[
DWORD				g_nNumMatModel;				// �������̑���

D3DXMATRIX			g_mtxWorldModel;			// ���[���h�}�g���b�N�X
D3DXVECTOR3			g_posModel;					// ���݂̈ʒu
D3DXVECTOR3			g_moveModel;				// �ړ���
D3DXVECTOR3			g_rotModel;					// ���݂̌���
D3DXVECTOR3			g_rotDestModel;				// �ړI�̌���

int					g_nIdxShadow;				// �eID
float				g_fSizeShadow;				// �e�̃T�C�Y
D3DXCOLOR			g_colShadow;				// �e�̐F

//=============================================================================
// ����������
//=============================================================================
HRESULT InitModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	g_pD3DTextureModel = NULL;
	g_pD3DXMeshModel = NULL;
	g_pD3DXBuffMatModel = NULL;

	g_posModel = D3DXVECTOR3(0.0f, 20.0f, 0.0f);
	g_moveModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotDestModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// X�t�@�C���̓ǂݍ���
	if(FAILED(D3DXLoadMeshFromX(MODEL_AIRPLANE,
								D3DXMESH_SYSTEMMEM,
								pDevice,
								NULL,
								&g_pD3DXBuffMatModel,
								NULL,
								&g_nNumMatModel,
								&g_pD3DXMeshModel)))
	{
		return E_FAIL;
	}

#if 0
	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&g_pD3DTextureModel);	// �ǂݍ��ރ������[
#endif

	// �e�̐���
	g_nIdxShadow = CreateShadow(g_posModel, 25.0f, 25.0f);

	g_fSizeShadow = 25.0f;
	g_colShadow = D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitModel(void)
{
	if(g_pD3DTextureModel != NULL)
	{// �e�N�X�`���̊J��
		g_pD3DTextureModel->Release();
		g_pD3DTextureModel = NULL;
	}

	if(g_pD3DXMeshModel != NULL)
	{// ���b�V���̊J��
		g_pD3DXMeshModel->Release();
		g_pD3DXMeshModel = NULL;
	}

	if(g_pD3DXBuffMatModel != NULL)
	{// �}�e���A���̊J��
		g_pD3DXBuffMatModel->Release();
		g_pD3DXBuffMatModel = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateModel(void)
{
	D3DXVECTOR3 rotCamera;
	float fDiffRotY;

	// �J�����̌����擾
	rotCamera = GetRotCamera();
	

	if(GetKeyboardPress(DIK_LEFT))
	{
		if(GetKeyboardPress(DIK_UP))
		{// ���O�ړ�
			g_moveModel.x -= sinf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y + D3DX_PI * 0.75f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_DOWN))
		{// ����ړ�
			g_moveModel.x -= sinf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + D3DX_PI * 0.25f;
		}
		else
		{// ���ړ�
			g_moveModel.x -= sinf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y + D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y + D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_RIGHT))
	{
		if(GetKeyboardPress(DIK_UP))
		{// �E�O�ړ�
			g_moveModel.x -= sinf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y - D3DX_PI * 0.75f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - D3DX_PI * 0.75f;
		}
		else if(GetKeyboardPress(DIK_DOWN))
		{// �E��ړ�
			g_moveModel.x -= sinf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - D3DX_PI * 0.25f;
		}
		else
		{// �E�ړ�
			g_moveModel.x -= sinf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;
			g_moveModel.z -= cosf(rotCamera.y - D3DX_PI * 0.50f) * VALUE_MOVE_MODEL;

			g_rotDestModel.y = rotCamera.y - D3DX_PI * 0.50f;
		}
	}
	else if(GetKeyboardPress(DIK_UP))
	{// �O�ړ�
		g_moveModel.x -= sinf(D3DX_PI + rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= cosf(D3DX_PI + rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = D3DX_PI + rotCamera.y;
	}
	else if(GetKeyboardPress(DIK_DOWN))
	{// ��ړ�
		g_moveModel.x -= sinf(rotCamera.y) * VALUE_MOVE_MODEL;
		g_moveModel.z -= cosf(rotCamera.y) * VALUE_MOVE_MODEL;

		g_rotDestModel.y = rotCamera.y;
	}

	if(GetKeyboardPress(DIK_LSHIFT))
	{// ����]
		g_rotDestModel.y -= VALUE_ROTATE_MODEL;
		if(g_rotDestModel.y < -D3DX_PI)
		{
			g_rotDestModel.y += D3DX_PI * 2.0f;
		}
	}
	if(GetKeyboardPress(DIK_RSHIFT))
	{// �E��]
		g_rotDestModel.y += VALUE_ROTATE_MODEL;
		if(g_rotDestModel.y > D3DX_PI)
		{
			g_rotDestModel.y -= D3DX_PI * 2.0f;
		}
	}

	//if(GetKeyboardPress(DIK_I))
	//{// �㏸
	//	g_moveModel.y += VALUE_MOVE_MODEL * 0.5f;

	//	if(g_posModel.y < 75.0f)
	//	{
	//		g_fSizeShadow += 0.2f;
	//		g_colShadow.a -= 0.004f;
	//	}
	//}
	//if(GetKeyboardPress(DIK_K))
	//{// ���~
	//	g_moveModel.y -= VALUE_MOVE_MODEL * 0.5f;

	//	if(g_posModel.y > 5.0f)
	//	{
	//		g_fSizeShadow -= 0.2f;
	//		g_colShadow.a += 0.004f;
	//	}
	//}

	// �ړI�̊p�x�܂ł̍���
	fDiffRotY = g_rotDestModel.y - g_rotModel.y;
	if(fDiffRotY > D3DX_PI)
	{
		fDiffRotY -= D3DX_PI * 2.0f;
	}
	if(fDiffRotY < -D3DX_PI)
	{
		fDiffRotY += D3DX_PI * 2.0f;
	}

	// �ړI�̊p�x�܂Ŋ�����������
	g_rotModel.y += fDiffRotY * RATE_ROTATE_MODEL;
	if(g_rotModel.y > D3DX_PI)
	{
		g_rotModel.y -= D3DX_PI * 2.0f;
	}
	if(g_rotModel.y < -D3DX_PI)
	{
		g_rotModel.y += D3DX_PI * 2.0f;
	}

	/// �ʒu�ړ�
	g_posModel.x += g_moveModel.x;
	g_posModel.y += g_moveModel.y;
	if(g_posModel.y < 5.0f)
	{
		g_posModel.y = 5.0f;
	}
	if(g_posModel.y > 75.0f)
	{
		g_posModel.y = 75.0f;
	}
	g_posModel.z += g_moveModel.z;

	// �ړ��ʂɊ�����������
	g_moveModel.x += (0.0f - g_moveModel.x) * RATE_MOVE_MODEL;
	g_moveModel.y += (0.0f - g_moveModel.y) * RATE_MOVE_MODEL;
	g_moveModel.z += (0.0f - g_moveModel.z) * RATE_MOVE_MODEL;

	if(g_posModel.x < -310.0f)
	{
		g_posModel.x = -310.0f;
	}
	if(g_posModel.x > 310.0f)
	{
		g_posModel.x = 310.0f;
	}
	if(g_posModel.z < -310.0f)
	{
		g_posModel.z = -310.0f;
	}
	if(g_posModel.z > 310.0f)
	{
		g_posModel.z = 310.0f;
	}

	// �e�̈ʒu�ݒ�
	SetPositionShadow(g_nIdxShadow, D3DXVECTOR3(g_posModel.x, 0.1f, g_posModel.z));

	SetVertexShadow(g_nIdxShadow, g_fSizeShadow, g_fSizeShadow);
	SetColorShadow(g_nIdxShadow, g_colShadow);

	//PrintDebugProc("[��s�@�̈ʒu  �F(%f : %f : %f)]\n", g_posModel.x, g_posModel.y, g_posModel.z);
	//PrintDebugProc("[��s�@�̌���  �F(%f) < �ړI�̌���:(%f) >]\n", g_rotModel.y, g_rotDestModel.y);
	//PrintDebugProc("\n");

	//PrintDebugProc("*** ��s�@���� ***\n");
	//PrintDebugProc("�O�ړ� : ��\n");
	//PrintDebugProc("��ړ� : ��\n");
	//PrintDebugProc("���ړ� : ��\n");
	//PrintDebugProc("�E�ړ� : ��\n");
	//PrintDebugProc("�㏸   : �h\n");
	//PrintDebugProc("���~   : �j\n");
	//PrintDebugProc("������ : LSHIFT\n");
	//PrintDebugProc("�E���� : RSHIFT\n");
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawModel(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTranslate;
	D3DXMATERIAL *pD3DXMat;
	D3DMATERIAL9 matDef;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldModel);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotModel.y, g_rotModel.x, g_rotModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posModel.x, g_posModel.y, g_posModel.z);
	D3DXMatrixMultiply(&g_mtxWorldModel, &g_mtxWorldModel, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldModel);

	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	// �}�e���A�����ɑ΂���|�C���^���擾
	pD3DXMat = (D3DXMATERIAL*)g_pD3DXBuffMatModel->GetBufferPointer();

	for(int nCntMat = 0; nCntMat < (int)g_nNumMatModel; nCntMat++)
	{
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&pD3DXMat[nCntMat].MatD3D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pD3DTextureModel);

		// �`��
		g_pD3DXMeshModel->DrawSubset(nCntMat);
	}

	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

//=============================================================================
// �ʒu�擾
//=============================================================================
D3DXVECTOR3 GetPositionModel(void)
{
	return g_posModel;
}

//=============================================================================
// �����擾
//=============================================================================
D3DXVECTOR3 GetRotationModel(void)
{
	return g_rotModel;
}

//=============================================================================
// �ړI�̌����擾
//=============================================================================
D3DXVECTOR3 GetRotationDestModel(void)
{
	return g_rotDestModel;
}
