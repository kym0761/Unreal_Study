# Unreal_Study/UnrealC++
 
### enum 만들기 예시

```
UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None UMETA(DisplayName = "None"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff")

};
```

### struct 만들기 예시

```
USTRUCT(BlueprintType)
struct FAbilityDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		UTexture2D* Thumbnail;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		EAbilityDurabilityType AbilityDurabilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float Duration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float Cooldown;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		EAbilityType AbilityType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TArray<float> AbilityEffectAmounts;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		TArray<float> AbilityCoefficientAmounts;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability")
		float CastingTime;

	FAbilityDataStruct();

};

```

FTableRowBase는 언리얼 데이터 테이블로 사용하기 위해서 상속받아야함.


### UMG C++로 만들 때

```
UCLASS(abstract)
class PROJECTLIFE_API UConditionIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (BindWidget))
		UImage* ConditionImage;
}
```
UPROPERTY 안에 Meta를 BindWidget을 선언하면,
언리얼 에디터에서 같은 이름의 에셋에 자동으로 바인드 된다.


### 인터페이스 만들기
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
		void Interact(APawn* InteractCauser);

};
```

UInterface를 상속한 UInteractive를 만들고
class IInteractive라는 클래스를 따로 선언하면된다.
```
UCLASS()
class PROJECTLIFE_API ABasicInteractiveActor : public AActor, public IInteractive
{
	GENERATED_BODY()


	virtual void Interact_Implementation(APawn* InteractCauser);
};
```
위처럼 IInteractive를 상속받은 뒤에 해당 인터페이스의 함수를 구현해줘야함.


인터페이스를 사용하다면...
if (IsValid(interactee) && interactee->GetClass()->ImplementsInterface(UInteractive::StaticClass()))
{
IInteractive::Execute_Interact(interactee, this);
}

지금처럼, ImplementsInterface()를 사용하여 인터페이스가 적용되어 있는지 확인하고
인터페이스의 Execute_();를 사용한다.

### 카메라 기준 방향으로 움직이기
```
void ABasicCharacter::MoveRight(float Value)
{
	FVector cameraLocation;
	FRotator cameraRotation;
	GetController()->GetPlayerViewPoint(cameraLocation, cameraRotation);

	FRotator controlRot = FRotator(0.0f, cameraRotation.Yaw, 0.0f);

	FVector temp = UKismetMathLibrary::GetRightVector(controlRot);

	AddMovementInput(temp, Value);
}
```
GetController()->GetPlayerViewPoint()를 사용한다.
UKistmetMathLibrary를 사용해 RightVector ForwardVector 등을 계산함.

### Deferred Spawn

```
ADamageTextActor* damageTextActor = GetWorld()->SpawnActorDeferred<ADamageTextActor>(DamageTextActorClass, GetActorTransform());
if (damageTextActor)
{
	damageTextActor->AppliedDamage = 0.0f;
	damageTextActor->FinishSpawning(GetActorTransform());
}
```
SpawnActorDeferred를 사용
초기화가 끝난다면, FinishSpawning을 해야함.

### lambda 사용하기

```
	hasInteractives.Sort([this](const AActor& a, const AActor& b)
		{
			FVector compLocation = this->GetComponentLocation();

			float distA = FVector::Distance(compLocation, a.GetActorLocation());
			float distB = FVector::Distance(compLocation, b.GetActorLocation());

			return distA < distB;
		});
```
[]는 캡쳐다. this는 this가 캡쳐된다.
& 모든 변수를 Ref로 캡쳐
= 모든 변수를 복사하여 캡쳐
&x, &y, z  : x , y는 Ref 캡쳐, z는 Ref 캡쳐 


원래는
[] () -> returnType {}
이다.

TFunctionRef<>사용하면 Parameter로 람다 함수를 넣을 수 있다.
TFunctionRef<returnType(const param1, const param2, ...)> LambdaParam;

### 언리얼은 template를 사용할 수 있는가?

사용은 가능하다.

```
emplate<typename T>
class TURNBASESTRATEGY_API FGridSystem
{

private:

