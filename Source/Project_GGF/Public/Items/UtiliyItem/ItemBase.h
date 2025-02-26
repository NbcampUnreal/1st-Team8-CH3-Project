#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Project_GGF/Public/Items/Manager/ItemDataManager.h"
#include "ItemBase.generated.h"

UCLASS()
class PROJECT_GGF_API AItemBase : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSubclassOf<AItemDataManager> ItemDataManagerClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	AItemDataManager* ItemDataManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemDataType ItemType;

public:
	FName GetItemName() { return ItemName; }

	virtual void DestroyItem();

public:	
	AItemBase();

};