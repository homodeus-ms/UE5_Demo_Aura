// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Game/AuraGameModeBase.h"
#include "AttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GA/AuraGABase.h"
#include "AbilitySystem/GA/AuraTargetedGA.h"
#include "Interface/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "SaveGame/LoadScreenSaveGame.h"
#include "AuraGameplayTags.h"
#include "AuraLog.h"

void UAuraAbilitySystemLibrary::WriteLog(const UObject* WorldContext, const FString& Log)
{
    UE_LOG(Aura, Log, TEXT("Blueprint Log: %s"), *Log);
}

bool UAuraAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContext, FWidgetControllerParams& OUTParams, AAuraHUD*& OUTAuraHUD)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContext, 0))
    {
        OUTAuraHUD = Cast<AAuraHUD>(PC->GetHUD());

        if (OUTAuraHUD)
        {
            OUTParams.PlayerController = PC;
            AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
            OUTParams.PlayerState = PS;
            OUTParams.ASC = PS->GetAbilitySystemComponent();
            OUTParams.AttributeSet = PS->GetAttributeSet();

            return true;
        }
    }

    return false;
}

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContext)
{
    FWidgetControllerParams Params;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContext, Params, AuraHUD))
    {
        check(AuraHUD != nullptr);
        return AuraHUD->GetOverlayWidgetController(Params);
    }

    return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContext)
{
    FWidgetControllerParams Params;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContext, Params, AuraHUD))
    {
        check(AuraHUD != nullptr);
        return AuraHUD->GetAttributeMenuWidgetController(Params);
    }

    return nullptr;
}

USpellMenuWidgetController* UAuraAbilitySystemLibrary::GetSpellMenuWidgetController(const UObject* WorldContext)
{
    FWidgetControllerParams Params;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContext, Params, AuraHUD))
    {
        check(AuraHUD != nullptr);
        return AuraHUD->GetSpellMenuWidgetController(Params);
    }

    return nullptr;
}

UInventoryWidgetController* UAuraAbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContext)
{
    FWidgetControllerParams Params;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContext, Params, AuraHUD))
    {
        check(AuraHUD != nullptr);
        return AuraHUD->GetInventoryWidgetController(Params);
    }

    return nullptr;
}

UChatController* UAuraAbilitySystemLibrary::GetChatController(const UObject* WorldContext)
{
    FWidgetControllerParams Params;
    AAuraHUD* AuraHUD = nullptr;
    if (MakeWidgetControllerParams(WorldContext, Params, AuraHUD))
    {
        check(AuraHUD != nullptr);
        return AuraHUD->GetChatController(Params);
    }

    return nullptr;
}

void UAuraAbilitySystemLibrary::InitDefaultAttributes(const UObject* WorldContext, UAbilitySystemComponent* InASC, ECharacterType CharacterType, float Level)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (AuraGM == nullptr)
    {
        return;
    }

    UCharacterClassInfo* ClassInfo = AuraGM->CharacterClassInfo;
    const FCharacterClassDefaultInfo& DefaultInfo = ClassInfo->FindCharacterClassDefaultInfo(CharacterType);
    const TSubclassOf<UGameplayEffect>& SecondaryAttribute = CharacterType == ECharacterType::Aura ?
        ClassInfo->AuraSecondaryAttribute : ClassInfo->SecondaryAttribute;

    AActor* AvatarActor = InASC->GetAvatarActor();
    FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
    ContextHandle.AddSourceObject(AvatarActor);

    // Primary
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(DefaultInfo.PrimaryAttribute, Level, ContextHandle);
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
    // Secondary
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(SecondaryAttribute, Level, ContextHandle);
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
    // Vital
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(ClassInfo->VitalAttribute, Level, ContextHandle);
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

}

