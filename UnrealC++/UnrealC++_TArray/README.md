# Unreal_Study/UnrealC++_TArray
 
### 문서

https://docs.unrealengine.com/5.1/ko/array-containers-in-unreal-engine/

### emplace vs add

emplace가 성능이 더 좋음. add는 가독성 측면에서 좋음

대부분은 emplace를 쓰는 것을 추천함.

### find

find , findlast 등을 쓸 수 있음

근데 Array에서 find를 쓰는 것은 성능 측면에서 딱히 좋지 않음.

element를 찾기 못했다면 INDEX_NONE이 나옴.


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


### Heapify

최소힙 or 최대힙을 사용해서 최소/최대 값을 빠르게 얻어낼 수 있다.
	
사실 Heap과 PriorityQueue는 같은 기능을 한다.
	
근데 언리얼에 PriorityQueue가 없다. 따로 만들어줘야한다.
	
근데 Heap을 쓸 수 있다면 Heap으로 대체할 수 있으니 Heap쓰는 방법을 알면 된다.
	
	
```
    FORCEINLINE bool operator<(const UPathNode& Other)
    {
        return F_Cost < Other.GetFCost();
    }

     static bool PathFindingPredicated(const UPathNode& A, const UPathNode& B)
    {
        return A.GetFCost() < B.GetFCost();
    }
```

```
	TArray<UPathNode*> openList;

//아래 방법 중에 원하는 걸 사용하면 됨.

	openList.Heapify(); // 이거 안됨.
	
	openList.Heapify(UPathNode::PathFindingPredicated); // 미리 정의된 Predicate를 사용하는 방법
	
	openList.Heapify([](const UPathNode& A, const UPathNode& B) //람다로 Predicate를 정의하는 방법
	{
		return A.GetFCost() < B.GetFCost();
	});
	
	//!! 이 방법을 사용하면 TArray에서 Add할 때 반영이 안된다. 그래서 밑의 HeapSort도 해야함.
	Algo::Heapify(openList, [](UPathNode* A, UPathNode* B)
		{
			return A->GetFCost() < B->GetFCost();
		});
	//추가할 때마다 HeapSort를 써야함.
	Algo::HeapSort(PathNodeList, [](UPathNode* A, UPathNode* B)
		{
			return A->GetFCost() < B->GetFCost();
		});
```
