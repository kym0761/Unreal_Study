// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Containers/UnrealString.h"
#include "UObject/GCObject.h"

/**
 *  Template 클래스
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

	TArray<T*> ObjectArray; //읽을 Object
	TArray<T*> SafeObjectArray; //GC로부터 안전할 Object

	//매 GarbageCollect가 되기 전에 AddReferencedObjects가 동작해 매번 Obj를 등록하는 것으로 보임.
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

	//이 C++ 클래스는 Referencer가 되어야함을 명시하는 것으로 보임.
	virtual FString GetReferencerName() const override
	{
		return TEXT("FTemplateHolder");
	}

};
