# Unreal_Study/UnrealC++/UnrealC++Delegate

### 느슨한 결합
클래스간에 의존성을 가지지 않도록 함.

클래스가 자신이 해야할 작업에만 집중할 수 있는 장점이 생기고
외부의 변경이 자신에게 영향을 받지 않는다.

인터페이스를 구현하여 만들 수도 있지만, 델리게이트를 이용한 함수콜을 통해 구현하는 것도 좋음

### 발행 구독 디자인 패턴

발행은 생산, 구독은 소비만 하면 됨
유지보수하기 쉬움.

### Delegate

예시
DECLARE_DYNAMIC_MULTICAST_DELEGATE()


몇개의 인자?
어떤 방식으로 전달?
일대일? 일대다?

C++에서만 사용? 블루프린트에서도사용?

클래스 외부에 설계된 C++ 함수와 연결할건가?
 전역에 설계한 정적 함수를 연결할것인가? 
언리얼 오브젝트의 멤버 함수를 연결할것인가? (대부분은 이를 위해 사용함)

DYNAMIC - 블루프린트에서도 지원하는 델리게이트임을 의미
MULTICAST - 일대다 형태라는 것을 의미

즉
DECLARE_DELEGATE()
이는 C++만 지원하는 일대일 델리게이트라는 뜻임.

DECLARE_DELEGATE_RetVal_ThreeParams()
위의 델리게이트는 반환값이 존재하고 3개의 인자를 받는 다는 뜻이다.
multicast는 반환값을 지원하지 않으니 참고할 것.
파라미터는 최대 9개까지 지원한다.
