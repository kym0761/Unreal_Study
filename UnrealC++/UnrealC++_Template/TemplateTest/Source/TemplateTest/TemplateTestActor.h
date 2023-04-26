// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateHolder.h"
#include "TestObject.h"
#include "AdditionalObject.h"
#include "TemplateTestActor.generated.h"

UCLASS()
class TEMPLATETEST_API ATemplateTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATemplateTestActor();

	TSharedPtr<FTemplateHolder<UTestObject>> TemplateHolder01;
	TSharedPtr<FTemplateHolder<UAdditionalObject>> TemplateHolder02;

	FTimerHandle TestTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TimerFunc();

};
