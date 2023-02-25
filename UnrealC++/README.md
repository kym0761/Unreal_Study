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

 Meta = (BindWidget)를 Specifier로 추가한 뒤, UMG 블루프린트에서 해당 변수와 같은 위젯을 세팅한다.
 위의 예시대로면 Image 추가하고 이름을 ConditionImage 지으면 연동된다.
 
 void NativeConstruct()에서도 추가하는 방법이 있는데, 개인적으론 위 방법을 선호한다.
 ```

void USpawnSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ImageBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ImageBorder")));
}

```


### UMG C++로 생성하기.
	h:
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
		TSubclassOf<UActionSelectButtonWidget> ChooseActionButtonWidgetClass;

	cpp:
	UActionSelectButtonWidget* buttonWidget = CreateWidget<UActionSelectButtonWidget>(GetWorld(), ChooseActionButtonWidgetClass);
	buttonWidget->AddToViewport();
	
외형을 블루프린트에서 생성할 것이므로, h에서 외형을 연결해야한다.

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


원래는"[] () -> returnType {}"이다. 예시) [] (const FString& a) -> bool{return true;}

TFunctionRef<>사용하면 Parameter로 람다 함수를 넣을 수 있다.
 예시)TFunctionRef<returnType(const param1, const param2, ...)> LambdaParam;

### 언리얼은 template를 사용할 수 있는가?

사용은 가능하다.

```
template<typename T>
class TURNBASESTRATEGY_API FGridSystem
{

private:

	int32 X_Length;
	int32 Y_Length;
	float CellSize;

public:
	// !! UObject 사용이 불가능하다.
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

```
제한이 있다. 
.h에 구현까지 해야한다.(cpp에 구현시 오류난다.) 
UObject를 담을 수 없다.

UObject는 자신을 가리키는 포인터가 감지되지 않으면 언리얼엔진에서 가비지 컬렉팅을 한다.
근데, template class는 UCLASS()로 만들 수가 없으므로, UObject*로 묶어놔도 언리얼 엔진에서 인식을 못한다.
위의 예시대로 TArray<Object*>로 Object*를 가비지 컬렉션되지 않게 방지하려는 의도로 사용한다고 해도
template class 안에 있는 TArray가 언리얼 가비지 컬렉션에 정상적으로 감지되지 않아
결국 가비지 컬렉팅이 되기 때문에, template 클래스 안에서 UObject를 섞어서 사용할 수는 없다.
```
template를 사용하려면 순수 C++로만 구성하고, 언리얼 오브젝트는 포함하지 않아야한다.
다른 Actor에서 가비지 컬렉션이 되지 않도록 잡아두던가, 아니면 걍 UObject로 다 만들던지...

### Smart Pointer

스마트 포인터는 C++의 포인터의 가비지 컬렉션 되지 않아 메모리 누수를 막기 위한 포인터 객체다.

Unreal도 있다.
~~~
TUniquePtr;
TSharedPtr;
TWeakPtr;
~~~

보통은 TSharedPtr, TWeakPtr을 사용하게 될 것이다.

원래 UObject는 위의 스마트포인터를 묶을 수가 없고, 묶을 이유가 없긴 했는데 에픽 게임즈에서 TObjectPtr을 만들었다.

실질적으로 UObject*를 TObjectPtr<UObject>로 고쳐서 로직이 돌아가고 있기 때문에, 둘중 아무거나 써도 상관없는 것 같다.

다만, 언제든 UObject에 대한 순수 포인터를 막을지도 모르니 이후에 TObjectPtr<T>로 고칠 필요가 있음.

UFUNCTION은 5.1 기준으로는 아직 TObjectPtr을 Parameter로 받을 수 없다.

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

### private 변수 에디터에 Expose하기

```
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability", Meta = (AllowPrivateAccess = "true"))
		float CurrentDurationTime;
```		

### UFUNCTION 함수 override

```
	UFUNCTION(BlueprintNativeEvent)
	void EndAbility();
	virtual void EndAbility_Implementation();
```

Specifier는 BlueprintImplementableEvent나 BlueprintNativeEvent를 넣으면 된다. virtual void [FuncName]_Implementation(); 을 하면 C++에서도 override가 가능하다.

### ActorComponent를 블루프린트에서 추가하기

```
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTLIFE_API UAbilityComponent : public UActorComponent
{...}
```
UCLASS()에 위의 내용을 넣어주면 에디터에서도 추가를 할 수 있다.

### Timer 예시

```
h:
FTimerHandle TraceTimer;

