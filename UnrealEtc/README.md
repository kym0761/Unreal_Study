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
Sequencer에서 AnimBP를 사용하려면 AnimBP이 Enum에 맞게 움직이도록 해야 한다.
1. Enum을 만든다
2. AnimBP에 enum 변수를 만든다
3. State Machine을 enum에 맞게 동작하도록 한다.

그런데, 이렇게 만들면 runtime때만 애니메이션이 동작할 것이다.
다만, 만드는 동안에 애니메이션이 동작하지 않아도 문제가 있지는 않다. 어차피 Sequencer를 runtime에서만 동작하게 할테니깐.

### Sequencer에 관한 정보

Sequencer는 렌더링 할 때나, 인게임 중에 플레이되면
결국 플레이 중일 때로 기준으로 동작한다
예를 들자면, 렌더링 중이면 게임 실행시 보이지 말아야할 각종 Gizmo들이 보이지 않을 것이다.
그러니 에디터에서 편집 & 재생 할 때 해당 Gizmo 들이 보여도 실제론 다르니 걱정하지 말 것

언리얼 4.2X 에서 Sequencer로 렌더링 하는 방식이 바뀌어 Movie Render Queue를 사용해야한다.
Command Line Encoder를 Exports에 설정해두어야 영상으로 변환이 가능하다.
추가 설정은 Project Settings -> Plugins -> Movie Pipeline CLI Encoder에 들어간다.
Executable Path 는 ffmpeg.exe가 있는 경로
Video Codec은 libx264
audio Codec은 aac
Output File Extension은 avi나 mp4 등의 확장자를 세팅하고 Movie Render Queue를 동작하면 된다.

jpg 말고 다른 파일을 기반으로 렌더링 하고 싶었지만 잘 안됐다.
그냥 jpg써야할지도.



### Drag해서 미리 배치된 pawn과 Spawn된 pawn의 차이

possess 차이가 있을 수 있다.
pawn 기본 세팅이 auto Possess ai가 placed in world로 되어 있어서 Spawn된 Pawn은 aicontroller의 영향을 안받는다.
ai에게 possess가 안되어 있다면 movement와 gravity 등이 제대로 동작 안하기 때문에
pawn 만든 뒤에 auto possess ai를 placed in world or Spawned로 고친다.

### 기 타

기타? ㅁ?ㄹ