# Unreal_Study/UnrealC++_TSet

### 문서
https://docs.unrealengine.com/5.1/ko/set-containers-in-unreal-engine/

### TSet

C++의 Set은 트리 기반 구조

언리얼의 TSet은 해시 기반

unordered_set과 비슷한 구조임.


 
### 구조체를 TMap, TSet의 키로 사용하기

```
//예시

USTRUCT(BlueprintType, Blueprintable)
struct FGrid
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 X;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
	int32 Y;

...
...
	bool operator==(const FGrid& Other) const;

};

//To Use in TSet Or TMap. You Need To Make operator==, GetTypeHash(). 
uint32 GetTypeHash(const FGrid& Grid);

```

1. 구조체에 == operator가 구현되어 있어야함.
2. 아래의 함수 GetTypeHash가 구현되어 있어야함.
```
uint32 GetTypeHash(const StructName& Param)
{
return FCrc::MemCrc32(&Param, sizeof(Param));
}
```

