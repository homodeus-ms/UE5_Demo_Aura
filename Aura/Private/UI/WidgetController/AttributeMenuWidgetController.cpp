
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"
#include "AuraLog.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	check(AuraAttributeSet);

	for (const auto& Pair : AuraAttributeSet->TagsToAttributeMap)
	{
		BroadcastAttributeInfoChangedDelegate(Pair.Key, Pair.Value());
	}
	
	OnAttributePointChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoint());
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	check(AuraAttributeSet);

	for (const auto& Pair : AuraAttributeSet->TagsToAttributeMap)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfoChangedDelegate(Pair.Key, Pair.Value());
			}
		);
	}
	
	AuraPlayerState->OnAttributePointChangedDelegate.AddUObject(this, &ThisClass::AttributePointChangedCallback);
	

}

void UAttributeMenuWidgetController::IncreaseAttribute(const FGameplayTag& AttributeTag)
{
	AuraASC->IncreaseAttribute(AttributeTag);
}


void UAttributeMenuWidgetController::AttributePointChangedCallback(int32 NewValue)
{
	OnAttributePointChangedDelegate.Broadcast(NewValue);
}


void UAttributeMenuWidgetController::BroadcastAttributeInfoChangedDelegate(const FGameplayTag& Tag, const FGameplayAttribute& Attribute)
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoByTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AuraAttributeSet);
	OnAttributeInfoChangedDelegate.Broadcast(Info);
}
