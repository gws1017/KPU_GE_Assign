#pragma once
#include "Mesh.h"
#include "Camera.h"

class CShader;

class CGameObject
{
public:
	CGameObject();

	virtual ~CGameObject();

private:
	int m_nReferences = 0;

public:
	void AddRef() { m_nReferences++; }

	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	XMFLOAT4X4 m_xmf4x4World;

	CMesh* m_pMesh = NULL;

	CShader* m_pShader = NULL;

public:
	void ReleaseUploadBuffers();

	virtual void SetMesh(CMesh* pMesh);

	virtual void SetShader(CShader* pShader);

	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);

	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(float Pitch = 10.0f, float Yaw = 10.0f, float Roll = 10.0f);
	//��ġ ���Ϳ� ���⺤�� ��ȯ
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	//���� ��ü ��ġ ����
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Pos);

	//���� ��ü ���� x,y,z �� �����̵�
	void MoveStrafe(float Dist = 1.0f);
	void MoveUp(float Dist = 1.0f);
	void MoveForward(float Dist = 1.0f);

	//���� ��ü�� ī�޶� ���δ� ���� �˻��Ѵ�. 
	bool IsVisible(CCamera *pCamera=NULL);

	//�� ��ǥ���� ��ŷ ������ �����Ѵ�. 
	void GenerateRayForPicking(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4& xmf4x4View, 
	XMFLOAT3* pxmf3PickRayOrigin, XMFLOAT3* pxmf3PickRayDirection);
	//ī�޶� ��ǥ���� �� ���� ���� �� ��ǥ���� ��ŷ ������ �����ϰ� ��ü���� ������ �˻��Ѵ�. 
	int PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4& xmf4x4View, 
	float* pfHitDistance);

public:
	//������� ����
	virtual void CreateShaderVariables(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* d3dCommandList);
	//������� ����
	virtual void UpdateShaderVariables(ID3D12GraphicsCommandList* d3dCommandList);
	virtual void ReleaseShaderVariables();
};

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject();
	virtual ~CRotatingObject();

private:
	XMFLOAT3 m_xmf3RotationAxis;
	float m_fRotationSpeed;

public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	
	void SetRotationAxis(XMFLOAT3 xmf3RotationAxis) { m_xmf3RotationAxis = xmf3RotationAxis; }
	
	virtual void Animate(float fTimeElapsed);
};