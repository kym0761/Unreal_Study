# Unreal_Study/UnrealNetwork

언리얼의 네트워크 프로그래밍은 클라이언트 - 서버 구조이다.   
언리얼은 Replication을 사용해서 서버와 클라가 데이터나 명령 등을 주고받는다.   


Replication과 RPC가 정확하게 동작하기 위해서는

1. Actor에서 호출되어야함   
2. Actor가 bReplicates = true 여야함   

### Replication

언리얼의 Replication 세팅 방법

1. Actor의 bReplicates = true;

2. UPROPERTY(replicated)

```
UPROPERTY(relicated)
int32 abc;
```

3. void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; 구현

```
void ASomeActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

  DOREPLIFETIME(ASomeActor, abc); //#include "Net/UnrealNetwork.h"

}
```

### ReplicatedUsing

실시간으로 값을 계속 갱신하는 방식인 위의 방식보다는, ReplicatedUsing을 사용해 값이 변경됐을 때만 갱신하는 방식이 더 효율적이다.

1. UPROPERTY(ReplicatedUsing = 함수이름)
```
UPROPERTY(ReplicatedUsing = OnRep_Func)
int32 abc;
```

2. OnRep_함수 구현

```
UFUNCTION()
void OnRep_Func();
```

언리얼 엔진 내에서 abc의 값이 변경되면 OnRep이 호출될 것이다.
OnRep은 명시적 호출도 가능하다.(서버에선 명시적 호출을 하지 않으면 동작하지 않음.)
OnRep은 서버가 아닌 클라이언트에서만 호출되는 점을 알아둬야한다.

#### 네트워크에서의 C++ vs 블루프린트

C++에서는 ReplicatedUsing, 블루프린트에서는 RepNotify가 있다.

C++은 클라이언트에서만 호출된다.
명시적 호출이 가능하다.
값이 변경될 때만 호출이 된다.

블루프린트는 서버, 클라이언트 각각 호출된다.
명시적 호출이 불가능하게 막혀있다.
서버에서는 값이 변경되지 않았어도 계속 호출된다.
클라이언트는 값이 변경되었을 때만 호출된다.


### RPC (Remote Procedure Call)

UFUNCTION()에 Server, Client, NetMulticast 등의 Specifier를 넣어주면 RPC가 된다.
기본은 Unreliable임. 신뢰성을 위해서 Reliable을 붙이면 확실하게 실행된다.

```
UFUNCTION(Client)
void Func();

이 함수는 서버에서 Call 하지만, Execution은 클라이언트에서 함.
```
```
UFUNCTION(Server)
void Func();

이 함수는 클라이언트에서 Call 하지만, Execution은 서버에서 함.
```
```
UFUNCTION(NetMulticast)
void Func();

이 함수는 2가지 상황임
1. 서버에서 Call함. 서버에서 Call한뒤 서버와 클라이언트 둘 다 Execution함.
2. 어느 클라이언트에서 Call함. 그러면 Call한 클라이언트에서만 Execution함.
```
### Role

언리얼 네트워킹에서 알아야할 Role은 다음과 같다.
```
Role_Authority - 신뢰할 수 있는 게임 역할로, 게임 로직을 수행한다.
Role_AutonomousProxy - authority를 가진 복제품인데, 일부 게임 로직을 수행할 수 있다.
Role_SimulatedProxy - authority를 가진 복제품이지, 게임 로직을 전혀 수행하지 않는다. 서버의 변경사항만 반영함.
```

autonomous는 클라이언트의 입력 정보를 서버에 보내는 능동적 역할을 수행한다.   
simulated는 일방적으로 서버로부터 데이터를 받아 이를 반영하는 역할이다.   
autonomous 역할을 하는 액터는 보통 플레이어 컨트롤러와 폰이다.   

클라이언트의 플레이어 컨트롤러와 pawn은 autonomous 역할을 받는다.
클라이언트가 보유하지 않은 액터(월드에 배치된 Actor?)와 다른 클라이언트의 pawn은 simulated 역할을 받는다.

UI나 Animation 블루프린트 등은 클라이언트에서만 동작한다.   

언리얼 네트워킹에서 액터는 "로컬 역할" / "리모트 역할" 두가지를 가지게 된다.   
로컬 역할은 동작하고 있는 게임 프로그램에서의 역할,   
리모트 역할은 연결되어 있는 게임 프로그램에서의 역할이다.   


```
예시 : 서버를 중심으로 Star 형식의 네트워크를 구성했다고 생각하면 편하다.

리슨 서버를 처음 가동해서 리슨 서버의 캐릭터는 Authority / Simulated 이다.
서버의 액터이므로 Local은 Authority로 모든 권한을 가진다
서버 외의 클라이언트에서는 리슨 서버 캐릭터의 권한은 전부 Simulated이다.

클라이언트1이 가동되어 서버와 연결되고 캐릭터가 생성된다.
클라이언트1의 pawn이므로 Local은 Autonomous다.
서버에 연결되어 있으므로 리모트 역할은 authority다.

이 클라이언트1의 캐릭터는 서버에서는 authority / autonomous다.

클라이언트2가 가동되어 서버와 연결되고 캐릭터가 생성된다.
클라이언트2의 pawn이므로 local은 autonomous다.
서버와 연결되어 있으므로 remote는 authority다.

서버에서 클라이언트 2의 캐릭터는 authority / autonomous다.

그러나 클라이언트 1에서는 클라이언트2의 캐릭터는 simulated / authority다.
클라이언트2에서도 클라1의 캐릭터는 simulated / authority다.
```


