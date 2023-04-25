# Unreal_Study/UnrealC++/UnrealC++Base


### Super::

언리얼에서 제공하는 Class의 virtual 함수는 override하게 됐을 때 Super::Func()해주어야한다.

예시
```
::BeginPlay()
{
Super::BeginPlay();

}
```

### 코딩 표준

언리얼 코딩 표준 문서를 참고할 것

https://docs.unrealengine.com/5.1/ko/epic-cplusplus-coding-standard-for-unreal-engine/


```
이동 시맨틱

언리얼의 TArray TSet TMap FString은 move가 있음. 명시적 호출은 sdt::move의 언리얼 버전인 MoveTemp를 사용하면 됨

```

```
const 사용 주의점

// const 포인터에서 const 이외 오브젝트 - 포인터로의 재할당은 불가하나, T는 여전히 수정 가능합니다.
T* const Ptr = ...;

// 틀림
T& const Ref = ...; //T&을 const로 사용하지 않는 이유는, 애초에 레퍼런스는 변경이 불가능함.


언리얼 이동 시맨틱의 제한에 의해 const를 함부로 사용하지 말아야함 예시는 다음과 같음

// 나쁜 예 - const 배열 반환 -- 레퍼런스가 아니므로 복사가 일어남.
const TArray<FString> GetSomeArray();

// 좋은 예 - const 배열로의 레퍼런스 반환 -- 레퍼런스라 복사가 일어나지 않음.
const TArray<FString>& GetSomeArray();

// 좋은 예- const 배열로의 포인터 반환 --포인터의 증감연산으로 접근이 가능함
const TArray<FString>* GetSomeArray();

// 나쁜 예 - const 배열로의 const 포인터 반환 -- 포인터에 const를 걸어서 array로 사용이 안됨 
const TArray<FString>* const GetSomeArray();

```

```
nullptr

언리얼은 C타입 NULL을 쓰지말고 nullptr을 사용해야함.

```

```
static 변수

언리얼의 static 변수는 헤더에 정의하면 안됨
외부에 선언 및 정의를 해야함.

// SomeModule.h
extern SOMEMODULE_API const FString GUsefulNamedString;

// SomeModule.cpp
const FString GUsefulNamedString = TEXT("String");

```

### 기본 타입

언리얼의 타입은 명시해야하는 이유

```
파편화된 플랫폼마다 타입의 크기가 다름.

단일 컴퓨터에서 최대 퍼포먼스를 위해 데이터 전달이 명확해야할 필요가 있음.
```

```
int형은 비트 수를 명시해서 써야함 예시)int32, uint8

```

```
bool도 명확한 크기가 정해져 있지 않음

그래서 헤더에는 되도록 bool대신, uint8을 사용하고 bit field 오퍼레이터를 사용하는 것이 좋음

UPROPERTY()
uint8 bSomethingIsRight:1;

cpp 로직에선 bool을 원하는대로 사용해도 별 문제 없음.

```

### String

캐릭터 인코딩 문서 참고

언리얼의 String은 UTF-16을 사용함. 그러니 나중에 String도 UTF-16으로 통일해야함.

언리얼의 char는 TCHAR임 -> 문자열은 FString임. FString은 TCHAR를 포함한 구조인 클래스임. *str을 하면 사실 TCHAR*를 읽는 것임. FString은 내부적으로 TArray<TCHAR>를 가지고 있다고 보면 된다.

문자열은 언제나 TEXT("")를 사용해야함. 이것은 UTF-16을 명시한 String임.

FCString은 C언어의 String인데, 문자열을 자르거나 변환하거나 등에서 사용할 수도 있음. 예시)strstr atoi atof strcpy

```
유용한 함수들
FString::PrintF()
FString::SanitizeFloat()
FString::FromInt()


```

```

TCHAR charArr[] = TEXT("asdf");


FString str = charArr;

UE_LOG(LogTemp, Log, TEXT("%s"), *str);

```

### FName

에셋을 빠르게 찾기 위한 문자열 체계

대소문자 없음
한번 선언되면 변경 불가능
가볍고 빠름
빌드할때 해시값으로 저장됨

FName은 전역 Pool이 존재함

 FName <-> FNamePool(key, Value)

FName에 저장된 값을 pool에서 검색해서 반환하여 빠른 검색을 함.

### FText

다국어 지원을 위한 문자열 관리 체계


