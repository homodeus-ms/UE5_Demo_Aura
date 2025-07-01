// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/Character.h"
#include "AuraGameplayTags.h"
#include "Interface/CombatInterface.h"
#include "Interface/EnemyInterface.h"
#include "Interface/PlayerInterface.h"
#include "AuraLog.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AuraAbilityTypes.h"
#include <GameplayEffectComponents/TargetTagsGameplayEffectComponent.h>

UAuraAttributeSet::UAuraAttributeSet()
{
	const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

	// Vital
	TagsToAttributeMap.Add(Tags.Attribute_Vital_Health, GetHealthAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Vital_Mana, GetManaAttribute);

	// Primary
	TagsToAttributeMap.Add(Tags.Attribute_Primary_Strength, GetStrengthAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Primary_Vigor, GetVigorAttribute);

	// Secondary
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_Armor, GetArmorAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Secondary_MaxMana, GetMaxManaAttribute);

	// Resistance
	TagsToAttributeMap.Add(Tags.Attribute_Resistance_Fire, GetFireResistanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Resistance_Lightning, GetLightningResistanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Resistance_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributeMap.Add(Tags.Attribute_Resistance_Physical, GetPhysicalResistanceAttribute);

	
}

void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Mana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);

	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);
}

void UAuraAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
}

void UAuraAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	// Only Set MaxHealth and MaxMana When Level Up 
	if (Attribute == GetMaxHealthAttribute() && bHealthChangedByLevelUp)
	{
		SetHealth(NewValue);
		bHealthChangedByLevelUp = false;
	}
	else if (Attribute == GetMaxManaAttribute() && bManaChangedByLevelUp)
	{
		SetMana(NewValue);
		bManaChangedByLevelUp = false;
	}
}

void UAuraAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	
	UAbilitySystemComponent* SourceASC = Data.EffectSpec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	if (!IsValid(SourceASC))
	{
		UE_LOG(Aura, Log, TEXT("Not Valid SourceASC %s"), __FUNCTION__);
		return;
	}
	if (!IsValid(Data.Target.GetAvatarActor()))
	{
		UE_LOG(Aura, Log, TEXT("Not Valid Target Actor In Data %s"), __FUNCTION__);
		return;
	}

	FEffectProperties Props;
	SetEffectProperties(Data, Props);

	if (IsValid(Props.TargetAvatarActor) && Props.TargetAvatarActor->Implements<UCombatInterface>())
	{
		if (ICombatInterface::Execute_IsDead(Props.TargetAvatarActor))
		{
			const FString& TargetName = Props.TargetAvatarActor->GetName();
			UE_LOG(Aura, Log, TEXT("Execute_IsDead(): %s is Already Dead!"), *TargetName);
			return;
		}
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));

		const FString& TargetName = Props.TargetAvatarActor->GetName();

		if (GetHealth() == 0.f)
		{
			CallDie(Props);
		}
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		const float DamageAmount = Data.EvaluatedData.Magnitude;
		const float ChangedHealth = GetHealth() - DamageAmount;

		SetHealth(FMath::Clamp(ChangedHealth, 0.f, GetMaxHealth()));

		if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Props.SourceAvatarActor))
		{
			if (PlayerInterface->IsSiphonEffectActivate())
			{
				PlayerInterface->HandleSiphonEffect(DamageAmount);
			}
		}

		HandleDamageAttribute(Props, DamageAmount, ChangedHealth);
	}
	else if (Data.EvaluatedData.Attribute == GetRewardXPAttribute())
	{
		IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Props.SourceAvatarActor);
		if (PlayerInterface)
		{
			ApplyXPAndLevel(PlayerInterface, GetRewardXP());
			SetRewardXP(0.f);
		}
	}
}


