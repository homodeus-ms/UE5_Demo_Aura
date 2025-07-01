// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/BuyableItem.h"
#include "Interface/PlayerInterface.h"
#include "AuraLog.h"

void ABuyableItem::OnOverlap(AActor* TargetActor)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(TargetActor))
	{
		PlayerInterface->GetItem(ItemSpendTag);

		Destroy();
	}
}

void ABuyableItem::OnEndOverlap(AActor* TargetActor)
{

}

void ABuyableItem::Spend(AActor* Target)
{ 
	ApplyEffectToTarget(Target, InstantEffect.EffectClass);
}


