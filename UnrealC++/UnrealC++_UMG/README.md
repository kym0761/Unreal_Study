# Unreal_Study/UnrealC++/UnrealC++_UMG
 

### UMG C++로 만들 때

```
UCLASS(abstract)
class PROJECTLIFE_API UConditionIconWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (BindWidget))
		UImage* ConditionImage;
}
```

 Meta = (BindWidget)를 Specifier로 추가한 뒤, UMG Widget 블루프린트에서 해당 변수와 같은 위젯을 세팅한다.
 
 위의 예시대로면 Image 위젯을 추가하고 이름을 ConditionImage 지으면 UMG Widget 블루프린트와 연동된다.
 
 void NativeConstruct()에서도 추가하는 방법이 있는데, 개인적으론 위 방법을 선호한다.
 ```

void USpawnSlotBase::NativeConstruct()
{
	Super::NativeConstruct();

	ImageBorder = Cast<UBorder>(GetWidgetFromName(TEXT("ImageBorder")));
}

```


### UMG C++로 생성하기.
	h:
		UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Component")
		TSubclassOf<UActionSelectButtonWidget> ChooseActionButtonWidgetClass;

	cpp:
	UActionSelectButtonWidget* buttonWidget = CreateWidget<UActionSelectButtonWidget>(GetWorld(), ChooseActionButtonWidgetClass);
	buttonWidget->AddToViewport();
	
외형을 블루프린트로 만들 것이므로, h에서 외형을 연결하고 CreateWidget에서 사용해야한다.

외형 Widget 블루프린트는 해당 C++ 클래스를 상속받아야한다.


