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

### 현재 overlapping 중인 Actor들을 찾기

```
TArray<AActor*> overlapActors;
FireOverlap->GetOverlappingActors(overlapActors); // FireOverlap은 UBoxComponent다.
```

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

