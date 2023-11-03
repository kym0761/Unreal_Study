# Unreal_Study/Unreal Mobile

## 설명

언리얼 엔진에서 모바일 빌드를 하는 방법

참고 : https://docs.unrealengine.com/5.3/en-US/how-to-set-up-android-sdk-and-ndk-for-your-unreal-engine-development-environment/

## 방법

기준은 UE5.3과 갤럭시 Note8


### Android Studio를 설치한다.

안드로이드 스튜디오를 Google에서 검색해서 설치한다.

참고 문서에는 특정 버전의 안드로이드 스튜디오를 사용하라는 듯이 써있지만, 안드로이드 스튜디오 자체가 중요한 것이 아니라 SDK NDK 등이 중요한 것이라 최신 버전으로 설치하면 된다.

안드로이드 스튜디오 기본 설정(standard)로 설치하고 실행하고, More Actions-> SDK Manager를 실행한다.

### SDK 설정

먼저 SDK 설정이다.

처음 Android SDK를 들어가보면 SDK Platforms가 나온다.

빌드할 때 사용할 안드로이드 버전의 SDK 플랫폼을 선택한다.

노트8의 경우 안드로이드 버전 9.0(pie)이므로 해당 SDK 플랫폼을 선택해준다. 필요한 SDK 플랫폼은 알아서 선택한다.

SDK Tools는 Show Package Details를 체크해준다.

Android SDK Build-tools 34 -> 다른 사람들은 이거를 많이 체크했지만 난 기본으로 있는 34.0.0 33.0.1만 사용하고 문제가 전혀 없었다.

NDK -> 언리얼에서 25.1.893... 를 사용하라고 했다.(r25b == 25.1.893...)

Android SDK Command-Line Tools는 최신버전(latest)만 체크해도 문제 없었다.

CMake -> 3.22.1, 3.10.2.4988... 두개만 해도 문제 없었다. 참고 링크에도 이 2개만 존재하여 두개만 추가했다.

적절히 체크했으면 Apply해서 다운로드 받고 OK해주면 된다.

## 환경 변수

내 컴퓨터에 환경 변수를 넣어줘야한다.

JAVA_HOME
ANDROID_HOME
NDK_ROOT
NDKROOT

4가지를 추가해준다. 참고 문서에 경로가 써져있다.

특이한 점은, 튜토리얼에서는 JDK 다운받아서 JDK 경로를 설정해주지만, 언리얼 5.3부터는 안드로이드 스튜디오의 jbr 폴더로 설정했다.

## SetupAndroid 스크립트

위의 설정을 다 끝냈다면 SetupAndroid 스크립트를 사용해 적절한 NDK를 다운받을 수 있다고 한다.

언리얼 버전/ engine/Extras/Android 폴더를 들어가면 SetupAndroid.bat이 존재한다. 이를 실행하면 NDK를 다운받는다.

근데 해보면 이거 25.1.893을 설치하는 스크립트다. 사실 필요 없을지도?

## Unreal SDK Paths

위의 절차를 전부 문제없이 해냈다면, UE_5.3은 알아서 SDK NDK를 잡기 때문에 더 할 필요는 없다.

만약 옛날 버전 언리얼을 사용한다면, 프로젝트 세팅 -> Android SDK에 들어가서 올바른 SDK 경로를 수동으로 설정해야한다.

## 모바일 프로젝트

테스트를 위해, 3인칭 프로젝트를 타겟을 "모바일" "스케일 가능" 등의 설정을 해서 모바일에 적절한 프로젝트를 만든다.

오른쪽 위에 Settings에 Preview Platform에서 Android OpenGL, Android Vulkan을 선택해서 프리뷰 세팅을 해준다.

프리뷰 세팅을 완료했으면 PC판 프리뷰와 안드로이드 프리뷰를 선택할 수 있다.(플레이 버튼 왼쪽에 안드로이드 아이콘 생김)

## 안드로이드 폰 설정

개발자 모드 들어가서 USB 디버깅을 활성화한다. 요즘 폰마다 설정의 UI가 많이 달라서 구글에 검색해서 개발자 모드 활성화를 찾는 것이 빠를 수도 있다.

USB로 컴퓨터랑 연결하고, CMD로 adb devices를 입력하여 제대로 연결됐는지 확인한다.

device라고 나오면 문제없고, unauthorized라고 되어 있으면 문제가 있는 것이다.

unauthorized는 USB 디버깅을 컴퓨터에 항상허용이 되어 있지 않은 것이다. 컴퓨터 RSA key fingerprint ~~~~가 나오면 Always allow from this computer. 체크한 상태서 핸드폰에서 컴퓨터와 디버깅 연결 보장해주면 된다.

이는 핸드폰 연결 중에 usb 디버깅 활성화를 해서 그런 것 같다. 디버깅 활성화 한 후에 usb선을 빼서 컴퓨터랑 다시 연결하면 해당 팝업이 핸드폰에 나올 것이다.

## 모바일 실행

실행할 때, 연결된 모바일 디바이스로 Quick Launch하면 된다.

## 모바일 빌드

Platforms의 Android 세팅을 적절히 한다.

그리고 프로젝트 세팅 -> Android 가보면 APK Packaging과 Google Play Services 메뉴에 빨간 경고가 있다. 버튼을 눌러서  OK 상태로 만들어 준다.

그 후에 안드로이드로 Package Project를 실행하여 앱으로 만든다.

그러면 여러 파일이 생길텐데, 2가지 파일이 중요하다.

```
.apk
.obb
```

이 두 파일을 핸드폰으로 옮긴다.

apk파일은 핸드폰에서 실행해서 앱을 설치하면 된다.

.obb는 핸드폰 폴더의 Android->obb->"빌드한 프로젝트에 맞는 이름 폴더" 로 들어가서 obb 파일을 옮겨주면 된다.

이 절차를 마치면, 설치한 앱을 실행하면 정상적으로 실행되는 것을 볼 수 있다.

## IOS?

IOS로도 해보고 싶지만, Mac 컴퓨터가 없는 관계로 못한다. 언젠가 할 수 있을지는 아직 모른다.