# OOP Client

Windows console client for the OOP multiplayer game.

The multiplayer client connects to the cloud TCP server at:

```text
158.180.71.181:9000
```

## Build

Install Visual Studio 2022 or Visual Studio Build Tools with the C++ toolchain,
then run:

```bat
build_game.bat
```

This creates:

```text
Client.exe
```

## Run

```bat
Client.exe
```

Use the multiplayer menus to create or join an online room.

## Server Address

The server address is configured in:

```text
game/NetworkConfig.h
```
