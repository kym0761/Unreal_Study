# Unreal_Study/UnrealNetwork

언리얼의 네트워크 프로그래밍은 클라이언트 - 서버 구조이다.

언리얼은 Replicate를 사용해서 서버와 클라가 데이터나 명령 등을 주고받는다.


Replication과 RPC가 정확하게 동작하기 위해서는

1. Actor에서 호출되어야함

2. Actor가 bReplicates = true 여야함

### Replication

언리얼의 Replication 세팅 방법

1. Actor의 bReplicates = true;

2. UPROPERTY(replicated)함

```
UPROPERTY(relicated)
int32 abc;
```

3. void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const; 구현해야함

```
void ASomeActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

  DOREPLIFETIME(ASomeActor, abc);

}
```
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

언리얼 네트워킹에서 알아야할 Role은 다음과 같다
```
Role_Authority - 서버
Role_AutonomousProxy - 해당 객체의 소유자인 클라이언트
Role_SimulatedProxy - 다른 클라이언트가 소유중인 객체의 의미
```
