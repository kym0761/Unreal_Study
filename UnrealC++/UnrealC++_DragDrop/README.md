# Unreal_Study/UnrealC++/UnrealC++_DragDrop
 


### 인벤토리 DragDrop 구현

```
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, 
		const FPointerEvent& InMouseEvent,
		UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry,
		const FDragDropEvent& InDragDropEvent,
		UDragDropOperation* InOperation) override;
```

NativeOnDragDetected에서는 DragDisplay와 DragDropOperation을 만들어줘야한다.

```
	//Create DragDisplayUI
	UItemSlot* dragDisplay = CreateWidget<UItemSlot>(GetOwningPlayer(), ItemSlotClass);

	dragDisplay->InventoryComponentRef = InventoryComponentRef;
	dragDisplay->InventorySlotNumber = InventorySlotNumber;
	dragDisplay->UpdateItemSlot();

	//Make DragDropEvent And Assign it.
	UDragDropOperation* dragDropOper = NewObject<UDragDropOperation>();
	dragDropOper->Payload = this;
	dragDropOper->DefaultDragVisual = dragDisplay;
	dragDropOper->Pivot = EDragPivot::CenterCenter;
```

NativeOnDrop에서는 UDragDropOperation* InOperation에서  InOperation->Payload를 위의 DragDisplay에 사용된 Widget으로 Casting하여 기능을 만든다.

```
			UItemSlot* droppedItemSlot = Cast<UItemSlot>(InOperation->Payload);

			//drop의 인벤토리 정보를 받아옴.
			int32 drop_InventorySlotNumber = droppedItemSlot->InventorySlotNumber;
			UInventoryComponent* drop_InventoryComponent = droppedItemSlot->InventoryComponentRef;
```


