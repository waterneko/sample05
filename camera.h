//=============================================================================
//
// �J�������� [camera.h]
// Author : �N�V��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

void SetCamera(void);

D3DXVECTOR3 GetRotCamera(void);

D3DXMATRIX GetMtxView(void);

#endif
