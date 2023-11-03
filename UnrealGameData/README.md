# Unreal_Study/GameData


## AssetData

### AssetManager

엔진이 초기화될 때 제공되는 에셋 관리용 싱글톤 클래스다.

PrimaryAssetId를 사용해 프로젝트 내 에셋의 주소를 얻어올 수 있다.  
PrimaryAssetId는 "태그"와 "이름" 두 가지 "키 조합"으로 구성될 수 있다.  
특정 태그를 가진 모든 에셋 목록을 가져올 수 있다는 뜻이다.  

AssetData를 폴더 째로 받아와 관리할 수 있다.

Project Settings -> Game / AssetManager 

Primary Asset Type to Scan

Primary Asset Type  
Asset Base Class  
Directories를 세팅해주면 된다. 이 디렉토리는 에셋이 모두 모인 폴더를 설정해주면 된다.


UPrimaryDataAsset의 GetPrimaryAssetId()를 override 해주면 된다. 
```
UCLASS()
class ARENABATTLE_API UABItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ABItemData", GetFName()); //적당한 tag 명, 에셋의 FName으로 위의 태그와 이름을 설정
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	EItemType Type;
};
```


## DataTable

.cvs 데이터를 구조체의 안의 프로퍼티들의 이름과 맞게 만들면 엑셀 시트 데이터로 관리할 수 있다.

## 데이터 관리용 싱글톤 클래스

UObject로 싱글톤으로 사용할 오브젝트를 만든다. 그러면 GEngine에 등록되어 사용이 가능하다.

Project Settings의 Engine General Settings에 Advanced Default Class에 Singleton 클래스에 등록해 사용하면 된다.

```
UCLASS()
class ARENABATTLE_API UABGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	UABGameSingleton();
	static UABGameSingleton& Get();

// Character Stat Data Section
public:
	FORCEINLINE FABCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel - 1) ? CharacterStatTable[InLevel - 1] : FABCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FABCharacterStat> CharacterStatTable;
	
};

```

```
UABGameSingleton& UABGameSingleton::Get()
{
	//GEngine으로부터 GameSingleton을 불러온다.
	UABGameSingleton* Singleton = CastChecked< UABGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogABGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<UABGameSingleton>();
}
```