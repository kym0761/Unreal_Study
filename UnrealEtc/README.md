# Unreal_Study/Etc

## 설명

분류를 지정하지 못한 내용을 정리함.

추후에 분류 카테고리가 생기면 옮겨질 수 있음.

### Foliage가 거리가 멀어지면 사라지는 이유

~~~
1. Foliage Paint로 만들었으면 Foliage mesh 선택창에서 Cull Distance를 확인하기.
2. Landscape Grass Type을 통해 Landscape Material로 Foliage를 만들었다면, Landscape Grass Type의 각 Inst의 Cull Distance 확인하기.
3. Cull Distance Volume.
~~~

### Foliage 종류

Foliage는 여러 종류가 있다. 만약 쓸모없는 Foliage가 존재한다면 어떤 Foliage가 있는지를 먼저 알아야 한다.
일단 알게된 Foliage는 아래 3가지다.

~~~
1. landscape로 만든 foliage
2. procedural volume에 의해 생성된 foliage
3. landscape material에 설정된 foliage (Material에 따라 알아서 foliage가 생성됨)
~~~

각 방법에 따라 수정 및 접근 방법이 다르므로 종류 정도는 알 필요가 있음.

### 최적화
~~~
1. Texture LOD Bias 값을 건드려서 최대 Texture 사이즈 조절하기. (4096X4096 -> 1024X1024)
2. Mesh의 Min LOD를 건드려서 근접시 최대 퀄리티가 나오지 않게 막기. (다만, 이 방법은 직접 적절한 거리에서도 Mesh가 올바르게 렌더 되는지 확인 필요함.)
3. 필요없는 Mesh 액터들을 삭제해보기.
4. 중복되는 Mesh를 사용하는거라면 Instanced Static Mesh로 여러개 만들기.
5. Foliage도 너무 많으면 줄여보기.
6. 쓸데없는 라이트가 많은지 확인해볼 것.
7. Moveable, Stationary Light가 꼭 필요한지 확인해볼 것.
8. Material의 비싼 노드를 비활성화하여 연산 줄이기. (반사나, 유리창 효과같은 내부 렌더 등등)
9. Mesh 자체가 문제라면 Mesh를 고쳐보기. (필요 이상으로 Vertex나 Edge가 존재)
~~~

다 안된다면, 게임 로직이 잘못된 걸지도 모른다. 코드를 수정해보기.

그게 아니면 하드웨어 성능이 낮아서 그럴 가능성이 높으니 하드웨어를 업그레이드 해야함.

~~~
언리얼 5 기준으로 추천하는 사양은
인텔 or AMD cpu 6코어 12스레드 이상
램 16GB 이상 (가능하다면 32기가나 64기가 정도)
글카 RTX 3060ti 이상
~~~

### Animation의 Sequencer 처리

AnimBP를 만든 Animation State는 Sequencer에서 움직여도 의도된대로 동작하지 않는다.
AnimBP를 적용하면 AnimBP 로직과 Sequencer에서 재생할 Animation 타이밍이 꼬인다.
Sequencer에서 AnimBP를 사용하려면 AnimBP이 Enum에 맞게 움직이도록 해야 한다.
~~~
1. Enum을 만든다
2. AnimBP에 enum 변수를 만든다
3. State Machine을 enum에 맞게 동작하도록 한다.
~~~
그런데, 이렇게 만들면 runtime때만 애니메이션이 동작할 것이다.
다만, 만드는 동안에 애니메이션이 동작하지 않아도 문제가 있지는 않다. 어차피 Sequencer를 runtime에서만 동작하게 할테니 문제는 없다.

### Sequencer에 관한 정보

Sequencer는 렌더링 할 때나, 인게임 중에 플레이되면 결국 플레이 중일 때로 기준으로 동작한다
예를 들자면, 렌더링 중이면 게임 실행시 보이지 말아야할 각종 Gizmo들이 보이지 않을 것이다.
그러니 에디터에서 편집 & 재생 할 때 해당 Gizmo 들이 보여도 실제론 다르니 걱정하지 말 것

