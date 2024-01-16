# Unreal_Study/UnrealC++/UnrealC++_Bitmask
 

### Bitmask

```
UENUM(Meta = (Bitflags))
enum class EBitmaskExample : uint8
{
	A,
	B,
	C
};

{
...
...

UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (BitMask, BitmaskEnum = "EBitMaskExample"))
int32 BitmaskFlags;

...
...
}

```

1. enum 클래스를 만들고 Meta = (Bitflags) 설정.

2. int32 변수를 만들고 URPOPERTY(meta = (BitMask, BitmaskEnum = "[EnumClassName]")) 설정.

이렇게 만들면 에디터에서 Bitmask로 사용할 수 있다.

BitmaskFlags의 값이 int32이므로 bitmask로 최대 32개까지만 사용할 수 있을 것이다.
