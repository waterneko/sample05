//=============================================================================
//
// ���b�V���n�ʂ̏��� [meshfield.cpp]
// Author : �N�V��
//
//=============================================================================
#include "meshfield.h"
#include "input.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_FILENAME	"data/TEXTURE/field004.jpg"		// �ǂݍ��ރe�N�X�`���t�@�C����

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pD3DTextureField;			// �e�N�X�`���ǂݍ��ݏꏊ
LPDIRECT3DVERTEXBUFFER9 g_pD3DVtxBuffField;		// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pD3DIdxBuffField;		// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^

D3DXMATRIX g_mtxWorldField;						// ���[���h�}�g���b�N�X
D3DXVECTOR3 g_posField;							// �|���S���\���ʒu�̒��S���W
D3DXVECTOR3 g_rotField;							// �|���S���̉�]�p

int g_nNumBlockXField, g_nNumBlockZField;		// �u���b�N��
int g_nNumVertexField;							// �����_��	
int g_nNumVertexIndexField;						// ���C���f�b�N�X��
int g_nNumPolygonField;							// ���|���S����
float g_fBlockSizeXField, g_fBlockSizeZField;	// �u���b�N�T�C�Y

//=============================================================================
// ����������
//=============================================================================
HRESULT InitMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot,
							int nNumBlockX, int nNumBlockZ, float fBlockSizeX, float fBlockSizeZ)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �|���S���\���ʒu�̒��S���W��ݒ�
	g_posField = pos;

	g_rotField = rot;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,					// �f�o�C�X�ւ̃|�C���^
								TEXTURE_FILENAME,		// �t�@�C���̖��O
								&g_pD3DTextureField);	// �ǂݍ��ރ������[

	// �u���b�N���̐ݒ�
	g_nNumBlockXField = nNumBlockX;
	g_nNumBlockZField = nNumBlockZ;

	// ���_���̐ݒ�
	g_nNumVertexField = (nNumBlockX + 1) * (nNumBlockZ + 1);

	// �C���f�b�N�X���̐ݒ�
	g_nNumVertexIndexField = (nNumBlockX + 1) * 2 * nNumBlockZ + (nNumBlockZ - 1) * 2;

	// �|���S�����̐ݒ�
	g_nNumPolygonField = nNumBlockX * nNumBlockZ * 2 + (nNumBlockZ - 1) * 4;

	// �u���b�N�T�C�Y�̐ݒ�
	g_fBlockSizeXField = fBlockSizeX;
	g_fBlockSizeZField = fBlockSizeZ;

	// �I�u�W�F�N�g�̒��_�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexField,	// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												FVF_VERTEX_3D,						// �g�p���钸�_�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DVtxBuffField,				// ���_�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	// �I�u�W�F�N�g�̃C���f�b�N�X�o�b�t�@�𐶐�
    if(FAILED(pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumVertexIndexField,		// ���_�f�[�^�p�Ɋm�ۂ���o�b�t�@�T�C�Y(�o�C�g�P��)
												D3DUSAGE_WRITEONLY,					// ���_�o�b�t�@�̎g�p�@�@
												D3DFMT_INDEX16,						// �g�p����C���f�b�N�X�t�H�[�}�b�g
												D3DPOOL_MANAGED,					// ���\�[�X�̃o�b�t�@��ێ����郁�����N���X���w��
												&g_pD3DIdxBuffField,				// �C���f�b�N�X�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
												NULL)))								// NULL�ɐݒ�
	{
        return E_FAIL;
	}

	{//���_�o�b�t�@�̒��g�𖄂߂�
		VERTEX_3D *pVtx;
#if 0
		const float texSizeX = 1.0f / g_nNumBlockX;
		const float texSizeZ = 1.0f / g_nNumBlockZ;
#else
		const float texSizeX = 1.0f;
		const float texSizeZ = 1.0f;
#endif

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DVtxBuffField->Lock( 0, 0, (void**)&pVtx, 0 );

		for(int nCntVtxZ = 0; nCntVtxZ < (g_nNumBlockZField + 1); nCntVtxZ++)
		{
			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				// ���_���W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.x = -(g_nNumBlockXField / 2.0f) * g_fBlockSizeXField + nCntVtxX * g_fBlockSizeXField;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.y = 0.0f;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].vtx.z = (g_nNumBlockZField / 2.0f) * g_fBlockSizeZField - nCntVtxZ * g_fBlockSizeZField;

				// �@���̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].nor = D3DXVECTOR3(0.0f, 1.0, 0.0f);

				// ���ˌ��̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				// �e�N�X�`�����W�̐ݒ�
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.x = texSizeX * nCntVtxX;
				pVtx[nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX].tex.y = texSizeZ * nCntVtxZ;
			}
		}

		// ���_�f�[�^���A�����b�N����
		g_pD3DVtxBuffField->Unlock();
	}

	{//�C���f�b�N�X�o�b�t�@�̒��g�𖄂߂�
		WORD *pIdx;

		// �C���f�b�N�X�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^���擾
		g_pD3DIdxBuffField->Lock( 0, 0, (void**)&pIdx, 0 );

		int nCntIdx = 0;
		for(int nCntVtxZ = 0; nCntVtxZ < g_nNumBlockZField; nCntVtxZ++)
		{
			if(nCntVtxZ > 0)
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1);
				nCntIdx++;
			}

			for(int nCntVtxX = 0; nCntVtxX < (g_nNumBlockXField + 1); nCntVtxX++)
			{
				pIdx[nCntIdx] = (nCntVtxZ + 1) * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + nCntVtxX;
				nCntIdx++;
			}

			if(nCntVtxZ < (g_nNumBlockZField - 1))
			{// �k�ރ|���S���̂��߂̃_�u��̐ݒ�
				pIdx[nCntIdx] = nCntVtxZ * (g_nNumBlockXField + 1) + g_nNumBlockXField;
				nCntIdx++;
			}
		}

		// �C���f�b�N�X�f�[�^���A�����b�N����
		g_pD3DIdxBuffField->Unlock();
	}

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitMeshField(void)
{
	if(g_pD3DVtxBuffField)
	{// ���_�o�b�t�@�̊J��
		g_pD3DVtxBuffField->Release();
		g_pD3DVtxBuffField = NULL;
	}

	if(g_pD3DIdxBuffField)
	{// �C���f�b�N�X�o�b�t�@�̊J��
		g_pD3DIdxBuffField->Release();
		g_pD3DIdxBuffField = NULL;
	}

	if(g_pD3DTextureField)
	{// �e�N�X�`���̊J��
		g_pD3DTextureField->Release();
		g_pD3DTextureField = NULL;
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateMeshField(void)
{
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawMeshField(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTranslate;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_mtxWorldField);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_rotField.y, g_rotField.x, g_rotField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTranslate, g_posField.x, g_posField.y, g_posField.z);
	D3DXMatrixMultiply(&g_mtxWorldField, &g_mtxWorldField, &mtxTranslate);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldField);

	// ���_�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetStreamSource(0, g_pD3DVtxBuffField, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@�������_�����O�p�C�v���C���ɐݒ�
	pDevice->SetIndices(g_pD3DIdxBuffField);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pD3DTextureField);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_nNumVertexField, 0, g_nNumPolygonField);
}

