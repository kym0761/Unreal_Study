# Unreal_Study/UnrealC++/UnrealObject


### Unreal C++이란?

C++은 빠른 대신에 생산성 안정성이 낮다
C#과 Java는 생산성과 안정성을 얻은 대신에 가상머신상에서 돌아가기 때문에 속도를 희생했다.

후발 언어에서는 SOLID 원칙을 따르고,
인터페이스, 리플렉션, 델리게이트 등의 기능을 가지고 있다.

UnrealC++은 모던 언어가 가진 기능을 C++에 도입해서 일반 C++과는 다른 언어가 되었다.


C++ 오브젝트는 F, 언리얼 오브젝트는 U, A 등의 접두사를 받는다.


### Unreal C++ 기본

언리얼 C++ 클래스를 생헝하면 generated.h가 붙는다. 이는 건드리지 말아야함.

GENERATED_BODY()가 있어야 언리얼 오브젝트로 변환됨.

class ??_API UMyObject

이 API는 이 클래스가 모듈에 있다는 것을 명시한 것이므로, 모듈을 정하지 않으면 직접 찾아 넣어야함.

### 리플렉션

프로퍼티 시스템이라고도 부름.

프로그램이 실행시간에 자신을 조사하는 기능이다.

언리얼 오브젝트는 언리얼에서 제공하는 리플렉션을 사용할 수 있다(일반 C++ 클래스는 안됨.)

.generated.h 가 이 리플렉션을 제공하기 위한 코드다

UFUNCTION() UPROPERTY() UCLASS() UENUM() 등이 이를 위한 매크로다.

언리얼 오브젝트를 포인터로 사용할 때, UPROPERTY()를 쓰지 않으면 가비지 컬렉션으로 레퍼런싱이 되지 않으므로, 그냥 UPROPERTY를 쓰면 된다 생각하면 됨.

StaticClass(), GetClass() 등이 리플렉션의 예시다.

### Unreal Object


생성시 
C++은 new를 썻지만 언리얼 오브젝트는 NewObject()를 사용하도록 해야함.

### CDO

CDO : class default object

언리얼 객체의 기본 값을 보관하는 템플릿 객체임

엔진 초기화 과정에서 생성됨.

후에 데이터 값을 변경한 클래스를 사용하더라도, 기본 값은 언리얼 C++ 생성자에서 초기화한 데이터는 CDO를 부를 때마다 접근할 수 있음.

만약 헤더와 생성자를 변경하는 작업을 했다면, 에디터를 끈 뒤 다시 실행하는 것을 추천함.

또한, 헤더를 고쳤을 때 라이브 코딩으로 컴파일하면 무조건 크래시가 남.

cpp만 고쳤을 때만 라이브 코딩을 하고, 그외에는 에디터를 꺼야함.

### 리플렉션 활용 예시

```
//UMyObject라는 클래스에 MyClassName이라는 UPROPERTY()가 있다면 이렇게 접근할 수 있음.

UMyObject* myObj = NewObject<UMyObject>();
FProperty* prop = UMyObject::StaticClass()->FindPropertyByName(TEXT("MyClassName"));
if(prop)
{
	FString currentName;
	prop->GetValue_InContainer(myObj,&currentName); // 리플렉션으로 값을 얻음

	FString newName = TEXT("abc");
	prop->SetValue_InContainer(myObj, &newName); // 리플렉션으로 값을 변경함.
}


UFunction* func = myObj->GetClass()->FundFunctionByName(TEXT("MyFunc"));
if(func)
{
	myObj->ProcessEvent(func,nullptr); // 리플렉션으로 이용한 함수 호출.
}

```

