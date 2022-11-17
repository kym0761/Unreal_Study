# Unreal_Study/UnrealC++
 
### Foliage가 거리가 멀어지면 사라지는 이유

1. Foliage Paint로 만들었으면 Foliage mesh 선택창에서 Cull Distance를 확인하기.
2. Landscape Grass Type을 통해 Landscape Material로 Foliage를 만들었다면, Landscape Grass Type의 각 Inst의 Cull Distance 확인하기.
3. 그게 아니면 맵 만든 사람이 따로 만든 커스텀 Culling이 존재하는 것.

### Foliage 종류

Foliage는 여러 종류가 있다. 만약 쓸모없는 Foliage가 존재한다면 어떤 Foliage가 있는지를 먼저 알아야 한다.
일단 알게된 Foliage는 아래 3가지다.

1. landscape로 만든 foliage
2. procedural volume에 의해 생성된 foliage
3. landscape material에 설정된 foliage

### 최적화
1. Texture LOD Bias 값을 건드려서 최대 Texture 사이즈 조절하기.
2. Mesh의 Min LOD를 건드려서 근접시 최대 퀄리티가 나오지 않게 막기.
3. 필요없는 mesh 액터들을 삭제해보기.
4. 중복되는 Mesh를 사용하는거라면 Instanced Static Mesh로 여러개 만들기.
5. 쓸데없는 라이트가 많은지 확인해볼 것.
6. Material의 비싼 노드를 비활성화하여 연산 줄이기.
7. Mesh 자체가 문제라면 Mesh를 고쳐보기.

다 안됨? 그럼 로직이 잘못된 걸지도 모름? 코드를 수정해보기.
그게 아니면 걍 언리얼이 무거워서 느린거임 ㅅㄱ

### Animation의 Sequencer 처리

AnimBP를 만들어서 Sequncer에서 움직여도 의도된대로 동작하지 않는다.
Sequencer에서 AnimBP를 사용하려면 AnimBP에 Enum을 주고, Enum에 맞게 움직이도록 해야 한다.
1. Enum을 만든다
2. AnimBP에 enum 변수를 만든다
3. State Machine을 enum에 맞게 동작하도록 한다.

그런데, 이렇게 만들면 runtime때만 애니메이션이 동작할 것이다.
그러니 만드는 동안에 애니메이션이 동작하지 않아도 문제가 있지는 않다. 어차피 Sequencer를 runtime에서만 동작하니깐.

