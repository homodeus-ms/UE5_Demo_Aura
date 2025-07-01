// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraTreasureBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/PlayerInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/Data/TreasureBoxItemsInfo.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"
#include "AuraDefines.h"
#include "Net/UnrealNetwork.h"

AAuraTreasureBox::AAuraTreasureBox()
{	
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("TreasureBoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreasureBoxMesh"));
	Mesh->SetupAttachment(RootComponent);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Mesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	Mesh->MarkRenderStateDirty();
}

void AAuraTreasureBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraTreasureBox, BoxItem);
}

void AAuraTreasureBox::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
		return;

	// TODO: Remove this! For Debugging for a while
	if (bCollectItemForDebugging)
	{
		BoxItem.ItemTag = FAuraGameplayTags::Get().Item_Collecting_AlchemistsFire;
		BoxItem.ItemAmount = 1;
		return;
	}

	if (UTreasureBoxItemsInfo* ItemsInfo = UAuraAbilitySystemLibrary::GetTreasureBoxItemsInfo(this))
	{
		const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
		const TArray<FGameplayTag>& ItemTags = ItemsInfo->ItemTags;
		const int32 ItemsCount = ItemTags.Num();
		const int32 RandItemIndex = FMath::RandRange(0, ItemsCount - 1);  // +5 Is For Missed 

		if (ItemTags.IsValidIndex(RandItemIndex))
		{
			// Random Item Type
			BoxItem.ItemTag = ItemTags[RandItemIndex];

			if (BoxItem.ItemTag.MatchesTagExact(AuraTags.Item_Vacant))
				return;

			// Random Item Count
			int32 RandCount = 1;
			if (BoxItem.ItemTag.MatchesTagExact(AuraTags.Item_Gold))
			{
				RandCount = FMath::RandRange(5, 20);
			}
			else if (BoxItem.ItemTag.MatchesTag(FGameplayTag::RequestGameplayTag("Item.Buyable")))
			{
				RandCount = FMath::RandRange(1, 4);
			}
			else
			{
				RandCount = 1;
			}

			BoxItem.ItemAmount = RandCount;
		}
	}
}

void AAuraTreasureBox::OnRep_BoxItem()
{
	UE_LOG(Aura, Log, TEXT("BoxItem: %s, ItemCount: %d"), *BoxItem.ItemTag.ToString(), BoxItem.ItemAmount);
}


void AAuraTreasureBox::OnSelected()
{
	Mesh->SetRenderCustomDepth(true);
}

void AAuraTreasureBox::OnDeselected()
{
	Mesh->SetRenderCustomDepth(false);
}

void AAuraTreasureBox::OpenBox(AActor* Actor)
{
	UE_LOG(Aura, Log, TEXT("AAuraTreasureBox::OpenBox() By %s"), HasAuthority() ? TEXT("Server") : TEXT("Client"));

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Actor))
	{
		PlayerInterface->RecieveBoxItem(BoxItem.ItemTag, BoxItem.ItemAmount);
	
		Destroy();
	}
}

