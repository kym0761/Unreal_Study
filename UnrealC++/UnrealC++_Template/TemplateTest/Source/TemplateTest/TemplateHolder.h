// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/UnrealString.h"
#include "UObject/GCObject.h"

/**
 *  Template Ŭ����
 */
template<typename T>
class TEMPLATETEST_API FTemplateHolder : public FGCObject
{
public:


	FTemplateHolder()
	{
	
	}

	virtual ~FTemplateHolder()
	{
	
	}

	TArray<T*> ObjectArray; //���� Object
	TArray<T*> SafeObjectArray; //GC�κ��� ������ Object

	//�� GarbageCollect�� �Ǳ� ���� AddReferencedObjects�� ������ �Ź� Obj�� ����ϴ� ������ ����.
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		for (int32 i = 0; i < SafeObjectArray.Num(); i++)
		{
			if (SafeObjectArray[i] && SafeObjectArray[i]->IsValidLowLevel())
			{
				Collector.AddReferencedObject(SafeObjectArray[i]);
			}
		}
	}

	//�� C++ Ŭ������ Referencer�� �Ǿ������ ����ϴ� ������ ����.
	virtual FString GetReferencerName() const override
	{
		return TEXT("FTemplateHolder");
	}

};
