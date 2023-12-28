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
OnRep은 명시적 호출도 가능하다.
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

