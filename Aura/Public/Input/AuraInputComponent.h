// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "Input/AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * 
 */



UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	template<typename UserClass, typename PressedCallback, typename ReleasedCallback, typename HeldCallback>
	void BindInputCallbacks(
		const UAuraInputConfig* InputConfig,
		UserClass* Object,
		PressedCallback OnPressed,
		ReleasedCallback OnReleased,
		HeldCallback OnHeld);

};

template<typename UserClass, typename PressedCallback, typename ReleasedCallback, typename HeldCallback>
void UAuraInputComponent::BindInputCallbacks(
	const UAuraInputConfig* InputConfig, 
	UserClass* Object, 
	PressedCallback OnPressed, 
	ReleasedCallback OnReleased, 
	HeldCallback OnHeld)
{
	if (!InputConfig)
	{
		return;
	}

	for (const FAuraInputAction& Action : InputConfig->InputActions)
	{
		if (Action.InputAction && Action.InputActionTag.IsValid())
		{
			if (OnPressed)
			{
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, OnPressed, Action.InputActionTag);
			}
			if (OnReleased)
			{
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, OnReleased, Action.InputActionTag);
			}
			if (OnHeld)
			{
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, OnHeld, Action.InputActionTag);
			}
		}
	}
}
