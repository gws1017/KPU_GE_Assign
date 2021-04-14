FMOD.dll 을 윈플기말.exe 파일이 있는곳에 넣어줘야합니다 (알집에 있는건 이미 넣어준 상태임)
/ 게임 실행 파일에는 상관없지만 visual studio로 돌릴 땐
https://fmod.com/download 가서 FMOD Engine 2.00 버전의 Windows 버전설치후
프로젝트 속성 
VC++디렉토리 -> 포함디렉터리 -> C:\Program Files (x86)\FMODSoundSystem\FMOD Studio API Windows\api\core\inc
VC++디렉토리 -> 라이브러리 디렉터리 -> C:\Program Files (x86)\FMODSoundSystem\FMOD Studio API WIndows\api\core\lib\x86
링커->입력->추가종속성->fmod_vc.lib 추가
3가지를 해줘야 합니다.
솔루션 구성 Debug에 솔루션 플랫폼을 x86으로 해야 실행이 됩니다.