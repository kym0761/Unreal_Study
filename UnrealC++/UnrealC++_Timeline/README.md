# Unreal_Study/UnrealC++/UnrealC++_Timeline
 

### C++에서 Timeline 사용하기


1. Timeline을 동작시킬 C++ 클래스에 FTimeline, UCurveFloat를 추가한다. 

```
class APuzzleDoor // 자세한 것은 생략.
{
	...
	...
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timeline")
		TObjectPtr<UCurveFloat> CurveForTimeline; //언리얼 에디터에서 만든 "float 커브"로 타임라인을 동작시킬 예정..

		FTimeline TimelineHandle;
	...
	...
}

```
2. 블루프린트 타임라인의 Output으로 동작하는 Function을 만들어준다.

```
void APuzzleDoor::TimelineFloatFunction(float Value) //타임 라인에 맞춰 문을 열게 만드는 간단한 Function
{
	FVector doorMeshLocation = FVector(0.0f,0.0f,Value);
	Mesh->SetRelativeLocation(doorMeshLocation);
}
```



3. FTimeline에 위에 미리 만든 Function을 Bind한다.

```
void APuzzleDoor::InitializeForTimeline() //BeginPlay()에서 동작할 예정이다.
{
	if (CurveForTimeline)
	{
		FOnTimelineFloat onTimelineFloat; //float말고도 LinearColor, Vector도 사용할 수 있다.
		onTimelineFloat.BindDynamic(this, &APuzzleDoor::TimelineFloatFunction); //바인드를 위한 FOnTimeline

		TimelineHandle.AddInterpFloat(CurveForTimeline, onTimelineFloat); //Curve와 FOnTimeline를 TimelineHandle에 세팅.
		TimelineHandle.SetLooping(false); //끝나면 다시 처음부터 루프할지 아니면 그대로 끝낼지 선택.
	}
}
```

4. FTimeline의 TickTimeline(deltatime)을 Tick()에서 동작시켜준다.

```
void APuzzleDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Timeline Need this.
	TimelineHandle.TickTimeline(DeltaTime);

}

```

5. 이 타임라인을 사용하는 방법의 예시는 다음과 같다.

```
void APuzzleDoor::OpenDoor()
{

TimelineHandle.Play(); //블루프린트 타임라인의 Play와 같다.
}

```

```
void APuzzleDoor::CloseDoor()
{

	TimelineHandle.Reverse(); //블루프린트 타임라인의 Reverse와 같다.
}

```

그 외에도 Stop이나 Play From Start ... 등 여러가지가 있다.
