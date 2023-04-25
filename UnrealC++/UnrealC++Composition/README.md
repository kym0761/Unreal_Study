# Unreal_Study/UnrealC++/UnrealC++Composition


### Composition

Is_A는 상속 관계임. 유지 보수 및 설계가 매우 어려움
has_A는 컴포지션 관계임

컴포지션 관계로 SOLID 원칙을 구현하기 좋음.

내가 소유한 언리얼 오브젝트는 Subobject라고 부름.
나를 소유한 언리얼 오브젝트는 Outer라고 부른다.

### 컴포지션 구현법

생성자(cdo)에서 CreateDefaultSubobject()를 사용할 것
beginplay, init 등에서는 NewObject<>()를 사용할 것.

부모 클래스가 이미 CreateDefaultSubobject로 오브젝트를 생성했다면
이 후 자식 클래스는 이를 실행하지 않고 접근 가능함.

### TObjectPtr<T>

기존 언리얼에서 오브젝트를 포인터로 썻지만, TObjectPtr<T>이 더 좋으니 꼭 사용할 것.