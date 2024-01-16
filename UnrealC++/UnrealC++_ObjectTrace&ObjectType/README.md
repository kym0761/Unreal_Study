# Unreal_Study/UnrealC++/UnrealC++_ObjectTrace&ObjectType
 
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

### Object Type & Collision Profile

DefaultEngine.ini에 들어가면
사용자가 추가한 Object Type이나 Collision Profile의 ECC_GameTraceChannel[번호] 등은 알 수 있음.

Object Type을 추가했다면, 필요한 액터에 Object Type을 변경해야하는 점을 잊지 말아야함.

```
	TArray<TEnumAsByte<EObjectTypeQuery>> objects;

	objects.Add(UEngineTypes::ConvertToObjectType(
		ECollisionChannel::ECC_GameTraceChannel2));
```
