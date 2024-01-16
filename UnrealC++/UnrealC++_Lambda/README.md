# Unreal_Study/UnrealC++/UnrealC++_Lambda
 

### lambda 사용하기

```
	hasInteractives.Sort([this](const AActor& a, const AActor& b)
		{
			FVector compLocation = this->GetComponentLocation();

			float distA = FVector::Distance(compLocation, a.GetActorLocation());
			float distB = FVector::Distance(compLocation, b.GetActorLocation());

			return distA < distB;
		});
```

[]는 캡쳐다. this는 this가 캡쳐된다.

& 모든 변수를 Ref로 캡쳐

= 모든 변수를 복사하여 캡쳐

&x, &y, z  : x , y는 Ref 캡쳐, z는 복사 캡쳐 


원래는"[] () -> returnType {}"이다. 예시) [] (const FString& a) -> bool{return true;}

다만 어차피 return ??;을 하면 컴파일러에서 어떤 타입으로 return하는지 파악하기 때문에 생략해도 된다.

TFunctionRef<>사용하면 Parameter로 람다 함수를 넣을 수 있다.
 예시)TFunctionRef<returnType(const param1, const param2, ...)> LambdaParam;