void UAuraAttributeSet::HandleDamageAttribute(const FEffectProperties& Props, float DamageAmount, float ChangedHealth)
{
	const FGameplayEffectContextHandle& ContextHandle = Props.EffectContextHandle;

	// Death 와 HitReact, Knockback 처리
	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
	{
		bool bIsNotPlayer = !Cast<ICombatInterface>(Props.TargetAvatarActor)->IsPlayer();
		bool bIsNotInShockLoop = !Cast<ICombatInterface>(Props.TargetAvatarActor)->Execute_IsShockLoop(Props.TargetAvatarActor);

		if (ChangedHealth <= 0.f)
		{
			CallDie(Props);
			SendXPEvent(Props);
		}
		else if (bIsNotPlayer && bIsNotInShockLoop)
		{
			// Handle Hit Result
			FGameplayTagContainer TagContainer;
			TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
			if (IsValid(Props.TargetASC))
			{
				bool bResult = Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

				if (!bResult)
				{
					const TArray<FGameplayTag>& Tags = TagContainer.GetGameplayTagArray();
					for (const FGameplayTag& Tag : Tags)
					{
						UE_LOG(Aura, Log, TEXT("Tag: %s"), *Tag.ToString());
					}
				}
			}
			else
			{
				UE_LOG(Aura, Warning, TEXT("Props.TargetASC NOT Valid, %s"), LOG_POS);
			}

			// Handle Knockback
			FVector Pos = UAuraAbilitySystemLibrary::GetKnockbackPos(ContextHandle);
			if (!Pos.IsNearlyZero(1.f) && IsValid(Props.TargetCharacter))
			{
				Props.TargetCharacter->LaunchCharacter(Pos, true, true);
			}
		}
	}

	// Debuff
	if (UAuraAbilitySystemLibrary::ShouldDebuffActivate(ContextHandle))
	{
		Debuff(Props);
	}


	// Show Damage Text

	IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(Props.SourceAvatarActor) ? 
		Cast<IPlayerInterface>(Props.SourceAvatarActor) : Cast<IPlayerInterface>(Props.TargetAvatarActor);
	if (!PlayerInterface)
	{
		UE_LOG(Aura, Log, TEXT("Source And Target Both are Not Player!, %s"), LOG_POS);
		return;
	}

	if (!FMath::IsNearlyZero(DamageAmount))
	{
		if (PlayerInterface && IsValid(Props.TargetAvatarActor))
		{
			bool bBlocked = UAuraAbilitySystemLibrary::IsBlocked(ContextHandle);
			bool bCritHit = UAuraAbilitySystemLibrary::IsCriticalHit(ContextHandle);
			PlayerInterface->ShowDamageText(DamageAmount, bBlocked, bCritHit, Cast<ACharacter>(Props.TargetAvatarActor));
		}
	}
}

void UAuraAttributeSet::SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props)
{
	Props.EffectContextHandle = Data.EffectSpec.GetContext();
	Props.SourceASC = Props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	TSharedPtr<FGameplayAbilityActorInfo> SourceActorInfo = Props.SourceASC->AbilityActorInfo;
	TSharedPtr<FGameplayAbilityActorInfo> TargetActorInfo = Data.Target.AbilityActorInfo;

	if (Props.SourceASC && SourceActorInfo)
	{
		Props.SourceAvatarActor = SourceActorInfo->AvatarActor.Get();
		Props.SourceController = SourceActorInfo->PlayerController.Get();

		if (Props.SourceController == nullptr && Props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(Props.SourceAvatarActor))
			{
				Props.SourceController = Pawn->GetController();
			}
		}
		
		if (Props.SourceController)
		{
			Props.SourceCharacter = Cast<ACharacter>(Props.SourceController->GetPawn());
		}
	}

	if (TargetActorInfo && TargetActorInfo->AvatarActor.Get())
	{
		Props.TargetAvatarActor = TargetActorInfo->AvatarActor.Get();
		Props.TargetController = TargetActorInfo->PlayerController.Get();
		Props.TargetCharacter = Cast<ACharacter>(Props.TargetAvatarActor);
		Props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Props.TargetAvatarActor);
	}
}

void UAuraAttributeSet::CallDie(const FEffectProperties& Props)
{
	if (!GetOwningActor()->HasAuthority())
		return;

	if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor))
	{
		CombatInterface->Die();
	}
}

void UAuraAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(Props.TargetAvatarActor);
	if (!EnemyInterface)
		return;

	const float EarnXP = EnemyInterface->GetRewardPoint();

	if (IsValid(Props.SourceAvatarActor))
	{
		AActor* Player = Props.SourceAvatarActor;
		FGameplayTag EventTag = FAuraGameplayTags::Get().Attribute_Meta_RewardXP;
		FGameplayEventData Payload;
		Payload.EventTag = EventTag;
		Payload.EventMagnitude = EarnXP;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Player, EventTag, Payload);
	}
}

