// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Data/InventoryItemInfo.h"
#include "AbilitySystem/Data/CollectingItemInfo.h"
#include "Interface/ConsumableInterface.h"
#include "Interface/PlayerInterface.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"
#include "DedicatedServer/Public/Save/AuraSaveData.h"
#include "DedicatedServer/Public/Interfaces/SaveManageInterface.h"

AAuraPlayerState::AAuraPlayerState()
{
    ASC = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("ASC"));
    ASC->SetIsReplicated(true);
    ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
    NetUpdateFrequency = 100.f;

    AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
    return ASC;
}


void AAuraPlayerState::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
    if (!HasAuthority())
        return;

    if (IsValid(LevelUpInfo))
    {
        MaxLevel = LevelUpInfo->GetMaxLevel();
    }

    for (const auto& Pair : InventoryItems)
    {
        InventoryItemArray.Add(FInventoryItemEntry(Pair.Key, Pair.Value));
    }
}

void AAuraPlayerState::BeginPlay()
{
    Super::BeginPlay();
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AAuraPlayerState, PlayerLevel);
    DOREPLIFETIME(AAuraPlayerState, XP);
    DOREPLIFETIME(AAuraPlayerState, AttributePoint);
    DOREPLIFETIME(AAuraPlayerState, SpellPoint);
    DOREPLIFETIME(AAuraPlayerState, Gold);
    DOREPLIFETIME(AAuraPlayerState, InventoryItemArray);
    DOREPLIFETIME(AAuraPlayerState, AddedCollectingItem);
}

UAttributeSet* AAuraPlayerState::GetAttributeSet() const
{
    return AttributeSet;
}

void AAuraPlayerState::LoadNewPlayerDatas()
{
    UE_LOG(Aura, Log, TEXT("AAuraPlayerState::LoadNewPlayerDatas()"));

    if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetPlayerController()->GetPawn()))
    {
        PlayerInterface->LoadPlayerData(true);
        PlayerInterface->SaveCurrentGame(DefaultMapName, DefaultPlayerStartName);
        SaveManageInterface->SetNotANewPlayer(this);
    }
}

void AAuraPlayerState::OnRetrieveData(UAuraSaveData* InSaveData)
{
    UE_LOG(Aura, Log, TEXT("AAuraPlayerState::OnRetrieveData"));

    SaveData = InSaveData;

    if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetPlayerController()->GetPawn()))
    {
        PlayerInterface->LoadPlayerData();
    }
}

