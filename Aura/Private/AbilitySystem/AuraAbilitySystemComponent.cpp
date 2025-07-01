// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "AuraLog.h"
#include "GameplayTagContainer.h"
#include "Interface/PlayerInterface.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/GA/AuraPassiveGABase.h"
#include "SaveGame/LoadScreenSaveGame.h"
#include "DedicatedServer/Public/Save/AuraSaveData.h"
#include "AuraLog.h"
#include "DedicatedServer/Public/Save/SaveHelper.h"

// TEMP
#include "GameplayTagsManager.h"

UAuraAbilitySystemComponent::UAuraAbilitySystemComponent()
{
	
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &ThisClass::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability, 1);

		GiveAbility(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AddPassiveStartupAbilities(const TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
	if (!AbilityInfo)
		return;

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();

	for (const TSubclassOf<UGameplayAbility> Ability : Abilities)
	{
		FGameplayAbilitySpec AbilitySpec(Ability, 1);
		const FGameplayTag& AbilityTag = GetAbilityTagFromAbilitySpec(AbilitySpec);
		int32 RequirementLevel = AbilityInfo->FindAbilityByAbilityTag(AbilityTag).LevelRequirement;

		if (RequirementLevel > 1)
		{
			GiveAbility(AbilitySpec);
		}
		else
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraTags.Abilities_Status_Equipped);
			GiveAbilityAndActivateOnce(AbilitySpec);
		}
	}
}

void UAuraAbilitySystemComponent::AddAbilitiesWithTag(const TArray<TSubclassOf<class UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& GAClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(GAClass, 1);
		AbilitySpec.Ability = GAClass->GetDefaultObject<UGameplayAbility>();

		if (const UAuraGABase* AuraGA = Cast<UAuraGABase>(AbilitySpec.Ability))
		{
			const FGameplayTag& InputTag = AuraGA->StartupInputTag;
			AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Abilities_Status_Equipped);

			FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);
			UpdateSlotToEquippedSpecMap(InputTag, SpecHandle);
		}
	}
	bStartupAbilitySet = true;
	OnStartupAbilitySetDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddAbilitiesWithTag(FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		const FGameplayTag& InputTag = GetSlotTagFromAbilitySpec(AbilitySpec);
		const FGameplayTag& StatusTag = GetStatusTagFromAbilitySpec(AbilitySpec);
		
		AbilitySpec.DynamicAbilityTags.AddTag(InputTag);
		AbilitySpec.DynamicAbilityTags.AddTag(StatusTag);

		FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);

		if (InputTag.IsValid() && StatusTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Status_Equipped))
			UpdateSlotToEquippedSpecMap(InputTag, SpecHandle);

		MarkAbilitySpecDirty(AbilitySpec);
		const FGameplayTag& AbilityTag = GetAbilityTagFromAbilitySpec(AbilitySpec);
		ClientEquipSkill(AbilityTag, StatusTag, InputTag, FGameplayTag());
	}
}

void UAuraAbilitySystemComponent::AddAbilityOnLevel(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
	if (!AbilityInfo)
		return;

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilityInfos)
	{
		if (!Info.AbilityTag.IsValid() || Level < Info.LevelRequirement)
			continue;

		if (GetSpecFromActivatableAbilitiesByTagOrNull(Info.AbilityTag) == nullptr)
		{
			// The ability has not been added to ASC yet, 
			// but the required level has been reached, 
			// so, GiveAbility
			FGameplayAbilitySpec Spec(Info.AbilityClass, 1);

			const FGameplayTag& NewStatusTag = AuraTags.Abilities_Status_Eligible;

			Spec.DynamicAbilityTags.AddTag(NewStatusTag);
			GiveAbility(Spec);
			MarkAbilitySpecDirty(Spec);
			
			ClientUpdateAbilityStatus(Info.AbilityTag, NewStatusTag, Spec.Level);
		}
	}
}