void UAuraAttributeSet::ApplyXPAndLevel(IPlayerInterface* PlayerInterface, int32 InXP)
{
	// Check Level Up Condition
	if (PlayerInterface->IsLevelUpCondition(InXP))
	{
		bHealthChangedByLevelUp = true;
		bManaChangedByLevelUp = true;

		int32 CurrentXP = PlayerInterface->GetCurrentXP();
		int32 NewLevel = PlayerInterface->GetNewLevelForXP(CurrentXP + InXP);

		HandleLevelUp(PlayerInterface, NewLevel);
	}

	PlayerInterface->AddToXP(InXP);
}

void UAuraAttributeSet::HandleLevelUp(IPlayerInterface* PlayerInterface, int32 NewLevel)
{
	PlayerInterface->UpdateLevel(NewLevel);
	PlayerInterface->HandleLevelUp();
	
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetOwningAbilitySystemComponent()))
	{
		AuraASC->AddAbilityOnLevel(NewLevel);
	}
}

void UAuraAttributeSet::Debuff(const FEffectProperties& Props)
{
	const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
	UAbilitySystemComponent* SourceASC = Props.SourceASC;
	UAbilitySystemComponent* TargetASC = Props.TargetASC;

	if (!IsValid(SourceASC) || !IsValid(TargetASC))
		return;

	FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();
	ContextHandle.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag& DamageTypeTag = UAuraAbilitySystemLibrary::GetDamageTypeTag(Props.EffectContextHandle);
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);

	FString GEName = FString::Printf(TEXT("Debuff Effect By %s"), *DamageTypeTag.ToString());
	UGameplayEffect* GE = NewObject<UGameplayEffect>(GetTransientPackage(), FName(GEName));

	GE->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	GE->Period = DebuffFrequency;
	GE->DurationMagnitude = FScalableFloat(DebuffDuration);

	// GE->InheritableOwnedTagsContainer.AddTag(AuraTags.DamageTypeToDebuff[DamageTypeTag]); -> |Depricated| 
	FInheritedTagContainer TagContainer;
	UTargetTagsGameplayEffectComponent& TargetTagsComponent = GE->FindOrAddComponent<UTargetTagsGameplayEffectComponent>();
	TagContainer.Added.AddTag(AuraTags.DamageTypeToDebuff[DamageTypeTag]);
	TargetTagsComponent.SetAndApplyTargetTagChanges(TagContainer);

	
	GE->StackingType = EGameplayEffectStackingType::AggregateBySource;
	GE->StackLimitCount = 1;

	const int32 Index = GE->Modifiers.Num();
	FGameplayModifierInfo ModifierInfo;
	ModifierInfo.Attribute = UAuraAttributeSet::GetDamageAttribute();
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	GE->Modifiers.Add(ModifierInfo);

	if (FGameplayEffectSpec* Spec = new FGameplayEffectSpec(GE, ContextHandle, 1.f))
	{
		//Spec->DynamicGrantedTags.AddTag(AuraTags.DamageTypeToDebuff[DamageTypeTag]);
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(Spec->GetContext().Get());
		TSharedPtr<FGameplayTag> DebuffTypeTag = MakeShared<FGameplayTag>(DamageTypeTag);
		AuraContext->SetDamgeTypeTag(DebuffTypeTag);

		TargetASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}
}


/*
*  OnRep_ Function Defines Using Macro
*/

DEFINE_ONREP_FUNCTION(Health)
DEFINE_ONREP_FUNCTION(Mana)

DEFINE_ONREP_FUNCTION(Strength)
DEFINE_ONREP_FUNCTION(Intelligence)
DEFINE_ONREP_FUNCTION(Resilience)
DEFINE_ONREP_FUNCTION(Vigor)

DEFINE_ONREP_FUNCTION(Armor)
DEFINE_ONREP_FUNCTION(ArmorPenetration)
DEFINE_ONREP_FUNCTION(BlockChance)
DEFINE_ONREP_FUNCTION(CriticalHitChance)
DEFINE_ONREP_FUNCTION(CriticalHitDamage)
DEFINE_ONREP_FUNCTION(CriticalHitResistance)
DEFINE_ONREP_FUNCTION(HealthRegeneration)
DEFINE_ONREP_FUNCTION(ManaRegeneration)
DEFINE_ONREP_FUNCTION(MaxHealth)
DEFINE_ONREP_FUNCTION(MaxMana)

DEFINE_ONREP_FUNCTION(FireResistance)
DEFINE_ONREP_FUNCTION(LightningResistance)
DEFINE_ONREP_FUNCTION(ArcaneResistance)
DEFINE_ONREP_FUNCTION(PhysicalResistance)