cpp:
void AUnitSelectPawn::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(TraceTimer, this, &AUnitSelectPawn::UnitLook, 0.12f, true); //0.12f는 Interval Time, true는 Loop인지 아닌지를 결정.
}
```

만약 타이머에 사용할 Function이 Parameter를 받는다면 FTimerDelegate::CreateUObject를 이용하면 된다.

```
	//Function for Damage caused by Poison.
	FTimerDelegate funcDelegate = FTimerDelegate::CreateUObject(this, &USoldierStatComponent::PoisonDamage, PoisonCauser, PoisonInstigator);
	GetWorld()->GetTimerManager().SetTimer(PoisonTimer, funcDelegate, 1.0f, true, 0.0f);
```

### Blueprint Library 만들기 예시

UCLASS()
class PROJECTLIFE_API UGameTimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GameTime + GameTime", 
		CompactNodeTitle = "+", KeyWords = "+ Plus", CommutativeAssosiativeBinaryOperator = "true"), 
		Category = "GameTime|Convert")
		static FInGameTime Plus_GameTimeGameTime(FInGameTime InValue1, FInGameTime InValue2);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (GameTime)",
		CompactNodeTitle = "->", BlueprintAutocast),
		Category = "GameTime|Convert")
		static FString Conv_GameTimeToString(FInGameTime InGameTime);

	UFUNCTION(BlueprintPure, Category = "GameTime|Get")
		static int32 GetYear_GameTime(FInGameTime InGameTime);
	...
};

### operator overloading

```
	FInGameTime operator+(const FInGameTime& rValue);
	FInGameTime& operator+=(const FInGameTime& rValue);

	FInGameTime operator-(const FInGameTime& rValue);
	FInGameTime& operator-=(const FInGameTime& rValue);
```

parameter로 const와 &를 붙이는 것이 중요하다.
이유 :
void SomeFunc(FSomeStruct& a); 가 있다 가정하고

someFunc(FSomeStruct());는 사용이 불가능하다.
이는 FSomeStruct는 RValue라서 그렇다. RValue는 const라서 값을 변경하는데에 문제가 생긴다.
FSomeStruct a로 Parameter를 고치면 사용은 가능하지만 복사 Parameter라서 구조체의 크기가 큰 경우에 비효율적이다.

위의 operator의 경우에는
FInGameTime a, b;
...
a+=b;
이렇게 사용할 수도 있지만,

FInGameTime a = 0;
a+= FInGameTime(1,1...);
이런 식으로도 사용할 수 있어야 한다.
그러므로 const와 &가 필요하다.

사실 RValue의 경우, void SomeFunc(FSomeStruct&& a);
를 overloading할 수도 있긴 한데, 기억상 operator의 경우에는 언리얼에서 모호성 컴파일 오류가 났다.
	
만약, 들어오는 input이 함수 내에서 변경될 일이 아예 없으며 위에처럼 변수생성을 함수 파라미터에 직접 선언하면서 쓸 경우엔 const와 &를 꼭 쓰는 것이 좋다.
	

```
FInGameTime& FInGameTime::operator-=(const FInGameTime& rValue)
{
	*this = *this - rValue;

	return *this;
}
```

return이 &이면 위처럼 만든다.

### 맞은 부위에 따라 다른 데미지를 주기 만들기

```

void USoldierStatComponent::AddDamage(float Damage, const FHitResult& Hit)
{
	FName hitBoneName = Hit.BoneName;

	//Will Multiply for real Damage
	float damageVariation = 1.0f;

	//Available Bone Name is in the Skeletal's Physics Asset.
	if (hitBoneName == "head")
	{
		damageVariation = 2.0f;
	}
	else if (hitBoneName == "pelvis" || hitBoneName == "spine_01" || hitBoneName == "spine_02" || hitBoneName == "neck_01")
	{
		damageVariation = 1.0f;
	}
	else if (hitBoneName == "upperarm_r" || hitBoneName == "lowerarm_r" || hitBoneName == "hand_r")
	{
		damageVariation = 0.75f;
	}
		....
		....

	HP = FMath::Clamp<float>(HP - Damage * damageVariation, 0.0f, MaxHP);

	if (HP <= 0.0f) // when hp is 0. then Character will die.
	{
		bIsDied = true;
	}

}

