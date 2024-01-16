# Unreal_Study/Unreal Insights

## 설명

언리얼 통계 분석툴

### 사용법

1. 언리얼 엔진 설치 경로를 찾는다

```
보통은 C드라이브\Program\Epic Games\UE_XX\Engine\Binaries\Win64에 있다

UnrealInsights.exe

이를 바탕화면에 바로가기를 만들던지, 여러 방법으로 접근 경로를 만든다.
```

2. 환경 변수 추가

```
Path에 C드라이브\Program\Epic Games\UE_XX\Engine\Binaries\Win64 추가
```

3. 프로젝트 안에 bat 파일 생성

```
UnrealEditor.exe %cd%\프로젝트이름.uproject -NetTrace=1 -trace=Net
```

4. bat 실행하여 프로젝트 실행하고, UnrealInsights를 따로 실행