//=============================================================================
//
// �J�������� [camera.cpp]
// Author : �N�V��
//
//=============================================================================
#include "camera.h"
#include "model.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	VIEW_ANGLE			(D3DXToRadian(45.0f))	// ����p
#define	VIEW_ASPECT			((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT)	// �r���[���ʂ̃A�X�y�N�g��
#define	VIEW_NEAR_Z			(10.0f)					// �r���[���ʂ�NearZ�l
#define	VIEW_FAR_Z			(1000.0f)				// �r���[���ʂ�FarZ�l
#define	VALUE_MOVE_CAMERA	(2.0f)					// �J�����̈ړ���
#define	VALUE_ROTATE_CAMERA	(D3DX_PI * 0.01f)		// �J�����̉�]��

#define	INTERVAL_CAMERA_R	(12.5f)					// ���f���̎����̐�܂ł̋���
#define	RATE_CHASE_CAMERA_P	(0.35f)					// �J�����̎��_�ւ̕␳�W��
#define	RATE_CHASE_CAMERA_R	(0.20f)					// �J�����̒����_�ւ̕␳�W��

#define	CHASE_HEIGHT_P		(100.0f)				// �ǐՎ��̎��_�̍���
#define	CHASE_HEIGHT_R		(10.0f)					// �ǐՎ��̒����_�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
D3DXVECTOR3		g_posCameraP;				// �J�����̎��_
D3DXVECTOR3		g_posCameraR;				// �J�����̒����_
D3DXVECTOR3		g_posCameraU;				// �J�����̏����
D3DXVECTOR3		g_posCameraPDest;			// �J�����̎��_�̖ړI�ʒu
D3DXVECTOR3		g_posCameraRDest;			// �J�����̒����_�̖ړI�ʒu
D3DXVECTOR3		g_rotCamera;				// �J�����̉�]
float			g_fLengthIntervalCamera;	// �J�����̎��_�ƒ����_�̋���
D3DXMATRIX		g_mtxView;					// �r���[�}�g���b�N�X
D3DXMATRIX		g_mtxProjection;			// �v���W�F�N�V�����}�g���b�N�X

//=============================================================================
// �J�����̏�����
//=============================================================================
HRESULT InitCamera(void)
{
	g_posCameraP = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
	g_posCameraR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_posCameraU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_posCameraPDest = D3DXVECTOR3(0.0f, 100.0f, -200.0f);
	g_posCameraRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	float vx,vz;
	vx = g_posCameraP.x - g_posCameraR.x;
	vz = g_posCameraP.z - g_posCameraR.z;
	g_fLengthIntervalCamera = sqrtf(vx * vx + vz * vz);

	return S_OK;
}

//=============================================================================
// �J�����̏I������
//=============================================================================
void UninitCamera(void)
{
}

