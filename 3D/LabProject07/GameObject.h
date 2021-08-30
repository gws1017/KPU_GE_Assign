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


	CMesh* m_pMesh = NULL;

	CShader* m_pShader = NULL;

public:
	XMFLOAT4X4 m_xmf4x4World;
public:
	void ReleaseUploadBuffers();

	virtual void SetMesh(CMesh* pMesh);

	virtual void SetShader(CShader* pShader);

	virtual void Animate(float fTimeElapsed);

	virtual void OnPrepareRender();

	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, UINT
		nInstances, D3D12_VERTEX_BUFFER_VIEW d3dInstancingBufferView);
	virtual void Render(ID3D12GraphicsCommandList* pd3dCommandList, CCamera* pCamera, UINT
		nInstances);

	void Rotate(XMFLOAT3* pxmf3Axis, float fAngle);
	void Rotate(float Pitch = 10.0f, float Yaw = 10.0f, float Roll = 10.0f);
	//위치 벡터와 방향벡터 반환
	XMFLOAT3 GetPosition();
	XMFLOAT3 GetLook();
	XMFLOAT3 GetUp();
	XMFLOAT3 GetRight();

	//게임 객체 위치 설정
	void SetPosition(float x, float y, float z);
	void SetPosition(XMFLOAT3 xmf3Pos);

	//게임 객체 로컬 x,y,z 축 방향이동
	void MoveStrafe(float Dist = 1.0f);
	void MoveUp(float Dist = 1.0f);
	void MoveForward(float Dist = 1.0f);

public:
	//상수버퍼 생성
	virtual void CreateShaderVariables(ID3D12Device* d3dDevice, ID3D12GraphicsCommandList* d3dCommandList);
	//상수버퍼 갱신
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