void AAuraPlayerState::AddXP(int32 EarnXP)
{
    if (!HasAuthority())
        return;
    
    XP += EarnXP;
   
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::UpdateLevel(int32 NewLevel)
{
    if (!HasAuthority())
        return;
    
    UpdateEarnPointsAndBroadcast(PlayerLevel, NewLevel);

    PlayerLevel = NewLevel;

    OnLevelChangedDelegate.Broadcast(NewLevel, true);
}

void AAuraPlayerState::AddAttributePoint(int32 AddPoint)
{
    AttributePoint += AddPoint;
    OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::AddSpellPoint(int32 AddPoint)
{
    SpellPoint += AddPoint;
    OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void AAuraPlayerState::AddGold(int32 InGold)
{
    if (InGold < 0 && Gold + InGold < 0)
    {
        UE_LOG(Aura, Warning, TEXT("The Result Of Adding Gold Is Negative, %s"), __FUNCTION__);
        return;
    }

    Gold += InGold;
    // OverFlow
    Gold = Gold >= 0 ? Gold : MAX_int32;

    OnGoldChangedDelegate.Broadcast(Gold);
}

void AAuraPlayerState::SpendItem(const FGameplayTag& ItemSpendTag)
{
    if (!HasAuthority())
        return;

    int32 CurrentItemCount = GetInventoryItemCount(ItemSpendTag);

    if (InventoryItems.Contains(ItemSpendTag) && CurrentItemCount > 0)
    {
        const FInventoryItem& Item = InventoryItemInfo->FindItemBySpendTag(ItemSpendTag);

        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        AActor* ItemActor = GetWorld()->SpawnActor<AActor>(Item.ItemClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
        
        if (IsValid(ItemActor))
        {
            if (IConsumableInterface* ConsumableInterface = Cast<IConsumableInterface>(ItemActor))
            {
                ConsumableInterface->Spend(GetPawn());
                AddInventoryItemByItemSpendTag(ItemSpendTag, -1);
            }

            ItemActor->Destroy();
        }
    }
}

void AAuraPlayerState::BuyItem(const TArray<TPair<FGameplayTag, int32>>& Items, int32 SpentGold)
{
    bool ItemAdded = true;
    for (const auto& Pair : Items)
    {
        ItemAdded = AddInventoryItemByItemSpendTag(Pair.Key, Pair.Value);
    }

    if (ItemAdded)
    {
        AddGold(SpentGold);
    }
    else
    {
        UE_LOG(Aura, Warning, TEXT("Item Not Added!, %s"), *FString(__FUNCTION__));
    }
}

void AAuraPlayerState::SetPlayerLevelByLoadedData(int32 Level)
{
    PlayerLevel = Level;
    OnLevelChangedDelegate.Broadcast(Level, false);
}
void AAuraPlayerState::SetXPByLoadedData(int32 InXP)
{
    XP = InXP;
    OnXPChangedDelegate.Broadcast(InXP);
}

void AAuraPlayerState::SetGoldByLoadedData(int32 InGold)
{
    Gold = InGold;
    OnGoldChangedDelegate.Broadcast(InGold);
}

void AAuraPlayerState::SetInventoryItems(const TMap<FGameplayTag, int32>& Items)
{
    if (!HasAuthority())
        return;

    InventoryItems = Items;

    for (const auto& Pair : Items)
    {
        FInventoryItemEntry NewEntry(Pair.Key, Pair.Value);
        if (InventoryItemArray.Contains(NewEntry))
        {
            int32 Index = InventoryItemArray.Find(NewEntry);
            if (Index != INDEX_NONE)
            {
                InventoryItemArray[Index].ItemCount = Pair.Value;
            }
        }
        else
        {
            InventoryItemArray.Add(NewEntry);
        }
    }

    for (const FInventoryItemEntry& Entry : InventoryItemArray)
    {
        OnInventoryChangedDelegate.Broadcast(Entry.ItemTag, Entry.ItemCount);
    }
}

void AAuraPlayerState::SetCollectionsByLoadedData(const TArray<FGameplayTag>& InCollections)
{
    Collections = InCollections;

    Client_SetCollectionsByLoadedData(Collections);

    for (const FGameplayTag& ItemTag : InCollections)
    {
        OnGetItemFromTreasureBox.Broadcast(ItemTag, 1, true);
    }
}

void AAuraPlayerState::Server_PurchaseItem_Implementation(const TArray<FInventoryItemEntry>& Items, int32 TotalPrice)
{
    AddGold(-TotalPrice);

    for (const auto& Entry : Items)
    {
        const FGameplayTag& ItemTag = Entry.ItemTag;

        if (InventoryItems.Contains(ItemTag))
        {
            if (InventoryItems[ItemTag] != Entry.ItemCount)
            {
                AddInventoryItemByItemSpendTag(ItemTag, Entry.ItemCount);
            }
        }
    }
}

bool AAuraPlayerState::AddInventoryItemByItemSpendTag(const FGameplayTag& ItemSpendTag, int32 InAmount)
{
    UE_LOG(Aura, Log, TEXT("AAuraPlayerState::AddInventoryItemByItemSpendTag()"));
    UE_LOG(Aura, Log, TEXT("ItemTag: %s"), *ItemSpendTag.ToString());
    UE_LOG(Aura, Log, TEXT("AddAmount: %d"), InAmount);

    if (!HasAuthority())
        return false;

    if (!InventoryItems.Contains(ItemSpendTag))
        return false;

    int32 CurrentItemCount = InventoryItems[ItemSpendTag];
    if (CurrentItemCount + InAmount < 0)
    {
        UE_LOG(Aura, Warning, TEXT("Inventory Amount Is NOT Valid, %s"), *FString(__FUNCTION__));
        return false;
    }

    InventoryItems[ItemSpendTag] = CurrentItemCount + InAmount;
    UE_LOG(Aura, Log, TEXT("InventoryItems[ItemSpendTag]: %d"), InventoryItems[ItemSpendTag]);

    for (FInventoryItemEntry& Entry : InventoryItemArray)
    {
        if (Entry.ItemTag.MatchesTagExact(ItemSpendTag))
        {
            Entry.ItemCount += InAmount;
            if (OnInventoryChangedDelegate.IsBound())
            {
                OnInventoryChangedDelegate.Broadcast(ItemSpendTag, Entry.ItemCount);
            }
            break;
        }
    }

    return true;
}

void AAuraPlayerState::AddInventoryItemFromTreasureBox(const FGameplayTag& ItemTag, int32 InAmount)
{
    UE_LOG(Aura, Log, TEXT("AAuraPlayerState::AddInventoryItemFromTreasureBox()"));
    UE_LOG(Aura, Log, TEXT("ItemTag: %s"), *ItemTag.ToString());
    UE_LOG(Aura, Log, TEXT("ItemAmount: %d"), InAmount);

    const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

    if (ItemTag.MatchesTagExact(AuraTags.Item_Gold))
    {
        AddGold(InAmount);
    }
    else
    {
        const FGameplayTag& SpendTag = InventoryItemInfo->FindItemByItemTag(ItemTag).ItemSpendTag;
        AddInventoryItemByItemSpendTag(SpendTag, InAmount);
    }

    OnGetItemFromTreasureBox.Broadcast(ItemTag, InAmount, false);
    Client_OnGetItemFromTreasureBox(ItemTag, InAmount);
}

void AAuraPlayerState::AddCollectingItem(const FGameplayTag& ItemTag)
{
    UE_LOG(Aura, Log, TEXT("AAuraPlayerState::AddCollectingItem, Input: %s"), *ItemTag.ToString());

    if (!HasAuthority())
        return;

    const TArray<FCollectingItem>& Items = CollectingItemInfo->GetItemDataArray();
    const int32 Index = FMath::RandRange(0, Items.Num() - 1);
    const FCollectingItem& Item = Items[Index];

    if (!HasCollectingItemAlready(Item.ItemTag))
    {
        UE_LOG(Aura, Log, TEXT("NOT Have This Item!"));
        
        AddedCollectingItem = Item.ItemTag;

        Collections.Add(Item.ItemTag);
        
        OnGetItemFromTreasureBox.Broadcast(Item.ItemTag, 1, false);
    }
    else
    {
        UE_LOG(Aura, Log, TEXT("Already Have This Item!"));
    }
}

void AAuraPlayerState::OnRep_XP()
{
    OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_PlayerLevel()
{
    const bool bShowLevelUpMessagee = PrevLevel != -1 && PrevLevel < PlayerLevel;
    OnLevelChangedDelegate.Broadcast(PlayerLevel, bShowLevelUpMessagee);
    PrevLevel = PlayerLevel;
}

void AAuraPlayerState::OnRep_AttributePoint()
{ 
    OnAttributePointChangedDelegate.Broadcast(AttributePoint);
}

void AAuraPlayerState::OnRep_SpellPoint()
{
    OnSpellPointChangedDelegate.Broadcast(SpellPoint);
}

void AAuraPlayerState::OnRep_Gold()
{
    OnGoldChangedDelegate.Broadcast(Gold);
}

void AAuraPlayerState::OnRep_InventoryItemArray()
{
    UE_LOG(Aura, Log, TEXT("=== AAuraPlayerState::OnRep_InventoryItemArray() ==="));
    UE_LOG(Aura, Log, TEXT("InventoryItemArray.Num(): %d"), InventoryItemArray.Num());

    for (const FInventoryItemEntry& Entry : InventoryItemArray)
    {
        UE_LOG(Aura, Log, TEXT("{ Key: %s, Value: %d }"), *Entry.ItemTag.ToString(), Entry.ItemCount);
    }
    UE_LOG(Aura, Log, TEXT("End Of Array Check"));

    for (const FInventoryItemEntry& Entry : InventoryItemArray)
    {
        int32 RepItemCount = Entry.ItemCount;

        if (InventoryItems.Contains(Entry.ItemTag))
        {
            if (RepItemCount == InventoryItems[Entry.ItemTag])
                continue;
        }
        else
        {
            UE_LOG(Aura, Warning, TEXT("InventoryItems Old Map Key Not Matched With Replicated Array: %s"), *FString(__FUNCTION__));
            return;
        }

        InventoryItems[Entry.ItemTag] = RepItemCount;

        UE_LOG(Aura, Log, TEXT("--- Check Array And Map Value ---"));
        UE_LOG(Aura, Log, TEXT("ItemTag: %s"), *Entry.ItemTag.ToString());
        UE_LOG(Aura, Log, TEXT("RepItemCount: %d"), RepItemCount);
        UE_LOG(Aura, Log, TEXT("ItemCountInMap: %d"), InventoryItems[Entry.ItemTag]);

        OnInventoryChangedDelegate.Broadcast(Entry.ItemTag, Entry.ItemCount);
    }
}

void AAuraPlayerState::OnRep_AddedCollectingItem()
{
    OnGetItemFromTreasureBox.Broadcast(AddedCollectingItem, 1, false);
}

void AAuraPlayerState::Client_OnGetItemFromTreasureBox_Implementation(const FGameplayTag& ItemTag, int32 InAmount)
{
    OnGetItemFromTreasureBox.Broadcast(ItemTag, InAmount, false);
}

void AAuraPlayerState::Client_SetCollectionsByLoadedData_Implementation(const TArray<FGameplayTag>& InCollections)
{
    Collections = InCollections;

    for (const FGameplayTag& Tag : Collections)
    {
        OnGetItemFromTreasureBox.Broadcast(Tag, 1, true);
    }
}

void AAuraPlayerState::UpdateEarnPointsAndBroadcast(int32 OldLevel, int32 NewLevel)
{
    int32 AttributePointIncAmount = 0;
    int32 SpellPointIncAmount = 0;
    
    do
    {
        const FAuraLevelUpInfo& Info = LevelUpInfo->GetLevelUpInfo(++OldLevel);
        AttributePointIncAmount += Info.AttributePoint;
        SpellPointIncAmount += Info.SpellPoint;
    } while (OldLevel < NewLevel);

    AddAttributePoint(AttributePointIncAmount);
    AddSpellPoint(SpellPointIncAmount);
}


int32 AAuraPlayerState::GetInventoryItemCount(const FGameplayTag& ItemTag) const
{
    if (InventoryItems.Contains(ItemTag))
    {
        return InventoryItems[ItemTag];
    }

    UE_LOG(Aura, Log, TEXT("InventoryItems Don't have such ItemTag"));
    return -1;
}


bool AAuraPlayerState::HasCollectingItemAlready(const FGameplayTag& ItemTag)
{
    for (const FGameplayTag& Tag : Collections)
    {
        if (Tag.MatchesTagExact(ItemTag))
            return true;
    }
    return false;
}

bool AAuraPlayerState::IsLevelUpCondition(int32 NewXP)
{
    int32 CurrentMaxXP = LevelUpInfo->GetCurrentLevelMaxXP(PlayerLevel);
    return (XP + NewXP) > CurrentMaxXP && PlayerLevel < MaxLevel;
}

int32 AAuraPlayerState::GetNewLevelForXP(int32 InXP)
{
    return LevelUpInfo->FindLevelForXP(InXP);
}








