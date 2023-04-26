// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TestObject.generated.h"

/**
 * 테스트용 UObject
 */
UCLASS()
class TEMPLATETEST_API UTestObject : public UObject
{
	GENERATED_BODY()

public:

	UTestObject();

	FString GetObjName() const;
public:

	FString ObjName;

};
