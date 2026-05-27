# OOP Client

OOP 멀티플레이어 게임의 Windows 콘솔 클라이언트입니다.

멀티플레이어 클라이언트는 아래 클라우드 TCP 서버에 접속합니다.

```text
169.213.25.19:9000
```

## 빌드 방법

Visual Studio 2022 또는 Visual Studio Build Tools를 설치하고,
C++ 빌드 도구가 포함되어 있는지 확인합니다.

그다음 아래 배치 파일을 실행합니다.

```bat
build_game.bat
```

빌드가 완료되면 다음 실행 파일이 생성됩니다.

```text
Client.exe
```

## 실행 방법

```bat
Client.exe
```

실행 후 멀티플레이어 메뉴에서 온라인 방을 만들거나 기존 방에 참가할 수 있습니다.

## 서버 주소 설정

서버 주소는 아래 파일에서 설정합니다.

```text
game/NetworkConfig.h
```