	int32 X_Length;
	int32 Y_Length;
	float CellSize;

public:
	TArray<T*> ObjectArray;

public:

	FGridSystem();
	~FGridSystem();
	FGridSystem(int _X_Length, int _Y_Length, float _CellSize);

	void SetGridSystem(int _X_Length, int _Y_Length, float _CellSize, TSharedPtr<FGridSystem<T>> SharedPtr,
		TFunctionRef<T* (TSharedPtr<FGridSystem<T>>, FGrid)> CreateObjectFunction);
...
};
```

제한이 있다.
.h에 구현까지 해야한다.
UObject를 담을 수 없다.

UObject는 자신을 가리키는 포인터가 감지되지 않으면 언리얼엔진에서 가비지 컬렉팅을 한다.
근데, template는 UCLASS()로 만들 수가 없으므로, UObject*로 묶어놔도 언리얼 엔진에서 인식을 못한다.
위의 예시대로 TArray<UObject*>로 가비지 컬렉션되지 않게 방지하려는 의도로 사용한다고 해도
결국 가비지 컬렉팅이 되기 때문에, UObject를 섞어서 사용할 수는 없다

template를 사용하려면 순수 C++로만 구성하고, 언리얼 오브젝트는 포함하지 않아야한다.
아래에 있는 Smart Pointer를 사용하던지, 아니면 걍 UObject로 다 만들던지...

### Smart Pointer

스마트 포인터는 C++의 포인터의 가비지 컬렉션 되지 않아 메모리 누수를 막기 위한 포인터 객체다.

Unreal도 있다
~~~
TUniquePtr;
TSharedPtr;
TWeakPtr;
~~~

보통은 TSharedPtr, TWeakPtr을 사용하게 될 것이다.

원래 UObject는 위의 스마트포인터를 묶을 수가 없고, 묶을 이유가 없긴 했는데
에픽 게임즈에서 TObjectPtr을 만들었다.

실질적으로 UObject*를 TObjectPtr<UObject>로 고쳐서 로직이 돌아가고 있기 때문에, 둘중 아무거나 써도 상관없는 것 같다.
마단 언제든 UObject에 대한 순수 포인터를 막을지도 모르니 이후에 TObjectPtr<T>로 고칠 필요가 있음.
UFUNCTION은 5.1 기준으로는 아직 TObjectPtr을 매개변수로 받을 수 없다.

~~~
// @TODO: OBJPTR: Investigate TObjectPtr support for UFunction parameters.

// @TODO: OBJPTR: we want to permit lazy background loading in the future
// else if (*LoadBehaviorMeta == "LazyBackground")
// {
// 	LoadBehavior = EImportLoadBehavior::LazyBackground;
// }

// @TODO: OBJPTR: Need to find other options for solving this issue of placeholder classes during blueprint compile without forcing all imports to resolve always

// @TODO: OBJPTR: We should have a test that ensures that lazy loading of an object with an external package is handled correctly.
~~~

TODO 목록을 보면 순수 포인터를 대체하기 위한 장기적 프로젝트로 보이니 되도록 고치는 것을 추천.


### Object Trace

```
APlayerController* playerController= UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (!IsValid(playerController))
	{
		return;
	}

	FVector loc;
	FVector rot;
	playerController->DeprojectMousePositionToWorld(loc, rot);

	TArray<TEnumAsByte<EObjectTypeQuery>> objects;
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	objects.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));


	TArray<AActor*> ignores;
	ignores.Add(this);

	FHitResult hit;

	bool result = UKismetSystemLibrary::LineTraceSingleForObjects(
		GetWorld(),
		loc,
		loc + rot * 10000,
		objects,
		true,
		ignores,
		//EDrawDebugTrace::None,
		EDrawDebugTrace::ForDuration,
		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Blue,
		5.0f
	);
```

위의 예시는 화면 마우스 위치의 Actor를 선택하는 방법에 대한 예시다.
