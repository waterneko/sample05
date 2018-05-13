//=============================================================================
//
// ���f������ [model.h]
// Author : �N�V��
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(void);

D3DXVECTOR3 GetPositionModel(void);
D3DXVECTOR3 GetRotationModel(void);
D3DXVECTOR3 GetRotationDestModel(void);

#endif
