# Unreal_Study/UnrealC++/UnrealC++_BlueprintLibrary
 


### Blueprint Library 만들기 예시

```
UCLASS()
class PROJECTLIFE_API UGameTimeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, meta = (DisplayName = "GameTime + GameTime", 
		CompactNodeTitle = "+", KeyWords = "+ Plus", CommutativeAssosiativeBinaryOperator = "true"), 
		Category = "GameTime|Convert")
		static FInGameTime Plus_GameTimeGameTime(FInGameTime InValue1, FInGameTime InValue2);

	UFUNCTION(BlueprintPure, meta = (DisplayName = "ToString (GameTime)",
		CompactNodeTitle = "->", BlueprintAutocast),
		Category = "GameTime|Convert")
		static FString Conv_GameTimeToString(FInGameTime InGameTime);

	UFUNCTION(BlueprintPure, Category = "GameTime|Get")
		static int32 GetYear_GameTime(FInGameTime InGameTime);
	...
};
```
	
Category는 블루프린트의 카테고리 검색에 용이하게 설정할 수 있다.

