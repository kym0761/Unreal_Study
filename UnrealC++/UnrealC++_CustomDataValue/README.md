# Unreal_Study/UnrealC++/UnrealC++_CustomDataValue

### Instanced Static Mesh의 Custom Data Value

Instanced Static Mesh Component 전용의 Material을 만들었다면

Custom Data Value를 세팅하여 Instance마다 다른 Material 효과가 나게 설정할 수 있음.

먼저 ISM_Component의 NumCustomDataFloats를 필요한 Data Value 수만큼 세팅함.
SetCustomDataValue(instance Index , Custom Data Value Index, Value(float)); 로 세팅.

```
예시)

UInstancedGridVisualComponent::UInstancedGridVisualComponent()
{
	//사용가능한 Material의 Custom Data Value의 최대 갯수
	NumCustomDataFloats = 8;
}

void UInstancedGridVisualComponent::DrawGridVisualswithGridSet(const TSet<FGrid>& GridSet, const float Height)
{
...
		FGrid upGrid = currentGrid + FGrid(0,1);
		float upValue = GridSet.Contains(upGrid) ? 0.0f : 1.0f;
		SetCustomDataValue(i, 0, upValue);
...
}
```

