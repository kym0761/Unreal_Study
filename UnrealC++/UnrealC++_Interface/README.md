# Unreal_Study/UnrealC++/UnrealC++Interface


언리얼의 인터페이스는

UInterface
IInterface

두가지의 클래스가 생성된다.

U는 클래스 정보를 런타임에서 구현되었는지 확인하는 용도
I는 실질적인 클래스로 구성 및 구현에 사용됨
U는 작업에서는 쓰이지 않을 것임.

언리얼 C++의 인터페이스는 추상 타입이 아니라서, 인터페이스 안에 기본적인 구현이 가능함.

```
 //Interactive Interface Class Base. Don't Use.
UINTERFACE(Blueprintable)
class UInteractive : public UInterface
{
	GENERATED_BODY()
};

//Actual Interactive Interface Class. You must Use This When you Inherit Interface.
class IInteractive
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, Category = "Interactive")
		void Interact(APawn* InteractCauser) = 0;

};
```

인터페이스로 형변환을 했다면, 인터페이스 구현 유무를 파악할 수 있음
```
IInteractive* interactive = Cast<IInteractive>(pawn);
if(interactive)
{
interactive->interact(aaa);
}
```