void UAuraAbilitySystemComponent::AddPlayerAbilitiesFromLoadedData(UAuraSaveData* SaveData)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	InitSlotToEquippedSpecMap();

	for (const FSkillSaveData& SkillData : SaveData->SavedSkills)
	{
		if (SkillData.AbilityClassPath.IsEmpty())
		{
			UE_LOG(Aura, Warning, TEXT("AbilityClassPath Is NOT Valid"));
			continue;
		}

		TSubclassOf<UGameplayAbility> AbilityClass;
		UClass* LoadedClass = StaticLoadClass(UGameplayAbility::StaticClass(), nullptr, *SkillData.AbilityClassPath);
		if (LoadedClass && LoadedClass->IsChildOf(UGameplayAbility::StaticClass()))
		{
			AbilityClass = LoadedClass;
		}

		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, SkillData.AbilityLevel);
		
		const FGameplayTag& SlotTag = FSaveHelper::ConvertStringToTag(SkillData.AbilitySlot);
		AbilitySpec.DynamicAbilityTags.AddTag(SlotTag);

		const FGameplayTag& StatusTag = FSaveHelper::ConvertStringToTag(SkillData.AbilityStatus);
		AbilitySpec.DynamicAbilityTags.AddTag(StatusTag);

		const FGameplayTag& TypeTag = FSaveHelper::ConvertStringToTag(SkillData.AbilityType);
		AbilitySpec.DynamicAbilityTags.AddTag(TypeTag);

		if (TypeTag.MatchesTagExact(AuraTags.Abilities_Type_Offensive))
		{
			AddAbilitiesWithTag(AbilitySpec);
		}
		else if (TypeTag.MatchesTagExact(AuraTags.Abilities_Type_Passive))
		{
			if (StatusTag.MatchesTagExact(AuraTags.Abilities_Status_Equipped))
			{
				AddAbilitiesWithTag(AbilitySpec);
				const FGameplayTag& AbilityTag = GetAbilityTagFromAbilitySpec(AbilitySpec);
				TryActivateAbilitiesByTag(FGameplayTagContainer(AbilityTag));
				MulticastActivatePassiveEffect(AbilityTag, true);
			}
			else
			{
				GiveAbility(AbilitySpec);
			}
		}
	}

	bStartupAbilitySet = true;
	OnStartupAbilitySetDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::ActivateAbilityOnPressed(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
		return;

	FScopedAbilityListLock ListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (AbilitySpec.IsActive())
			{
				if (UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance())
				{
					InvokeReplicatedEvent(
						EAbilityGenericReplicatedEvent::InputPressed, 
						AbilitySpec.Handle, 
						PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
				}
			}

			return;
		}
	}
}

void UAuraAbilitySystemComponent::ActivateAbilityOnHeld(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ListLock(*this);

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(AbilitySpec);

			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}

			return;
		}
	}
}

void UAuraAbilitySystemComponent::DeActivateAbilityOnReleased(const FGameplayTag& Tag)
{
	if (!Tag.IsValid())
	{
		return;
	}

	FScopedAbilityListLock ListLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(Tag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			
			if (UGameplayAbility* PrimaryInstance = AbilitySpec.GetPrimaryInstance())
			{
				bool InvokeResult = InvokeReplicatedEvent(
					EAbilityGenericReplicatedEvent::InputReleased,
					AbilitySpec.Handle,
					PrimaryInstance->GetCurrentActivationInfo().GetActivationPredictionKey());
			
				UE_LOG(Aura, Log, TEXT("Invoke Result: %d"), InvokeResult);
			}
			
			return;
		}
	}
}