//=============================================================================
// �J�����̍X�V����
//=============================================================================
void UpdateCamera(void)
{
	D3DXVECTOR3 posModel;

	//�v���C���[�̈ʒu�̎擾
	posModel = GetPositionModel(); 

	g_posCameraR = posModel;
	
	//�J�����̎��_�̔���
	g_posCameraP.x = g_posCameraR.x - sin(g_rotCamera.y) * g_fLengthIntervalCamera;
	g_posCameraP.y = g_posCameraR.y + 100.0f;
	g_posCameraP.z = g_posCameraR.z - cos(g_rotCamera.y) * g_fLengthIntervalCamera;


	//g_posCameraPDest = rotDestModel;
	//if(GetKeyboardPress(DIK_A))
	//{
	//	if(GetKeyboardPress(DIK_W))
	//	{// ���O�ړ�
	//		g_posCameraP.x -= cosf(g_rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z += sinf(g_rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//	}
	//	else if(GetKeyboardPress(DIK_S))
	//	{// ����ړ�
	//		g_posCameraP.x -= cosf(g_rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z += sinf(g_rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//	}
	//	else
	//	{// ���ړ�
	//		g_posCameraP.x -= cosf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z += sinf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//	}

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//else if(GetKeyboardPress(DIK_D))
	//{
	//	if(GetKeyboardPress(DIK_W))
	//	{// �E�O�ړ�
	//		g_posCameraP.x += cosf(g_rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z -= sinf(g_rotCamera.y - D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//	}
	//	else if(GetKeyboardPress(DIK_S))
	//	{// �E��ړ�
	//		g_posCameraP.x += cosf(g_rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z -= sinf(g_rotCamera.y + D3DX_PI * 0.25f) * VALUE_MOVE_CAMERA;
	//	}
	//	else
	//	{// �E�ړ�
	//		g_posCameraP.x += cosf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//		g_posCameraP.z -= sinf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//	}

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//else if(GetKeyboardPress(DIK_W))
	//{// �O�ړ�
	//	g_posCameraP.x += sinf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//	g_posCameraP.z += cosf(g_rotCamera.y) * VALUE_MOVE_CAMERA;

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//else if(GetKeyboardPress(DIK_S))
	//{// ��ړ�
	//	g_posCameraP.x -= sinf(g_rotCamera.y) * VALUE_MOVE_CAMERA;
	//	g_posCameraP.z -= cosf(g_rotCamera.y) * VALUE_MOVE_CAMERA;

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}

	if(GetKeyboardPress(DIK_Z))
	{// ���_����u���v
		g_rotCamera.y += VALUE_ROTATE_CAMERA;
		if(g_rotCamera.y > D3DX_PI)
		{
			g_rotCamera.y -= D3DX_PI * 2.0f;
		}

		g_posCameraP.x = g_posCameraR.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
		g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	}
	if(GetKeyboardPress(DIK_C))
	{// ���_����u�E�v
		g_rotCamera.y -= VALUE_ROTATE_CAMERA;
		if(g_rotCamera.y < -D3DX_PI)
		{
			g_rotCamera.y += D3DX_PI * 2.0f;
		}

		g_posCameraP.x = g_posCameraR.x - sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
		g_posCameraP.z = g_posCameraR.z - cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	}

	//if(GetKeyboardPress(DIK_Y))
	//{// ���_�ړ��u��v
	//	g_posCameraP.y += VALUE_MOVE_CAMERA;
	//}
	//if(GetKeyboardPress(DIK_N))
	//{// ���_�ړ��u���v
	//	g_posCameraP.y -= VALUE_MOVE_CAMERA;
	//}

	//if(GetKeyboardPress(DIK_Q))
	//{// �����_����u���v
	//	g_rotCamera.y -= VALUE_ROTATE_CAMERA;
	//	if(g_rotCamera.y < -D3DX_PI)
	//	{
	//		g_rotCamera.y += D3DX_PI*2.0f;
	//	}

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//if(GetKeyboardPress(DIK_E))
	//{// �����_����u�E�v
	//	g_rotCamera.y += VALUE_ROTATE_CAMERA;
	//	if(g_rotCamera.y > D3DX_PI)
	//	{
	//		g_rotCamera.y -= D3DX_PI*2.0f;
	//	}

	//	g_posCameraR.x = g_posCameraP.x + sinf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//	g_posCameraR.z = g_posCameraP.z + cosf(g_rotCamera.y) * g_fLengthIntervalCamera;
	//}
	//if(GetKeyboardPress(DIK_T))
	//{// �����_�ړ��u��v
	//	g_posCameraR.y += VALUE_MOVE_CAMERA;
	//}
	//if(GetKeyboardPress(DIK_B))
	//{// �����_�ړ��u���v
	//	g_posCameraR.y -= VALUE_MOVE_CAMERA;
	//}

	PrintDebugProc("[�J�����̎��_  �F(%f : %f : %f)]\n", g_posCameraP.x, g_posCameraP.y, g_posCameraP.z);
	PrintDebugProc("[�J�����̒����_�F(%f : %f : %f)]\n", g_posCameraR.x, g_posCameraR.y, g_posCameraR.z);
	PrintDebugProc("[�J�����̌���  �F(%f)]\n", g_rotCamera.y);
	PrintDebugProc("\n");

	//PrintDebugProc("*** ���_���� ***\n");
	//PrintDebugProc("�O�ړ� : W\n");
	//PrintDebugProc("��ړ� : S\n");
	//PrintDebugProc("���ړ� : A\n");
	//PrintDebugProc("�E�ړ� : D\n");
	//PrintDebugProc("��ړ� : Y\n");
	//PrintDebugProc("���ړ� : N\n");
	//PrintDebugProc("������ : Z\n");
	//PrintDebugProc("�E���� : C\n");
	//PrintDebugProc("\n");

	//PrintDebugProc("*** �����_���� ***\n");
	//PrintDebugProc("��ړ� : T\n");
	//PrintDebugProc("���ړ� : B\n");
	//PrintDebugProc("������ : Q\n");
	//PrintDebugProc("�E���� : E\n");
	//PrintDebugProc("\n");
}

//=============================================================================
// �J�����̐ݒ菈��
//=============================================================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_mtxView, 
						&g_posCameraP,		// �J�����̎��_
						&g_posCameraR,		// �J�����̒����_
						&g_posCameraU);		// �J�����̏����

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_mtxView);


	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_mtxProjection,
								VIEW_ANGLE,			// ����p
								VIEW_ASPECT,		// �A�X�y�N�g��
								VIEW_NEAR_Z,		// �r���[���ʂ�NearZ�l
								VIEW_FAR_Z);		// �r���[���ʂ�FarZ�l

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_mtxProjection);
}

//=============================================================================
// �J�����̌����̎擾
//=============================================================================
D3DXVECTOR3 GetRotCamera(void)
{
	return g_rotCamera;
}

//=============================================================================
// �r���[�}�g���b�N�X�̎擾
//=============================================================================
D3DXMATRIX GetMtxView(void)
{
	return g_mtxView;
}

