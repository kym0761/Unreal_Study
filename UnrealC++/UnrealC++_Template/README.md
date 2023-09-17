# Unreal_Study/UnrealC++_TemplateClass
 
## 예시

이 폴더 안에 template 클래스가 적용된 프로젝트 예시를 넣었음.

UE_5.1 기준으로 빌드되었음.


### 언리얼에서 template

```
template<typename T>
class TEMPLATETEST_API FTemplateHolder : public FGCObject
{
public:


	FTemplateHolder()
	{
	
	}

	virtual ~FTemplateHolder()
	{
	
	}

	TArray<T*> ObjectArray; //읽을 Object
	TArray<T*> SafeObjectArray; //GC로부터 안전할 Object

	//매 GarbageCollect가 되기 전에 AddReferencedObjects가 동작해 매번 Obj를 등록하는 것으로 보임.
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		for (int32 i = 0; i < SafeObjectArray.Num(); i++)
		{
			if (SafeObjectArray[i] && SafeObjectArray[i]->IsValidLowLevel())
			{
				Collector.AddReferencedObject(SafeObjectArray[i]);
			}
		}
	}

	//이 C++ 클래스는 Referencer가 되어야함을 명시하는 것으로 보임.
	virtual FString GetReferencerName() const override
	{
		return TEXT("FTemplateHolder");
	}

};

```

```
아래와 같은 사용에 제한이 있다.

1. 헤더에 구현까지 해야한다.(cpp에 구현시 오류난다.) 
2. Template 클래스는 UObject가 될 수 없다.

```

```
설명:

template Class는 엄밀히 따지면, Class가 아니라 클래스를 만들기 위한 규격이다.
예시로 
TemplateClass<int32> tc;
이런식으로 객체가 생성되어야 비로소 클래스가 된다. 

그러므로 UCLASS()를 위에 붙여봤자 클래스로 인식되지 않으므로 UObject로 파생된 클래스를 만들 수가 없다.


또한, UCLASS가 아니기 때문에 기본적인 방식으로는 UObject를 참조하는 클래스가 될 수 없어서 언리얼의 가비지 컬렉션으로부터 하위 객체를 지켜주지 않는다.

이 부분을 해결하기 위해서 FGCObject를 상속받은 template 클래스를 만들고

virtual void AddReferencedObjects(FReferenceCollector& Collector) override

virtual FString GetReferencerName() const override

를 위의 예시처럼 구현하면 순수 C++ 클래스에서도 언리얼 가비지 콜렉션으로부터 하위 객체들을 지켜줄 수 있다.

위의 방법은 template class 뿐만 아니라, 다른 순수 C++ 클래스에서도 적용되는 방법임.


또한 이런 순수 C++ 클래스들은 스마트포인터를 사용해야 안전하므로 참고하는 것이 좋다.

```

```

DECLARE_DELEGATE(FMySignature); // void 함수를 받을수 있는 delegate
...
...


	template<typename ObjectClass>
	FORCEINLINE void SomeFunction(ObjectClass* ObjectOwner, FMySignature::TMethodPtr<ObjectClass> Func)
	{
		// ::TMethodPtr<>을 사용하면 (,&ClassName::FunctionName); 처럼 함수 포인터를 파라미터로 받을 수 있음.
	}

...
...

//BeginPlay든 어느 공간에서 해당 함수를 사용할 때의 예시
{
...
...
	projectChar->SomeFunction(this, &AFuncTestActor::TestActorFunc);
...
...
}

```
위의 예시처럼 template Function은 UFUNCTION이 아닌 경우엔 사용할 수 있다.




### Smart Pointer

스마트 포인터는 C++의 포인터의 가비지 컬렉션 되지 않아 메모리 누수를 막기 위한 포인터 객체다.

Unreal도 있다.
```
TUniquePtr;
TSharedPtr;
TWeakPtr;
```

```
보통은 TSharedPtr, TWeakPtr을 사용하게 될 것이다.

원래 UObject는 위의 스마트포인터를 묶을 수가 없고, 묶을 이유가 없긴 했는데 에픽게임즈에서 TObjectPtr을 만들었다.

실질적으로 UObject*를 TObjectPtr<UObject>로 고쳐서 로직이 돌아가고 있기 때문에, 둘중 아무거나 써도 상관없는 것 같다.

다만, 언제든 UObject에 대한 순수 포인터를 막을지도 모르니 이후에 TObjectPtr<T>로 고칠 필요가 있음.

UFUNCTION은 5.2 기준으로는 아직 TObjectPtr을 Parameter로 받을 수 없다. (23.09.17 확인.)
```

```
// @TODO: OBJPTR: Investigate TObjectPtr support for UFunction parameters.

// @TODO: OBJPTR: we want to permit lazy background loading in the future
// else if (*LoadBehaviorMeta == "LazyBackground")
// {
// 	LoadBehavior = EImportLoadBehavior::LazyBackground;
// }

// @TODO: OBJPTR: Need to find other options for solving this issue of placeholder classes during blueprint compile without forcing all imports to resolve always

// @TODO: OBJPTR: We should have a test that ensures that lazy loading of an object with an external package is handled correctly.
```

TODO 목록을 보면 순수 포인터를 대체하기 위한 장기적 프로젝트로 보이니 되도록 고치는 것을 추천.


