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
	void Tick(float fLockFPS = 0.0f);		// 타이머 시간 갱신

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);		
	float GetTimeElapsed();

private:
	bool m_bHardwareHasPerformanceCounter;		// Performance Counter 플래그
	float m_fTimeScale;							
	float m_fTimeElapsed;
	__int64 m_nCurrentTime;
	__int64 m_nLastTime;
	__int64 m_nPerformanceFrequency;			// Performance Frequency 
	
	float m_fFrameTime[MAX_SAMPLE_COUNT];		// 프레임 시간 누적
	ULONG m_nSampleCount;						// 누적된 프레임 횟수
	
	unsigned long m_nCurrentFrameRate;
	unsigned long m_nFramesPerSecond;
	float m_fFPSTimeElapsed;
	
	bool m_bStopped;
};