void UAuraAbilitySystemLibrary::InitLoadedAttributes(const UObject* WorldContext, UAbilitySystemComponent* InASC, const FAttributesSaveData& SaveStats)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (AuraGM == nullptr)
    {
        return;
    }

    UCharacterClassInfo* ClassInfo = AuraGM->CharacterClassInfo; 

    AActor* AvatarActor = InASC->GetAvatarActor();
    FGameplayEffectContextHandle ContextHandle = InASC->MakeEffectContext();
    ContextHandle.AddSourceObject(AvatarActor);

    // Primary
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(ClassInfo->PrimaryAttribute_SetByCaller, 1.f, ContextHandle);

        // SetByCaller
        const FAuraGameplayTags& AuraTags = FAuraGameplayTags::Get();
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraTags.Attribute_Primary_Strength, SaveStats.Strength);
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraTags.Attribute_Primary_Intelligence, SaveStats.Intelligence);
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraTags.Attribute_Primary_Resilience, SaveStats.Resilience);
        UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, AuraTags.Attribute_Primary_Vigor, SaveStats.Vigor);

        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
    // Secondary
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(ClassInfo->AuraSecondaryAttribute, 1.f, ContextHandle);
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }
    // Vital
    {
        FGameplayEffectSpecHandle SpecHandle =
            InASC->MakeOutgoingSpec(ClassInfo->VitalAttribute, 1.f, ContextHandle);
        InASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
    }

}

void UAuraAbilitySystemLibrary::InitCommonAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (AuraGM == nullptr)
    {
        return;
    }

    const auto& CommonAbilities = AuraGM->CharacterClassInfo->CommonAbilities;

    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
        AuraASC->AddAbilities(CommonAbilities);
}

void UAuraAbilitySystemLibrary::InitStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC, ECharacterType CharacterType, float Level)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (AuraGM == nullptr)
    {
        return;
    }

    const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities = AuraGM->CharacterClassInfo->FindCharacterClassDefaultInfo(CharacterType).StartupAbilities;
    
    if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC))
    {
        AuraASC->InitSlotToEquippedSpecMap();
        AuraASC->AddAbilitiesWithTag(StartupAbilities);
    }
}

void UAuraAbilitySystemLibrary::InitPassiveAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC, const TArray<TSubclassOf<class UGameplayAbility>>& PassiveAbilities)
{
    UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC);
    AuraASC->AddPassiveStartupAbilities(PassiveAbilities);
}

int32 UAuraAbilitySystemLibrary::GetRewardPoint(const UObject* WorldContext, ECharacterType CharacterType, float Level)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (AuraGM == nullptr)
        return -1;

    const FScalableFloat ScalableFloatValue = AuraGM->CharacterClassInfo->FindCharacterClassDefaultInfo(CharacterType).XPReward;
    
    return static_cast<int32>(ScalableFloatValue.GetValueAtLevel(Level));
}


void UAuraAbilitySystemLibrary::GetLiveActorsInRange(const UObject* WorldContext, TArray<AActor*>& OUTLiveActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereCenter, float Radius, EActorSearchType SearchType)
{
    UWorld* World = WorldContext->GetWorld();
    if (!World)
        return;

    FCollisionQueryParams QueryParams;
    QueryParams.bTraceComplex = false;
    QueryParams.AddIgnoredActors(ActorsToIgnore);
    FCollisionObjectQueryParams ObjectParams = FCollisionObjectQueryParams::InitType::AllDynamicObjects;

    TArray<FOverlapResult> Overlaps;
    TSet<AActor*> TempSet;

    bool bOverlapped = World->OverlapMultiByObjectType(Overlaps, SphereCenter, FQuat::Identity, ObjectParams, FCollisionShape::MakeSphere(Radius), QueryParams);


    if (bOverlapped)
    {
        for (const auto& Overlapped : Overlaps)
        {
            AActor* OverlappedActor = Overlapped.GetActor();
            if (ICombatInterface* CombatActor = Cast<ICombatInterface>(OverlappedActor))
            {
                if (ICombatInterface::Execute_IsDead(OverlappedActor))
                    continue;

                switch (SearchType)
                {
                case EActorSearchType::AllLiveActors:
                    TempSet.Add(OverlappedActor);
                    break;
                case EActorSearchType::PlayersOnly:
                    if (CombatActor->IsPlayer())
                    {
                        TempSet.Add(OverlappedActor);
                    }
                    break;
                case EActorSearchType::EnemiesOnly:
                    if (!CombatActor->IsPlayer())
                    {
                        TempSet.Add(OverlappedActor);
                    }
                    break;
                default:
                    check(false);
                    break;
                }
            }
        }

        for (AActor* Actor : TempSet)
        {
            OUTLiveActors.Add(Actor);
        }
    }
}

