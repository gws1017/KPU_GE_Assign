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

	IDXGIFactory4* m_pdxgiFactory;										// DXGI 팩토리 포인터

	IDXGISwapChain3 *m_pdxgiSwapChain;									// 스왑체인 포인터. 디스플레이 제어

	ID3D12Device *m_pd3dDevice;											// D3D 디바이스 포인터. 리소스 생성

	bool m_bMsaa4xEnable = false;										// MSAA 다중 샘플링 활성화 플래그
	UINT m_nMsaa4xQualityLevels = 0;									// MSAA 다중 샘플링 레벨

	static const UINT m_nSwapChainBuffers = 2;							// 스왑체인 후면 버퍼 개수

	UINT m_nSwapChainBufferIndex;										// 현재 스왑체인 후면 버퍼 인덱스

	ID3D12Resource* m_ppd3dSwapChainBackBuffers[m_nSwapChainBuffers];
	ID3D12Resource *m_ppd3dRenderTargetBuffers[m_nSwapChainBuffers];	// 렌더 타겟 버퍼
	ID3D12DescriptorHeap* m_pd3dRtvDescriptorHeap;						// 서술자 힙 인터페이스 포인터
	UINT m_nRtvDescriptorIncrementSize;									// 렌더 타겟 서술자 원소 크기

	ID3D12Resource *m_pd3dDepthStencilBuffer;							// 깊이-스텐실 버퍼
	ID3D12DescriptorHeap* m_pd3dDsvDescriptorHeap;						// 서술자 힙 인터페이스 포인터
	UINT m_nDsvDescriptorIncrementSize;									// 깊이-스텐실 서술자 원소 크기

	ID3D12CommandQueue *m_pd3dCommandQueue;								// 커맨드 큐 포인터
	ID3D12CommandAllocator* m_pd3dCommandAllocator;						// 커맨드 할당자 포인터
	ID3D12GraphicsCommandList* m_pd3dCommandList;						// 커맨드 리스트 인터페이스 포인터

	ID3D12PipelineState *m_pd3dPipelineState;							// 그래픽스 파이프라인 상태 객체 포인터

	ID3D12Fence* m_pd3dFence;											// 펜스 포인터
	UINT64 m_nFenceValue;
	UINT64 m_nFenceValues[m_nSwapChainBuffers];							// 펜스 값
	HANDLE m_hFenceEvent;												// 펜스 이벤트 핸들

	CGameTimer m_GameTimer;								
	_TCHAR m_pszFrameRate[50];											// 프레임 레이트를 윈도우의 캡션에 출력하기 위한 문자열

	CScene* m_pScene;

public:
	CCamera* m_pCamera = NULL;

	//플레이어 객체에 대한 포인터이다.
	CPlayer* m_pPlayer = NULL;
	//마지막으로 마우스 버튼을 클릭할 때의 마우스 커서의 위치이다. 
	POINT m_ptOldCursorPos; 
public:
	CGameFramework();
	~CGameFramework();

	bool OnCreate(HINSTANCE hInstance, HWND hMainWnd);					//프레임워크를 초기화하는 함수. 주 윈도우가 생성되면 호출
	void OnDestroy();

	void CreateSwapChain();												// 스왑 체인 생성 함수.
	void CreateRtvAndDsvDescriptorHeaps();								// 서술자 힙 생성 함수.
	void CreateDirect3DDevice();										// 디바이스 생성 함수.
	void CreateCommandQueueAndList();									// 명령 큐,할당자,리스트 생성 함수.

	void CreateRenderTargetViews();										//렌더 타겟 뷰 생성 함수.
	void CreateDepthStencilView();										//깊이-스텐실 뷰 생성 함수.

	void BuildObjects();												// 렌더링할 메쉬와 게임 객체 생성
	void ReleaseObjects();												// 렌더링한 메쉬와 게임 객체 소멸

	void ProcessInput();												// 인풋 처리 함수
	void AnimateObjects();												// 애니메이션 처리 함수
	void FrameAdvance();												// 랜더링 함수
	void MoveToNextFrame();

	void WaitForGpuComplete();											// CPU-GPU 동기화 함수

	void OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	void OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	
	void ChangeSwapChainState();
};