~~~
언리얼 4.2X 에서 Sequencer로 렌더링 하는 방식이 바뀌어 Movie Render Queue를 사용해야한다.
Command Line Encoder를 Exports에 설정해두어야 영상으로 변환이 가능하다.
추가 설정은 Project Settings -> Plugins -> Movie Pipeline CLI Encoder에 들어간다.
Executable Path 는 ffmpeg.exe가 있는 경로
Video Codec은 libx264
audio Codec은 aac
Output File Extension은 avi나 mp4 등의 확장자를 세팅하고 Movie Render Queue를 동작하면 된다.
~~~

jpg 말고 다른 파일을 기반으로 렌더링 하고 싶었지만 잘 안됐다.
그냥 jpg써야할지도.

ffmpeg.exe는 다운받아서 써야한다.


### Drag해서 미리 배치된 pawn과 Spawn된 pawn의 차이

possess 차이가 있을 수 있다.
pawn 기본 세팅이 auto Possess ai가 'placed in world'로 되어 있어서 Spawn된 Pawn은 aicontroller의 영향을 안받는다.
ai에게 possess가 안되어 있다면 movement와 gravity 등이 제대로 동작 안하기 때문에
pawn 만든 뒤에 auto possess ai를 'placed in world or Spawned'로 고친다.

### 언리얼 프로젝트 이름 바꾸기

 언리얼 프로젝트 이름을 바꾸려면 uproject 파일 뿐만 아니라 Config의 내용과 C++ 소스의 [ProjectName]_API도 다 바꿔줘야한다.

 이후, 언리얼 프로젝트 컨텐츠에 있는 모든 리다이렉트가 고장난다.
~~~
 예시를 들어보자면, ProjectA라고 명칭을 지어놨던 프로젝트를 ProjectABC로 바꾼다는 가정이다.

 위의 방식대로 소스코드 및 uproject, config를 수정할 명칭에 맞게 고쳤을 때, 컨텐츠에 있는 대부분의 블루프린트가 Invalid Class가 된다.

 이는 블루프린트 안에 /Script/[ProjectName] 으로 리다이렉팅이 되어 있기 때문이다.
 
예를 들자면, /Script/ProjectA인 블루프린트는 위의 소스 코드 등을 고쳐도 이 모든 것이 그대로 되어 있다.

 이는 Config에서 DefaultEngine.ini의 [/Script/Engine.Engine]에서 +ActiveGameNameRedirects=(OldGameName="/Script/[과거명칭]",NewGameName="/Script/[현재명칭]")을 추가하면 리다이렉트를 다시 고칠 수 있다.
 
일단 Invalid Class 문제가 해결된 뒤에, 만약 위의 ini의 코드를 지우고도 올바르게 동작하게 하고 싶다면 문제의 소지가 있을 블루프린트들을 다시 저장 및 컴파일을 해주어 리다이렉트가 올바르게 고쳐질 필요가 있음을 명심해야한다.
~~~

### 언리얼 C++ 플러그인을 C++을 사용할 수 없는 환경에서 동작하게 만들기.

```
언리얼 프로젝트로 플러그인을 빌드한다.
플러그인 빌드된 것을 통째로 C:\Program Files\Epic Games\[언리얼 버전]\Engine\Plugins 에 넣는다.
해당 플러그인의 Binaries Win64에 들어가서 UnrealEditor.Module의 BuildId를 플러그인 폴더에 있는 MarketPlace의 BuildId로 대체한다.
```

### The execution order of BeginPlay 

Begin Play in Editor  


Actor in Level  
Game Mode  
Level  
Game State  
Player Controller  
Player State  
HUD  
Pawn  


Begin Play in Built Game


Pawn  
Game Mode  
Game State  
Player State  
Player Controller  
HUD  
Level  
Actor in Level  

### 명령어

"~"눌러서 입력하면 된다.

DisplayAll PlayerController ControlRotation - 플레이어 컨트롤러의 회전값을 볼 수 있다.

### 기타

필요하게 된다면 추가할 예정
