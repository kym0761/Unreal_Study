// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AdditionalObject.generated.h"

/**
 * Template가 제대로 동작하는지 확인하기 위한 추가적인 UObject
 */
UCLASS()
class TEMPLATETEST_API UAdditionalObject : public UObject
{
	GENERATED_BODY()
	
public:

	FString ObjName;

	UAdditionalObject();

};