void UAuraAbilitySystemComponent::GetEachAbilitySpec(const FGetEachAbilitySpecFuncType& Callback)
{
	FScopedAbilityListLock ActiveAbilityList(*this);
	for (const FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Callback.ExecuteIfBound(Spec))
		{
			UE_LOG(Aura, Error, TEXT("Callback Func Is Not Bound : %h"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	for (const FGameplayTag& Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("Abilities.Status")))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	if (Spec.Ability)
	{
		const FGameplayTagContainer TagContainer = Spec.Ability.Get()->AbilityTags;
		if (!TagContainer.IsEmpty())
		{
			for (const FGameplayTag& Tag : TagContainer)
			{
				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
				{
					continue;
				}

				if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
				{
					return Tag;
				}
			}
		}
	}
	return FGameplayTag();
}


FGameplayTag UAuraAbilitySystemComponent::GetSlotTagFromAbilitySpec(const FGameplayAbilitySpec& Spec)
{
	for (const FGameplayTag& Tag : Spec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Input"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FGameplayAbilitySpec* Spec = GetSpecFromActivatableAbilitiesByTagOrNull(AbilityTag);
	if (Spec)
	{
		return GetStatusTagFromAbilitySpec(*Spec);
	}

	return FGameplayTag();
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromActivatableAbilitiesByTagOrNull(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ListLock(*this);

	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if (!Spec.Ability)
			continue;

		for (const FGameplayTag& TagInContainer : Spec.Ability->AbilityTags)
		{
			// Assume Only One Valid Ability-Tag In Container Per Ability
			if (TagInContainer.MatchesTag(AbilityTag))
				return &Spec;
		}
	}
	return nullptr;
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayTag& AbilityTag)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	check(AbilityInfo);
	
	const FAuraAbilityInfo& Info = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);
	return Info.TypeTag.MatchesTagExact(FAuraGameplayTags::Get().Abilities_Type_Passive);
}

bool UAuraAbilitySystemComponent::IsEmptySlot(const FGameplayTag& SlotTag)
{
	return SlotToEquippedSpecOrNull[SlotTag] == FGameplayAbilitySpecHandle();
}

FGameplayAbilitySpecHandle UAuraAbilitySystemComponent::GetSpecHandleInSlot(const FGameplayTag& SlotTag)
{
	return SlotToEquippedSpecOrNull[SlotTag];
}

void UAuraAbilitySystemComponent::ClearSlot(const FGameplayTag& SlotTag, FGameplayAbilitySpec* SpecToClear)
{
	check(SpecToClear != nullptr);

	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	ChangeSpecAttachedTag(SpecToClear, AuraTags.Abilities_Status_Equipped, AuraTags.Abilities_Status_UnLocked);
	ChangeSpecAttachedTag(SpecToClear, SlotTag, FGameplayTag::EmptyTag);
	
	SlotToEquippedSpecOrNull[SlotTag] = FGameplayAbilitySpecHandle();
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, const UAbilityInfo* AbilityInfo, FString& OUTDesc, FString& OUTNextLevelDesc)
{
	if (!AbilityTag.IsValid())
		return false;

	const FAuraAbilityInfo& AuraAbilityInfo = AbilityInfo->FindAbilityByAbilityTag(AbilityTag);
	const FString& SkillName = AuraAbilityInfo.SkillName.ToString();

	if (const FGameplayAbilitySpec* Spec = GetSpecFromActivatableAbilitiesByTagOrNull(AbilityTag))
	{
		if (UAuraGABase* Ability = Cast<UAuraGABase>(Spec->Ability))
		{
			OUTDesc = Ability->GetDescription(SkillName, Spec->Level);
			OUTNextLevelDesc = Ability->GetNextLevelDescription(SkillName, Spec->Level);
			return true;
		}
	}
	
	OUTDesc = UAuraGABase::GetLockedBoxDescription(AuraAbilityInfo.LevelRequirement);
	OUTNextLevelDesc = FString();

	return false;
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitySet)
	{
		bStartupAbilitySet = true;
		OnStartupAbilitySetDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::IncreaseAttribute(const FGameplayTag& AttributeTag)
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor()))
	{
		if (PlayerInterface->GetAttributePoint() > 0)
		{
			ServerIncreaseAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::ServerEquipSkill_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& TargetSlotTag, const FGameplayTag& StatusTag)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	check(StatusTag.MatchesTagExact(AuraTags.Abilities_Status_Equipped) || StatusTag.MatchesTagExact(AuraTags.Abilities_Status_UnLocked));

	if (FGameplayAbilitySpec* Spec = GetSpecFromActivatableAbilitiesByTagOrNull(AbilityTag))
	{
		const FGameplayTag& PrevSlotTagInSpec = GetSlotTagFromAbilitySpec(*Spec);

		// Clearup Occupied Skill In The Slot
		if (!IsEmptySlot(TargetSlotTag))
		{
			const FGameplayAbilitySpecHandle OccupiedSpecHandle = GetSpecHandleInSlot(TargetSlotTag);
			FGameplayAbilitySpec* SlotOccuppiedSpec = FindAbilitySpecFromHandle(OccupiedSpecHandle);
			check(SlotOccuppiedSpec != nullptr);

			const FGameplayTag& OccupiedSpecAbilityTag = GetAbilityTagFromAbilitySpec(*SlotOccuppiedSpec);
			check(OccupiedSpecAbilityTag.IsValid());

			// For Passive Ability
			if (IsPassiveAbility(OccupiedSpecAbilityTag))
			{
				MulticastActivatePassiveEffect(OccupiedSpecAbilityTag, false);
				OnPassiveSkillDeactivate.Broadcast(OccupiedSpecAbilityTag);
			}

			ClearSlot(TargetSlotTag, SlotOccuppiedSpec);
			MarkAbilitySpecDirty(*SlotOccuppiedSpec);
			ClientEquipSkill(OccupiedSpecAbilityTag, AuraTags.Abilities_Status_UnLocked, FGameplayTag::EmptyTag, OccupiedSpecAbilityTag);
		}

		// For Passive Ability 
		if (IsPassiveAbility(AbilityTag) && !Spec->IsActive())
		{
			TryActivateAbility(Spec->Handle);
			MulticastActivatePassiveEffect(AbilityTag, true);
		}
		
		// Change Tags for Ability To Equip 
		if (StatusTag.MatchesTagExact(AuraTags.Abilities_Status_UnLocked))
		{
			ChangeSpecAttachedTag(Spec, AuraTags.Abilities_Status_UnLocked, AuraTags.Abilities_Status_Equipped);
		}
		else if (StatusTag.MatchesTagExact(AuraTags.Abilities_Status_Equipped))
		{
			// It's Already Equipped In Other Slot, So Cleanup The Slot
			SlotToEquippedSpecOrNull[PrevSlotTagInSpec] = FGameplayAbilitySpecHandle();
		}

		ChangeSpecAttachedTag(Spec, PrevSlotTagInSpec, TargetSlotTag);
		SlotToEquippedSpecOrNull[TargetSlotTag] = Spec->Handle;

		MarkAbilitySpecDirty(*Spec);
		ClientEquipSkill(AbilityTag, AuraTags.Abilities_Status_Equipped, TargetSlotTag, PrevSlotTagInSpec);
	}
	else
	{
		CheckValueOnScreen("Spec Is Nullptr", 1);
	}
}

