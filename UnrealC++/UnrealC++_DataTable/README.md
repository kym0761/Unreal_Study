# Unreal_Study/UnrealC++/UnrealC++_DataTable
 


### DataTable에 접근에 데이터 받아오기

```
			//Find Data from RowName;
			FSoldierRankTable* rowTable = soldierRankDataTable->FindRow<FSoldierRankTable>(rowName, TEXT(""));
			if (rowTable)
			{
				//UE_LOG(LogTemp, Log, TEXT("%s --- %d"), *rowTable->Rank, rowTable->Exp);
				RankName = rowTable->Rank;
				ExpToNeed = rowTable->Exp;
			}
```