```
### DataTable에 접근에 데이터 받아오기

```
			//Find Data from RowName;
			FSoldierRankTable* rowTable = soldierRankDataTable->FindRow<FSoldierRankTable>(rowName, TEXT(""));
			if (rowTable)
			{
				//UE_LOG(LogTemp, Log, TEXT("%s --- %d"), *rowTable->Rank, rowTable->Exp);
				RankName = rowTable->Rank;
				ExpToNeed = rowTable->Exp;
			}
```

### C++에서 Timeline 사용하기

```

void APuzzleDoor::InitializeForTimeline()
{
	if (CurveForTimeline)
	{
		FOnTimelineFloat timelineFloat;
		timelineFloat.BindDynamic(this, &APuzzleDoor::TimelineFloatFunction); //중요

		TimelineHandle.AddInterpFloat(CurveForTimeline, timelineFloat);
		TimelineHandle.SetLooping(false);
	}
}

void APuzzleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timeline Need this.
	TimelineHandle.TickTimeline(DeltaTime);

	//If there is CountDown.. Use This Function.
	if (bDoorOpen && bUseTimer)
	{
		CurrentTime = FMath::Clamp<float>(CurrentTime - DeltaTime, 0.0f, TriggerWaitTime);
		
		if (CurrentTime <= 0.0f && bBoxOverlapping == false)
		{
			CloseDoor();
		}
	}

}

void APuzzleDoor::TimelineFloatFunction(float Value)
{
	FVector doorMeshLocation = FVector(0.0f,0.0f,Value);
	Mesh->SetRelativeLocation(doorMeshLocation);
}

```

### 현재 overlapping 중인 Actor들을 찾기

```
TArray<AActor*> overlapActors;
FireOverlap->GetOverlappingActors(overlapActors); // FireOverlap은 UBoxComponent다.
```

### 인벤토리 DragDrop 구현

```
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, 
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
```

NativeOnDragDetected에서는 DragDisplay와 DragDropOperation을 만들어줘야한다.

```
	//Create DragDisplayUI
	UItemSlot* dragDisplay = CreateWidget<UItemSlot>(GetOwningPlayer(), ItemSlotClass);

	dragDisplay->InventoryComponentRef = InventoryComponentRef;
	dragDisplay->InventorySlotNumber = InventorySlotNumber;
	dragDisplay->UpdateItemSlot();

	//Make DragDropEvent And Assign it.
	UDragDropOperation* dragDropOper = NewObject<UDragDropOperation>();
	dragDropOper->Payload = this;
	dragDropOper->DefaultDragVisual = dragDisplay;
	dragDropOper->Pivot = EDragPivot::CenterCenter;
```

NativeOnDrop에서는 UDragDropOperation* InOperation에서  InOperation->Payload를 위의 DragDisplay에 사용된 Widget으로 Casting하여 기능을 만든다.

```
			UItemSlot* droppedItemSlot = Cast<UItemSlot>(InOperation->Payload);

			//drop의 인벤토리 정보를 받아옴.
			int32 drop_InventorySlotNumber = droppedItemSlot->InventorySlotNumber;
			UInventoryComponent* drop_InventoryComponent = droppedItemSlot->InventoryComponentRef;
```

### Source Code Path 등록

만약 Source 하위에 프로젝트 이름을 제외한 폴더를 추가로 만들어놨다면 접근 오류가 생긴다.

```
예시

Source/ProjectA/Base에 BaseCharacter.h와,
Source/ProjectA/Manager에 BaseManager.h를 존재한다고 가정하자

만약 CPP 파일에 include를 한다면
#include "Base/BaseCharacter.h" 라고 입력하면 컴파일 오류가 난다.

마찬가지로 처음에 클래스를 생성하면, cpp 파일의 경로가 존재하지 않는다면서 오류가 난다.
```

이는 .Build.cs에 Source/[YourProjectName]을 '모듈'이라 가정하고 Path를 등록해주어야 오류가 나지 않는다.

PrivateIncludePaths.AddRange(new string[] { "[YourProjectName]" });

```
예시 해결법
       PrivateIncludePaths.AddRange(new string[] { "ProjectA" });

를 .Build.cs에 추가한다.
```

아마 Source 폴더에 다른 폴더를 만들었다면 이와 같은 방식으로 Path를 등록해주면 될 듯 하다.
