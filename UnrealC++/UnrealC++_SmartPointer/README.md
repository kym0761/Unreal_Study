# Unreal_Study/UnrealC++/UnrealC++_SmartPointer
 
### Smart Pointer

스마트 포인터는 C++의 포인터의 가비지 컬렉션 되지 않아 메모리 누수를 막기 위한 포인터 객체다.

Unreal도 있다.
~~~
TUniquePtr;
TSharedPtr;
TWeakPtr;
~~~

보통은 TSharedPtr, TWeakPtr을 사용하게 될 것이다.

원래 UObject는 위의 스마트포인터를 묶을 수가 없고, 묶을 이유가 없긴 했는데 에픽게임즈에서 TObjectPtr을 만들었다.

실질적으로 UObject*를 TObjectPtr<UObject>로 고쳐서 로직이 돌아가고 있기 때문에, 둘중 아무거나 써도 상관없는 것 같다.

다만, 언제든 UObject에 대한 순수 포인터를 막을지도 모르니 이후에 TObjectPtr<T>로 고칠 필요가 있음.

UFUNCTION은 5.1 기준으로는 아직 TObjectPtr을 Parameter로 받을 수 없다.

~~~
// @TODO: OBJPTR: Investigate TObjectPtr support for UFunction parameters.

// @TODO: OBJPTR: we want to permit lazy background loading in the future
// else if (*LoadBehaviorMeta == "LazyBackground")
// {
// 	LoadBehavior = EImportLoadBehavior::LazyBackground;
// }

// @TODO: OBJPTR: Need to find other options for solving this issue of placeholder classes during blueprint compile without forcing all imports to resolve always

// @TODO: OBJPTR: We should have a test that ensures that lazy loading of an object with an external package is handled correctly.
~~~

TODO 목록을 보면 순수 포인터를 대체하기 위한 장기적 프로젝트로 보이니 되도록 고치는 것을 추천.



