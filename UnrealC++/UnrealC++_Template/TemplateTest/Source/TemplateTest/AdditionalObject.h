// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AdditionalObject.generated.h"

/**
 * Template�� ����� �����ϴ��� Ȯ���ϱ� ���� �߰����� UObject
 */
UCLASS()
class TEMPLATETEST_API UAdditionalObject : public UObject
{
	GENERATED_BODY()
	
public:

	FString ObjName;

	UAdditionalObject();

};
