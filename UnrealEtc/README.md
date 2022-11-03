# Unreal_Study/UnrealC++
 
### Foliage가 거리가 멀어지면 사라지는 이유

1. Foliage Paint로 만들었으면 Foliage mesh 선택창에서 Cull Distance를 확인하기.
2. Landscape Grass Type을 통해 Landscape Material로 Foliage를 만들었다면, Landscape Grass Type의 각 Inst의 Cull Distance 확인하기.
3. 그게 아니면 맵 만든 사람이 따로 만든 커스텀 Culling이 존재하는 것.

### 최적화
1. Texture LOD Bias 값을 건드려서 최대 Texture 사이즈 조절하기.
2. Mesh의 Min LOD를 건드려서 근접시 최대 퀄리티가 나오지 않게 막기.
3. 필요없는 mesh 액터들을 삭제해보기
4. 중복되는 Mesh를 사용하는거라면 Instanced Static Mesh로 여러개 만들기
5. 쓸데없는 라이트가 많은지 확인해볼 것.

다 안됨...? 그럼 로직이 잘못된 걸지도 모름... 코드를 수정해보기.
그게 아니면 걍 언리얼이 무거워서 느린거임 ㅅㄱ
