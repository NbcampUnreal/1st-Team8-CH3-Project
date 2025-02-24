#include "Items/UtiliyItem/ItemBase.h"


AItemBase::AItemBase()
	: ItemName("")
{
}

void AItemBase::DestroyItem()
{
	Destroy();
}
