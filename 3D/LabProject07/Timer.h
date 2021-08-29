#pragma once

const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	void Start() { }
	void Stop() { }
	void Reset();
	void Tick(float fLockFPS = 0.0f);		// Ÿ�̸� �ð� ����

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);		
	float GetTimeElapsed();

private:
	bool m_bHardwareHasPerformanceCounter;		// Performance Counter �÷���
	float m_fTimeScale;							
	float m_fTimeElapsed;
	__int64 m_nCurrentTime;
	__int64 m_nLastTime;
	__int64 m_nPerformanceFrequency;			// Performance Frequency 
	
	float m_fFrameTime[MAX_SAMPLE_COUNT];		// ������ �ð� ����
	ULONG m_nSampleCount;						// ������ ������ Ƚ��
	
	unsigned long m_nCurrentFrameRate;
	unsigned long m_nFramesPerSecond;
	float m_fFPSTimeElapsed;
	
	bool m_bStopped;
};

