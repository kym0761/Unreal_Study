# Unreal_Study/UnrealC++/UnrealC++_Timer
 

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

