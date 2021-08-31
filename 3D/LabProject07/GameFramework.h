#pragma once
#include "Timer.h"
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class CGameFramework
{
private:
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	int	m_nWndClientWidth  = FRAME_BUFFER_WIDTH;
	int	m_nWndClientHeight = FRAME_BUFFER_HEIGHT;

	IDXGIFactory4* m_pdxgiFactory;										// DXGI ���丮 ������

	IDXGISwapChain3 *m_pdxgiSwapChain;									// ����ü�� ������. ���÷��� ����

	ID3D12Device *m_pd3dDevice;											// D3D ����̽� ������. ���ҽ� ����

	bool m_bMsaa4xEnable = false;										// MSAA ���� ���ø� Ȱ��ȭ �÷���
	UINT m_nMsaa4xQualityLevels = 0;									// MSAA ���� ���ø� ����

	static const UINT m_nSwapChainBuffers = 2;							// ����ü�� �ĸ� ���� ����

	UINT m_nSwapChainBufferIndex;										// ���� ����ü�� �ĸ� ���� �ε���

	ID3D12Resource* m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12Resource *m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// ���� Ÿ�� ����
	ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap;						// ������ �� �������̽� ������
	UINT m_nRtvDescriptorIncrementSize;									// ���� Ÿ�� ������ ���� ũ��

	ID3D12Resource *m_pd3dDepthStencilBuffer;							// ����-���ٽ� ����
	ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap;						// ������ �� �������̽� ������
	UINT m_nDsvDescriptorIncrementSize;									// ����-���ٽ� ������ ���� ũ��

	ID3D12CommandQueue *m_pd3dCommandQueue;								// Ŀ�ǵ� ť ������
	ID3D12CommandAllocator* m_pd3dCommandAllocator;						// Ŀ�ǵ� �Ҵ��� ������
	ID3D12GraphicsCommandList* m_pd3dCommandList;						// Ŀ�ǵ� ����Ʈ �������̽� ������

	ID3D12PipelineState *m_pd3dPipelineState;							// �׷��Ƚ� ���������� ���� ��ü ������

	ID3D12Fence* m_pd3dFence;											// �潺 ������
	UINT64 m_nFenceValue;
	UINT64 m_nFenceValues[m_nSwapChainBuffers];							// �潺 ��
	HANDLE m_hFenceEvent;												// �潺 �̺�Ʈ �ڵ�

	CGameTimer m_GameTimer;								
	_TCHAR m_pszFrameRate[50];											// ������ ����Ʈ�� �������� ĸ�ǿ� ����ϱ� ���� ���ڿ�

	CScene* m_pScene;

public:
	CCamera* m_pCamera = NULL;

	//�÷��̾� ��ü�� ���� �������̴�.
	CPlayer* m_pPlayer = NULL;
	//���������� ���콺 ��ư�� Ŭ���� ���� ���콺 Ŀ���� ��ġ�̴�. 
	POINT m_ptOldCursorPos; 
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);					//�����ӿ�ũ�� �ʱ�ȭ�ϴ� �Լ�. �� �����찡 �����Ǹ� ȣ��
	void OnDestroy();

	void CreateSwapChain();												// ���� ü�� ���� �Լ�.
	void CreateRtvAndDsvDescriptorHeaps();								// ������ �� ���� �Լ�.
	void CreateDirect3DDevice();										// ����̽� ���� �Լ�.
	void CreateCommandQueueAndList();									// ��� ť,�Ҵ���,����Ʈ ���� �Լ�.

	void CreateRenderTargetViews();										//���� Ÿ�� �� ���� �Լ�.
	void CreateDepthStencilView();										//����-���ٽ� �� ���� �Լ�.

	void BuildObjects();												// �������� �޽��� ���� ��ü ����
	void ReleaseObjects();												// �������� �޽��� ���� ��ü �Ҹ�

	void ProcessInput();												// ��ǲ ó�� �Լ�
	void AnimateObjects();												// �ִϸ��̼� ó�� �Լ�
	void FrameAdvance();												// ������ �Լ�
	void MoveToNextFrame();

	void WaitForGpuComplete();											// CPU-GPU ����ȭ �Լ�

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void ChangeSwapChainState();
};