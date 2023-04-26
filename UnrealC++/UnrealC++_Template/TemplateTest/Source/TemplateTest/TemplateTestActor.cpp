// Fill out your copyright notice in the Description page of Project Settings.


#include "TemplateTestActor.h"

// Sets default values
ATemplateTestActor::ATemplateTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATemplateTestActor::BeginPlay()
{
	Super::BeginPlay();

	TemplateHolder01 = MakeShareable(new FTemplateHolder<UTestObject>());

	for (int32 i = 0; i < 5; i++)
	{
		UTestObject* test = NewObject<UTestObject>();
		test->ObjName = FString(TEXT("Object_")) + FString::FromInt(i);
		TemplateHolder01->ObjectArray.Emplace(test);
	}

	TemplateHolder01->SafeObjectArray = TemplateHolder01->ObjectArray;

	TemplateHolder02 = MakeShareable(new FTemplateHolder<UAdditionalObject>());

	for (int32 i = 0; i < 3; i++)
	{
		UAdditionalObject* ad = NewObject<UAdditionalObject>();
		ad->ObjName = FString(TEXT("AdditionalObject_")) + FString::FromInt(i);
		TemplateHolder02->ObjectArray.Emplace(ad);
	}

	TemplateHolder02->SafeObjectArray = TemplateHolder02->ObjectArray;

	//만약 이 코드가 활성화된다면, 1번째 Object는 GarbageCollection이 일어날 것이다.
	TemplateHolder02->SafeObjectArray.RemoveAt(1);

	//ProjectSettings -> Garbage Collection의 GCCycle을 3초로 세팅함.
	GetWorldTimerManager().SetTimer(TestTimer, this, &ATemplateTestActor::TimerFunc, 2.0f, true);
}

// Called every frame
void ATemplateTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATemplateTestActor::TimerFunc()
{
	if (!TemplateHolder01.IsValid())
	{
		return;
	}

	if (!TemplateHolder02.IsValid())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("=============="));

	for (int32 i = 0; i < TemplateHolder01->ObjectArray.Num(); i++)
	{
		auto currentObj = TemplateHolder01->ObjectArray[i];
		if (currentObj->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Log, TEXT("Obj : %s"), *currentObj->ObjName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Obj is Garbage Collected.."));
		}
	}


	UE_LOG(LogTemp, Log, TEXT("=============="));

	for (int32 i = 0; i < TemplateHolder02->ObjectArray.Num(); i++)
	{
		auto currentObj = TemplateHolder02->ObjectArray[i];
		if (currentObj->IsValidLowLevel())
		{
			UE_LOG(LogTemp, Log, TEXT("Obj : %s"), *currentObj->ObjName);
		}
		else
		{
			UE_LOG(LogTemp, Log, TEXT("Obj is Garbage Collected.."));
		}
	}

	UE_LOG(LogTemp, Log, TEXT("=============="));
}