UAbilityInfo* UAuraAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContext)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (!AuraGM)
        return nullptr;

    return AuraGM->AbilityInfo;
}

UTreasureBoxItemsInfo* UAuraAbilitySystemLibrary::GetTreasureBoxItemsInfo(const UObject* WorldContext)
{
    AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContext));
    if (!AuraGM)
        return nullptr;

    return AuraGM->TreasureBoxItemsInfo;
}


FGameplayEffectContextHandle UAuraAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
    const FAuraGameplayTags& Tags = FAuraGameplayTags::Get();

    UAbilitySystemComponent* SourceASC = DamageEffectParams.SourceASC;
    AActor* SourceActor = SourceASC->GetAvatarActor();

    FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
    EffectContextHandle.AddSourceObject(SourceActor);
    FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectParams.DamageEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);
    
    if (DamageEffectParams.bIsRadialDamage)
    {
        SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bIsRadialDamage);
        SetRadialInnerRadius(EffectContextHandle, DamageEffectParams.RadialInnerRadius);
        SetRadialOuterRadius(EffectContextHandle, DamageEffectParams.RadialOuterRadius);
        SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);
    }

    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageTypeTag, DamageEffectParams.BaseDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Debuff_Chance, DamageEffectParams.DebuffChance);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Debuff_Damage, DamageEffectParams.DebuffDamage);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Debuff_Duration, DamageEffectParams.DebuffDuration);
    UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Tags.Debuff_Frequency, DamageEffectParams.DebuffFrequency);
    
    SetKnockbackPos(EffectContextHandle, DamageEffectParams.KnockbackPos);

    DamageEffectParams.TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);

    return EffectContextHandle;
}

TArray<FRotator> UAuraAbilitySystemLibrary::GetRotatorsSpread(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 NumRotator)
{
    TArray<FRotator> Rotators;
    if (NumRotator <= 1)
    {
        Rotators.Add(Forward.Rotation());
        return Rotators;
    }

    const FVector LeftStart = Forward.RotateAngleAxis(-SpreadAngle / 2.f, Axis);
    const float DeltaAngle = SpreadAngle / (NumRotator - 1);

    for (int32 i = 0; i < NumRotator; ++i)
    {
        FVector Next = LeftStart.RotateAngleAxis(DeltaAngle * i, FVector::UpVector);
        Rotators.Add(Next.Rotation());
    }

    return Rotators;
}

TArray<FVector> UAuraAbilitySystemLibrary::GetVectorsSpread(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 NumVector)
{
    TArray<FVector> Vectors;
    if (NumVector <= 1)
    {
        Vectors.Add(Forward);
        return Vectors;
    }

    const FVector LeftStart = Forward.RotateAngleAxis(-SpreadAngle / 2.f, Axis);
    const float DeltaAngle = SpreadAngle / (NumVector - 1);

    for (int32 i = 0; i < NumVector; ++i)
    {
        FVector Next = LeftStart.RotateAngleAxis(DeltaAngle * i, FVector::UpVector);
        Vectors.Add(Next);
    }

    return Vectors;
}

FString UAuraAbilitySystemLibrary::ConvertClassToStringPath(const TSubclassOf<UObject>& ObjectClass)
{
    return FSoftObjectPath(ObjectClass.Get()).ToString();
}

TSubclassOf<class UGameplayAbility> UAuraAbilitySystemLibrary::ConvertPathToGameplayAbilityClass(const FString& Path)
{
    UClass* LoadedClass = StaticLoadClass(UGameplayAbility::StaticClass(), nullptr, *Path);
    if (!LoadedClass->IsChildOf(UGameplayAbility::StaticClass()))
    {
        return nullptr;
    }

    return TSubclassOf<UGameplayAbility>(LoadedClass);
}

