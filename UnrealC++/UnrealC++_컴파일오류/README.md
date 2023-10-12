# Unreal_Study/UnrealC++/UnrealC++_컴파일오류
 
## 설명

자주 하는 실수로 인해 나오는 컴파일 오류 예시 및 해결법을 서술함.


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

이런 식으로 헤더&.cpp가 생성되면 무조건 오류가 난다.

해결 방법은 여러가지가 있다.

```
#include "BaseActor.h"
```
1. 같은 폴더면 .cpp의 include의 하위 폴더 경로를 지운다.

```
#include "../CoreSystem/BaseActor.h"
```
2. ../를 사용한다. (개인적으로는 매우 비추천)


```
       PrivateIncludePaths.AddRange(new string[] { "{MyProjectName}" });
```

3. Build.cs 파일에 위 내용처럼 프로젝트 경로를 연결해준다.

### 구조체를 TMap, TSet에서 사용할 때 오류

개인 사용자가 만든 구조체는 TMap, TSet에 사용하기 위해서는 == 연산자 오버로딩, GetTypeHash() 구현이 필요하다.

```
USTRUCT(BlueprintType, Blueprintable)
struct FGrid
{
	GENERATED_BODY()

public:

	FGrid();

	//이거 구현 필요.
	bool operator==(const FGrid& Other) const;
};

//이거 구현 필요. 2
uint32 GetTypeHash(const FGrid& Grid)
{
	return FCrc::MemCrc32(&Grid, sizeof(Grid));
}

```
근데, 이 구조체로 TMap, TSet에 사용한 내역이 프로젝트에 존재하지 않으면 오류가 난다. //이유는 모른다. 다른 컴퓨터에선 문제가 없을지도 모르지만 확인 못해봄.  
그러니 만약 이 둘을 구현해놨다면 꼭 사용해야한다.

### Super::

언리얼 객체의 virtual 함수는 대부분 Super::가 있을 것이다.(모든 virtual함수에 Super::가 있는지는 모름)
예시) BeginPlay()

이 함수를 뜯어서 확인해보면 엔진 코어단 기능을 포함하고 있기 때문에, 클래스에 이런 함수를 구현할 때 Super::를 사용하지 않으면 엔진 에러가 난다.

그러니 꼭 Super::가 있는지 확인하고 안에 꼭 넣어주어야한다.