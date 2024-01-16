# Unreal_Study/UnrealC++/UnrealC++_연산자Overloading
 
### operator overloading

```
	FInGameTime operator+(const FInGameTime& rValue);
	FInGameTime& operator+=(const FInGameTime& rValue);

	FInGameTime operator-(const FInGameTime& rValue);
	FInGameTime& operator-=(const FInGameTime& rValue);
```

parameter로 const와 &를 붙이는 것이 중요하다.
이유 :
void SomeFunc(FSomeStruct& a); 가 있다 가정하고

someFunc(FSomeStruct());는 사용이 불가능하다.
이는 FSomeStruct는 RValue라서 그렇다. RValue는 const라서 값을 변경할 수 없다.
FSomeStruct a로 Parameter를 고치면 사용은 가능하지만 복사 Parameter라서 구조체의 크기가 큰 경우에 비효율적이다.

위의 operator의 경우에는
FInGameTime a, b;
...
a+=b;
이렇게 사용할 수도 있지만,

FInGameTime a = 0;
a+= FInGameTime(1,1...);
이런 식으로도 사용할 수 있어야 한다.
그러므로 const와 &가 필요하다.

사실 RValue의 경우, void SomeFunc(FSomeStruct&& a);
를 overloading할 수도 있긴 한데, 기억상 operator의 경우에는 언리얼에서 모호성 컴파일 오류가 났다.
	
만약, 들어오는 input이 함수 내에서 변경될 일이 아예 없으며 위에처럼 변수생성을 함수 파라미터에 직접 선언하면서 쓸 경우엔 const와 &를 꼭 쓰는 것이 좋다.
	

```
FInGameTime& FInGameTime::operator-=(const FInGameTime& rValue)
{
	*this = *this - rValue;

	return *this;
}
```

return이 &이면 위처럼 만든다.

