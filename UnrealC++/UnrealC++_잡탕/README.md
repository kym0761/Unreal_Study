# Unreal_Study/UnrealC++/UnrealC++_잡탕
 
## 설명

분류하지 않은 정보나 기억나는대로 막 썼다.

다른 곳에 중복내용이 있을 수도 있고, 여기에 매우 틀린 정보가 있을 수도 있다.

나중에 분류 완료해서 없애도록 할 예정.


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

private: 
	//protected도 가능함.
	//Meta = (AllowPrivateAccess = "true")를 추가하면 된다.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Ability", Meta = (AllowPrivateAccess = "true"))
		float CurrentDurationTime;
```		

### UFUNCTION 함수 override

```
	UFUNCTION(BlueprintNativeEvent)
	void EndAbility();
	virtual void EndAbility_Implementation();
```

Specifier는 BlueprintImplementableEvent나 BlueprintNativeEvent를 넣으면 된다. 기본 함수는 건드리지 말고, void [FuncName]_Implementation();로 C++에서 필요한 Function()을 구현 할 수도 있다. virtual을 붙이면 C++에서도 override가 가능하다.

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
	
	//0.12f는 Interval Time, true는 Loop할지 안할지를 결정.
	GetWorldTimerManager().SetTimer(TraceTimer, this, &AUnitSelectPawn::UnitLook, 0.12f, true); 
}
```

만약 타이머에 사용할 Function이 Parameter를 받는다면 FTimerDelegate::CreateUObject를 이용하면 된다.

```
	//Function for Damage caused by Poison.
	FTimerDelegate funcDelegate = FTimerDelegate::CreateUObject(this, &USoldierStatComponent::PoisonDamage, PoisonCauser, PoisonInstigator);
	GetWorld()->GetTimerManager().SetTimer(PoisonTimer, funcDelegate, 1.0f, true, 0.0f);
```

### Blueprint Library 만들기 예시

```
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
```
	
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
이는 FSomeStruct는 RValue라서 그렇다. RValue는 const라서 값을 변경할 수 없다.
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

hit result에서 BoneName을 찾아 어느 Bone에 맞았는지 확인하고 데미지를 다르게 적용하면 된다.

언리얼 FString, FName은 switch()에 사용할 수 없으니 if else if 등을 써야할 것이다.


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

만약 BaseManager의 CPP 파일에 include를 하려는 시도를 한다면
#include "Base/BaseCharacter.h" 라고 입력하면 컴파일 오류가 난다.
#include "../Base/BaseCharacter.h"라고 입력해야한다.

마찬가지로 처음에 클래스를 생성하면, cpp 파일의 경로가 존재하지 않는다면서 오류가 난다.
cpp 파일을 들어가보면 #include "Base/BaseCharacter.h"라고 되어있는게 오류가 난다.
#include "BaseCharacter.h"로 고쳐야 오류가 나지 않는다.
```

이는 .Build.cs에 Source/[YourProjectName]을  '모듈'이라 가정하고 Path를 등록해주어야 오류가 나지 않는다.

PrivateIncludePaths.AddRange(new string[] { "[YourProjectName]" });

```
예시 해결법
       PrivateIncludePaths.AddRange(new string[] { "ProjectA" });

를 .Build.cs에 추가한다.
```

아마 Source 폴더에 다른 폴더를 만들었다면 이와 같은 방식으로 Path를 등록해주면 될 듯 하다.

### Heapify

최소힙 or 최대힙을 사용해서 최소/최대 값을 빠르게 얻어낼 수 있다.
	
사실 Heap과 PriorityQueue는 같은 기능을 한다.
	
근데 언리얼에 PriorityQueue가 없다. 따로 만들어줘야한다.
	
근데 Heap을 쓸 수 있다면 Heap으로 대체할 수 있으니 Heap쓰는 방법을 알면 된다.
	
	
```
    FORCEINLINE bool operator<(const UPathNode& Other)
    {
        return F_Cost < Other.GetFCost();
    }

     static bool PathFindingPredicated(const UPathNode& A, const UPathNode& B)
    {
        return A.GetFCost() < B.GetFCost();
    }
```

```
	TArray<UPathNode*> openList;

//아래 방법 중에 원하는 걸 사용하면 됨.

	openList.Heapify(); // 이거 안됨.
	
	openList.Heapify(UPathNode::PathFindingPredicated); // 미리 정의된 Predicate를 사용하는 방법
	
	openList.Heapify([](const UPathNode& A, const UPathNode& B) //람다로 Predicate를 정의하는 방법
	{
		return A.GetFCost() < B.GetFCost();
	});
	
	//!! 이 방법을 사용하면 TArray에서 Add할 때 반영이 안된다. 그래서 밑의 HeapSort도 해야함.
	Algo::Heapify(openList, [](UPathNode* A, UPathNode* B)
		{
			return A->GetFCost() < B->GetFCost();
		});
	//추가할 때마다 HeapSort를 써야함.
	Algo::HeapSort(PathNodeList, [](UPathNode* A, UPathNode* B)
		{
			return A->GetFCost() < B->GetFCost();
		});
```

### Instanced Static Mesh의 Custom Data Value

Instanced Static Mesh Component 전용의 Material을 만들었다면

Custom Data Value를 세팅하여 Instance마다 다른 Material 효과가 나게 설정할 수 있음.

먼저 ISM_Component의 NumCustomDataFloats를 필요한 Data Value 수만큼 세팅함.
SetCustomDataValue(instance Index , Custom Data Value Index, Value(float)); 로 세팅.

```
예시)

UInstancedGridVisualComponent::UInstancedGridVisualComponent()
{
	//사용가능한 Material의 Custom Data Value의 최대 갯수
	NumCustomDataFloats = 8;
}

void UInstancedGridVisualComponent::DrawGridVisualswithGridSet(const TSet<FGrid>& GridSet, const float Height)
{
...
		FGrid upGrid = currentGrid + FGrid(0,1);
		float upValue = GridSet.Contains(upGrid) ? 0.0f : 1.0f;
		SetCustomDataValue(i, 0, upValue);
...
}
```

### Bitmask

```
UENUM(Meta = (Bitflags))
enum class EBitmaskExample : uint8
{
	A,
	B,
	C
};

{
...
...

UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BitMask, BitmaskEnum = "EBitMaskExample"))
int32 BitmaskFlags;

...
...
}

```

1. enum 클래스를 만들고 Meta = (Bitflags) 설정.

2. int32 변수를 만들고 URPOPERTY(meta = (BitMask, BitmaskEnum = "[EnumClassName]")) 설정.

이렇게 만들면 에디터에서 Bitmask로 사용할 수 있다.

BitmaskFlags의 값이 int32이므로 bitmask로 최대 32개까지만 사용할 수 있을 것이다.

### Object Type & Collision Profile

DefaultEngine.ini에 들어가면
사용자가 추가한 Object Type이나 Collision Profile의 ECC_GameTraceChannel[번호] 등은 알 수 있음.

Object Type을 추가했다면, 필요한 액터에 Object Type을 변경해야하는 점을 잊지 말아야함.

```
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;

	objects.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_GameTraceChannel2));
```
