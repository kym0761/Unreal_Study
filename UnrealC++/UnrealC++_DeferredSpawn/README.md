# Unreal_Study/UnrealC++/UnrealC++_DeferredSpawn
 

### Deferred Spawn

```
ADamageTextActor* damageTextActor = GetWorld()->SpawnActorDeferred<ADamageTextActor>(DamageTextActorClass, GetActorTransform());
if (damageTextActor)
{
	damageTextActor->AppliedDamage = 0.0f;
	damageTextActor->FinishSpawning(GetActorTransform());
}
```
Deferred Spawn은 Spawn 전에 변수가 바뀌거나 Init 등의 처리를 할 시간을 가진 뒤에 Spawn 완료 처리를 위한 지연 Spawn이다.

GetWorld()->SpawnActorDeferred를 사용

초기화가 끝난다면, FinishSpawning을 해야함.


