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

	//게임 객체가 카메라에 보인는 가를 검사한다. 
	bool IsVisible(CCamera *pCamera=NULL);

	//모델 좌표계의 픽킹 광선을 생성한다. 
	void GenerateRayForPicking(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4& xmf4x4View, 
	XMFLOAT3* pxmf3PickRayOrigin, XMFLOAT3* pxmf3PickRayDirection);
	//카메라 좌표계의 한 점에 대한 모델 좌표계의 픽킹 광선을 생성하고 객체와의 교차를 검사한다. 
	int PickObjectByRayIntersection(XMFLOAT3& xmf3PickPosition, XMFLOAT4X4& xmf4x4View, 
	float* pfHitDistance);

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