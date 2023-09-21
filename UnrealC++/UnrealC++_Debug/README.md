# Unreal_Study/UnrealC++/UnrealC++Debugging

## Assert

코드 검증

### check(), checkf()

내용이 false면 프로젝트를 멈추고 오류 내용을 알려줌.

```
check(actor);
check(b1 && b2);
```

checkf의 경우, 오류에 추가 정보 출력 가능

```
checkf(actor, TEXT("actor is Invalid"));
checkf(b1 , TEXT("checkf example: %f %s"), 1.0f, *actor->GetClass()->GetName());
```

### verify(), verifyf()

check와 비슷함. 다만 DO_CHECK = 1일 때 동작하는 check와 달리, verify는 아닌 상태에도 동작함.
```
verify(b1 && b2);
verifyf(b1, TEXT("verifyf : %f"),200.0f);
```

### checkCode()

중괄호 안에 있는 표현식을 실행하는 Assert.   
DO_CHECK가 0인 상태에선 실행되지 않으므로, 게임 필수 로직에 사용하면 안됨.
```
checkCode( if( Object->HasAnyFlags( RF_PendingKill ) ) { UE_LOG(LogUObjectGlobals, Fatal, TEXT("Object %s is part of root set though has been marked RF_PendingKill!"), *Object->GetFullName() ); } );
```

### checkNoEntry()

절대로 실행될 일 없는 코드의 경로를 표시할 때 사용.

```
switch (MyEnum)
{
    case MyEnumValue:
        break;
    default:
        checkNoEntry();
        break;
}
```

### checkNoReentry(), checkNoRecursion()

함수 재진입을 방지함.
```
void NoReentry()
{
    checkNoReentry();
}

int32 Recurse(int32 A, int32 B)
{
    checkNoRecursion();
    return Recurse(A - 1, B - 1);
}
```
### unimplemented()

오버라이딩이 필요한 함수임을 표시함.
```
virtual void Func01()
{
	// You must override this
	unimplemented();
}
```

### ensure() , ensureMsgf()

실패시 Call스택을 생성하고 출력함.

ensureMsg()는 이유는 모르겠지만 5.2버전에서 사용이 불가능했음.

## Debug Log

### UE_LOG()

UE_LOG('로그 카테고리', '로그 상세 수준', TEXT("??"));

```
로그 카테고리

LogTemp
LogBlueprint
LogClass
LogCollision
...
...
...
(약 90가지임.)

```


```
로그 상세 수준 (아래로 갈수록 중요도 높음)

Log
Warning
Error
Display
Fatal
Verbose
VeryVerbose
```

### AddOnScreenDebugMessage()

화면에 로그 띄워줌, 이 디버그 메시지는 GEngine이 있을 때만 사용가능하므로, 사용 전에 GEngine이 존재하는지 확인을 추천함.
```
if (GEngine)
{
	GEngine->AddOnScreenDebugMessage(1, 3.0f, FColor::Red, TEXT("??"));
}
```

### 커스텀 디버깅 로그

위의 UE_LOG(), AddOnScreenDebugMessage()은 어디에서 호출됐는지 확인이 불가능하다.  
Assert를 사용하면 확인할 수 있지만, Assert는 동작을 멈추는 크리티컬한 상황에 사용되는 부분이라, 굳이 멈추지 않아도 될 디버깅 로그일 때도 위치를 알면 좋을 것 같다.  
아래 코드를 사용하면 디버깅 로그에서 함수명과 몇번째 줄에서 호출됐는지 확인 가능함.

```
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//예시) AActor::BeginPlay()
#define FUNCTION_NAME (TEXT(" / Function Name : ") + FString(__FUNCTION__)+TEXT(" "))

//예시) AActor::Func에서 ::Func이 빠져서 AActor만 나옴.
#define CURRENT_CLASS (TEXT(" / Class Name : ") + FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) + TEXT(" "))

//함수의 라인만 나옴
#define CURRENT_LINE  (TEXT(" / Line : ") + FString::FromInt(__LINE__) + TEXT(" "))

//예시) void __cdecl AActor::BeginPlay(void) 형식으로 출력됨.
#define CURRENT_FUNCSIG (TEXT(" / Func signature : ") +FString(__FUNCSIG__)+ TEXT(" "))
#define DEBUG_TEXT_POSITION (FUNCTION_NAME + CURRENT_LINE)

//param0 꼭 string으로 할 것.   예시) DEBUG_TEXT("abc")
#define DEBUG_TEXT(Param0) (TEXT(Param0) + DEBUG_TEXT_POSITION)

/**
 * 
 */
namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::MakeRandomColor(), int32 InKey = -1)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, 3.0f, Color, Msg);
		}

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

}
```

아래는 사용예시
```
Debug::Print(DEBUG_TEXT("test01"));
```