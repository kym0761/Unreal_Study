# Unreal_Study/UnrealC++/UnrealC++Enum
 
### enum 만들기 예시

```
UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	None UMETA(DisplayName = "None"),
	Buff UMETA(DisplayName = "Buff"),
	Debuff UMETA(DisplayName = "Debuff")

};
```

### FindObject

언리얼 C++ 코드에서 지정한 UMETA를 받아오는 방법임

```
EAbilityType abilType = GetAbilityType();
const UEnum* enumType = FindObject<UEnum>(nullptr, TEXT(/Script/[modulename].EAbilityType));

if(enumType)
{

FString abilMetaData = enumType->GetDisplayNameTextByValue((int64)abilType).ToString();

}


```