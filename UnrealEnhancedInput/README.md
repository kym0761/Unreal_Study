# Unreal_Study/UnrealEnhancedInput
 
해당 내용은 Blueprint를 기반으로 설명함.

C++은 Engine/Plugins/Enhanced Input 을 참고하여 볼 것.

어차피 비슷한 내용일 것이므로, 이를 참고하여 C++화 하든지 말든지...?

### Input Action

Input에 대한 어떠한 행동을 정할 수 있음.

bool, float, Vector2D, Vector3D로 가능함.

Trigger나 Modifier를 정할 수 있는데, 지금은 모르겠음.

### Input Mapping Context

Input Action에 맞는 키를 설정해주는 역할을 함.

예시로 Input_Move의 경우 WASD로 키를 정할 수가 있음.

해당 키의 Input Modifier를 설정해주면, Modifier에 맞게 행동할 수 있음.

### Input Modifier

키의 값에 따라 어떤 Input result가 나오게 하는 역할을 함.

ModifyRaw()를 Override하여 Input Action Value를 return해야함.

```
예시를 들자면,

BP ThirdPersonCharacter를 보면, Forward/Backward를 Y의 값으로 움직이도록 해놨음.
IM_Move_W의 경우 X값을 Input Action Value의 Y에 들어감.
IM_Move_S의 경우 X값을 -1을 곱한 뒤, Input Action Value의 Y에 들어감.

참고로, 키보드의 Value는 1개(Scalar)만 나오므로, X Y Z가 있어도 X값만 나옴.
그러니, 위의 Move_W라는 Input Modifier는 X의 값을 뽑아서 Y에 꽂아야함.

```

### 실제 Input을 캐릭터에 Bind하는 방법.

실제 조종해야할 BP_Character의 블루프린트를 들어간다.

Beginplay()에서 Player Controller를 접근한 뒤, GetEnhancedInputLocalPlayerSubsystem()를 하여 Enhanced Input에 대한 Subsystem을 찾는다.

EnhancedInputLocalPlayerSubsystem의 AddMappingContext() 한 뒤, Mapping Context를 만든 Input Mapping Context를 연결해준다.

이후, 만들어놨던 Input Action들에 대한 행동을 기존에 존재했던, Axis&Action 방식대로 구현해주면 된다.

```
Input Action 의 Event 설명

Trigger - 조종하는 동안 Tick마다 1 or More로 동작.
Started - 최초 한번. 기존의 Pressed임.
OnGoing - 계속 동작하는 동안, 예를 들면 마우스 버튼 누른 상태에서 안 놓고 계속.
Canceled - Trigger가 캔슬됐을 때... 라는데 사실 잘 모르겠음. 아마 UI상에서 버튼 누른 상태로 마우스 호버링 벗어난 뒤에 땠을 때 상황을 표현한 게 아닌가 추정함.
Completed - 끝났을 때, 기존의 Released임.

Action Value - bool, float, Vector2D, Vector3D

Input Action - Input Action UObject.

확대시 나오는 변수
Elapsed Seconds
Triggered Seconds

```
