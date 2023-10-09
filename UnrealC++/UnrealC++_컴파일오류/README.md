# Unreal_Study/UnrealC++/UnrealC++_컴파일오류
 
## 설명

컴파일 오류 나는 예시 및 해결법을 서술함.


### 헤더가 없음

CoreMinimal에 있는 클래스를 제외하면 IWYU 규칙에 의해 컴파일 오류남.

컴포넌트나 라이브러리 사용할 때마다 .cpp에 필요한 헤더를 include해주어야함.

### 모듈을 추가하지 않음

프로젝트의 .Build.cs 파일에 필요한 모듈을 추가하지 않아서 오류가 생긴다.

```
PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

//추가된 모듈
PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "GameplayTasks", "UMG", "NavigationSystem", "Niagara", "EnhancedInput" });

PrivateDependencyModuleNames.AddRange(new string[] {  });

// Uncomment if you are using Slate UI
PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
```

### 폴더를 나누면 생기는 오류

프로젝트 Source 폴더 안에 클래스를 다른 하위 폴더로 나뉘어 관리하면 오류가 남.

폴더 안에 클래스를 만들면 .cpp 파일의 경로가 이상하다는 것을 알 수 있다.

예를 들면

```
#include "CoreSystem/BaseActor.h"
...
...

```

이런 식으로 헤더가 생성되면 무조건 오류가 난다.

해결 방법은 여러가지가 있다.

```
#include "BaseActor.h"
```
1. 같은 폴더면 하위 폴더 경로를 지운다.

```
#include "../CoreSystem/BaseActor.h"
```
2. ../를 사용한다. (개인적으로는 매우 비추천)


```
       PrivateIncludePaths.AddRange(new string[] { "{MyProjectName}" });
```

3. Build.cs 파일에 위 내용처럼 프로젝트 경로를 연결해준다.