서버를 기준으로, 서버가 소유한 모든 actor는 Role_Autority라는 로컬 역할을 가진다.
액터에 따라 None, AutonomousProxy, SimulatedProxy 3가지의 리모트 역할을 가질 수 있다.   
None은 클라이언트가 보유하지 않을 GameMode 같은 Actor다.

클라이언트를 기준으로, 클라이언트 안의 액터는 AutonomousProxy, SimulatedProxy 2개의 로컬 역할을 가진다.
모든 클라이언트 안의 액터는 리모트 역할로 Authority 역할을 가진다.

서버에 클라이언트가 접속했을 때 서버 안에서는, 플레이어 컨트롤러가 처음 생성될 때는 None이지만, Login을 거치면서 Autonomous로 바뀐다.
캐릭터가 생성됐을 때는 최초에는 Simulated지만, Possess를 거치면서 Autonomous로 바뀐다.



#### API

HasAuthority()

IsLocalController()

IsLocallyControlled()

GetLocalRole()

GetRemoteRole()

### Replication 빈도

NetUpdateFrequency : 리플리케이션 빈도 최대치 설정

1초에 몇번 리플리케이션을 "시도"할 지 지정하는 값.
기본은 100이다. 1/100초마다 서버가 리플리케이션을 시도한다.

이 빈도 값은 최대치일 뿐, 횟수가 보장되는 것은 아니다.
서버의 Tick Rate에 따라 리플리케이션이 발생하기 때문에 서버의 성능에 따라 달라진다.
서버의 성능이 NetFrequency 빈도보다 낮으면, 서버의 성능만큼만 리플리케이션한다.

일반적으로는 그래픽 기능이 없는 데디케이티드 서버가 더 좋은 성능을 발휘한다.

### 액터 연관성

플레이어의 시야에 보이는 정도의 액터를 연관이 있다고 파악하고 필요한 만큼만 네트워크를 사용하도록 하여 네트워크 사용량을 줄이는 목표

AlwaysRelevant : 항상 커넥션에 연관성을 가짐
NetUseOwnerRelevancy : 자신의 연관성은 판단 주체를 Owner에게 넘김
OnlyRelevantToOwner : 오너에 대해서만 연관성을 가짐 (카메라와 캐릭터에 연관이 없다면 클라이언트 쪽에 정보 보내지 않음.)
NetCullDistance : 뷰어와 거리에 따라 연관성 여부 판정 / 기본값이 큰데, 이 값은 제곱값으로 되어 있으므로 실제로 기본값이 2억2500만 값은 15000cm로 판정된다.

프로젝트에 따라 연관성을 적절하게 필요한 방식을 사용자가 설정해주면 좋다.

#### IsNetRelevantFor()

연관성이 있는지 확인하는 함수

### Lowlevel 패킷?

### 액터의 우선권

기본적인 NetPriority
Actor 1.0
Pawn 3.0
PlayerController 3.0

마지막 패킷을 보낸 후의 경과 시관과 최초 우선권 값을 곱해 최종 우선권 값을 계산한다.
최종 우선권 값을 사용해 클라이언트에 보낼 액터 목록을 정렬함
네트워크 포화될 때까지 정렬된 순서로 리플리케이션 수행
네트워크 포화면, 해당 액터는 다음 서버틱에서 넘겨줌.

GetNetPriority() 상의 가중치
현재 뷰타겟인 경우 : 4배
잘 보이는 경우 : 2배
잘 안보임 : 0.2~0.4
플레이어 컨트롤러가 뷰어인 경우 : 4배

### 액터 휴면(Dormancy)

액터의 전송을 최소화하기 위해 연관성과 더불어 제공되는 속성으로
액터가 휴면상태면 연관성이 있어도, 액터리플리케이션 & RPC를 수행하지 않음.

DORM_Never - 휴면하지 않음
DORM_AWake - 깨어 있음
DORM_DormantAll - 언제나 휴면 상태로, 필요시에 깨울 수 있음
DORM_DormantPartial - 특정 조건을 만족할 경우엔 리플리케이션을 수행
DORM_Initial - 액터를 휴면 상태로 시작하고 필요할 때 깨우도록 설정

속성 리플리케이션에 사용시에는 DORM_Initial만 고려하는 것이 좋음.

### 조건식 프로퍼티 리플리케이션

구글에 문서 검색하여 참고

프로퍼티가 일단 리플리케이션 등록이 되면 해제할 수 없다. 가급적이면 많은 정보를 구워 한번에 처리하는 것이 성능에 유리하기 때문이다. 특정 조건에서 리플리케이트 여부에 따라 할지 안할지 조정해야한다.

DOREPLIFETIME_CONDITION()

### Consider List

언리얼 엔진 내에서 액터 우선순위와 휴면 상태 등을 파악해서 네트워크로 데이터를 전송할 때 전송이 필요한 목록을 만들어 분석하여 액터의 리플리케이션을 진행한다.

### RPC

UFUNCTION()에 Client, Server, NetMulticast 등의 키워드를 붙이면 RPC가 될 수 있다.
Reliable Unreliable 키워드도 잊지 말아야함.
실제 동작은 [RPC 이름]_Implementation() 함수에서 일어남
[RPC 이름]_WithValidation은 서버 RPC에서 꼭 필요함.