void UAuraAbilitySystemComponent::ClientEquipSkill_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& TargetSlotTag, const FGameplayTag& PrevSlotTag)
{
	OnAbilityEquipChanged.Broadcast(AbilityTag, StatusTag, TargetSlotTag, PrevSlotTag);
}

void UAuraAbilitySystemComponent::ServerSpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	check(AbilityTag.IsValid() && !AbilityTag.MatchesTag(AuraTags.Abilities_None));

	FGameplayAbilitySpec* Spec = GetSpecFromActivatableAbilitiesByTagOrNull(AbilityTag);
	check(Spec != nullptr);

	FGameplayTag StatusTag = GetStatusTagFromAbilitySpec(*Spec);
	check(StatusTag != AuraTags.Abilities_Status_Locked);

	if (StatusTag == AuraTags.Abilities_Status_Eligible)
	{
		Spec->DynamicAbilityTags.RemoveTag(AuraTags.Abilities_Status_Eligible);
		Spec->DynamicAbilityTags.AddTag(AuraTags.Abilities_Status_UnLocked);
		StatusTag = AuraTags.Abilities_Status_UnLocked;
	}
	else
	{
		Spec->Level++;
	}

	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	if (PlayerInterface)
	{
		PlayerInterface->AddToSpellPoint(-1);
	}

	ClientUpdateAbilityStatus(AbilityTag, StatusTag, Spec->Level);
	MarkAbilitySpecDirty(*Spec);
}


void UAuraAbilitySystemComponent::ClientUpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	OnAbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}


void UAuraAbilitySystemComponent::ServerIncreaseAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, Payload);

	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetAvatarActor());
	if (PlayerInterface)
	{
		PlayerInterface->UpdateAttributePoint(-1);
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	OnEffectAssetTagsApplied.Broadcast(TagContainer);
}


void UAuraAbilitySystemComponent::InitSlotToEquippedSpecMap()
{
	for (const FGameplayTag& SlotTag : FAuraGameplayTags::Get().SlotTags)
	{
		SlotToEquippedSpecOrNull.Add({ SlotTag, FGameplayAbilitySpecHandle()});
	}
}

void UAuraAbilitySystemComponent::ChangeSpecAttachedTag(FGameplayAbilitySpec* Spec, const FGameplayTag& TagToRemove, const FGameplayTag& TagToAdd)
{
	Spec->DynamicAbilityTags.RemoveTag(TagToRemove);
	Spec->DynamicAbilityTags.AddTag(TagToAdd);
}

void UAuraAbilitySystemComponent::UpdateSlotToEquippedSpecMap(const FGameplayTag& InputTag, const FGameplayAbilitySpecHandle& SpecHandle)
{
	if (SlotToEquippedSpecOrNull.Contains(InputTag))
	{
		SlotToEquippedSpecOrNull[InputTag] = SpecHandle;
	}
}

void UAuraAbilitySystemComponent::MulticastActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, bool bActivate)
{
	OnActivatePassiveSkillEffect.Broadcast(AbilityTag, bActivate);
}
