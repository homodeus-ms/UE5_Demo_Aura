// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/InventoryItemInfo.h"

FInventoryItem UInventoryItemInfo::FindItemBySpendTag(const FGameplayTag& ItemSpendTag)
{
    for (const FInventoryItem& Item : InventoryItems)
    {
        if (Item.ItemSpendTag.MatchesTagExact(ItemSpendTag))
            return Item;
    }
    return FInventoryItem();
}

FInventoryItem UInventoryItemInfo::FindItemByItemTag(const FGameplayTag& ItemTag)
{
    for (const FInventoryItem& Item : InventoryItems)
    {
        if (Item.ItemTag.MatchesTagExact(ItemTag))
            return Item;
    }
    return FInventoryItem();
}