bool UAuraAbilitySystemLibrary::IsBlocked(const FGameplayEffectContextHandle& Handle)
{
    const FGameplayEffectContext* Context = Handle.Get();
    const FAuraGameplayEffectContext* AuraGEContext = static_cast<const FAuraGameplayEffectContext*>(Context);
    return AuraGEContext->IsBlocked();
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& Handle)
{
    const FGameplayEffectContext* Context = Handle.Get();
    const FAuraGameplayEffectContext* AuraGEContext = static_cast<const FAuraGameplayEffectContext*>(Context);
    return AuraGEContext->IsCriticalHit();
}

void UAuraAbilitySystemLibrary::SetIsBlocked(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool IsBlocked)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetIsBlocked(IsBlocked);
}

void UAuraAbilitySystemLibrary::SetIsCirticalHit(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool IsCriticalHit)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetIsCriticalHit(IsCriticalHit);
}

bool UAuraAbilitySystemLibrary::ShouldDebuffActivate(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->ShouldDebuffActivate();
}

float UAuraAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetDebuffDamage();
}

float UAuraAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetDebuffDuration();
}

float UAuraAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetDebuffFrequency();
}

FGameplayTag UAuraAbilitySystemLibrary::GetDamageTypeTag(const FGameplayEffectContextHandle& Handle)
{
    TSharedPtr<FGameplayTag> Tag = GetAuraGEContext(Handle)->GetDebuffDamageTypeTag().Pin();
    if (Tag.IsValid())
    {
        return *Tag;
    }
    return FGameplayTag();
}

void UAuraAbilitySystemLibrary::SetDebuffActivate(UPARAM(ref)FGameplayEffectContextHandle& Handle, bool bActivate)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetDebuffActivate(bActivate);
}

void UAuraAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& Handle, float DebuffDamage)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetDebuffDamage(DebuffDamage);
}

void UAuraAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& Handle, float DebuffDuration)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetDebuffDuration(DebuffDuration);
}

void UAuraAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& Handle, float DebuffFrequency)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetDebuffFrequency(DebuffFrequency);
}

void UAuraAbilitySystemLibrary::SetDamageTypeTag(UPARAM(ref)FGameplayEffectContextHandle& Handle, const FGameplayTag& DamageTypeTag)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    TSharedPtr<FGameplayTag> Tag = MakeShared<FGameplayTag>(DamageTypeTag);
    AuraGEContext->SetDamgeTypeTag(Tag);
}

FVector UAuraAbilitySystemLibrary::GetKnockbackPos(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetKnockbackPos();
}

void UAuraAbilitySystemLibrary::SetKnockbackPos(UPARAM(ref)FGameplayEffectContextHandle& Handle, FVector InPos)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetKnockbackPos(InPos);
}

bool UAuraAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->IsRadialDamage();
}

float UAuraAbilitySystemLibrary::GetRadialInnerRadius(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetRadialInnerRadius();
}

float UAuraAbilitySystemLibrary::GetRadialOuterRadius(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetRadialOuterRadius();
}

FVector UAuraAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& Handle)
{
    return GetAuraGEContext(Handle)->GetRadialDamageOrigin();
}

void UAuraAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& Handle, bool IsRadialDamage)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetIsRadialDamage(IsRadialDamage);
}

void UAuraAbilitySystemLibrary::SetRadialInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& Handle, float InRadius)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetRadialInnerRadius(InRadius);
}

void UAuraAbilitySystemLibrary::SetRadialOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& Handle, float InRadius)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetRadialOuterRadius(InRadius);
}

void UAuraAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& Handle, const FVector& DamageOrigin)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    AuraGEContext->SetRadialDamageOrigin(DamageOrigin);
}

const TArray<FVector_NetQuantize>& UAuraAbilitySystemLibrary::GetLocations(FGameplayEffectContextHandle Handle)
{
    FAuraGameplayEffectContext* AuraGEContext = static_cast<FAuraGameplayEffectContext*>(Handle.Get());
    return AuraGEContext->GetLocations();
}

const FAuraGameplayEffectContext* UAuraAbilitySystemLibrary::GetAuraGEContext(const FGameplayEffectContextHandle& Handle)
{
    const FAuraGameplayEffectContext* RetValue = static_cast<const FAuraGameplayEffectContext*>(Handle.Get());
    check(RetValue != nullptr);
    return RetValue;
}





