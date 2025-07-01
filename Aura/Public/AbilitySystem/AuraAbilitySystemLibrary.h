// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "DedicatedServer/Public/Save/AuraSaveData.h"
#include "AuraAbilityTypes.h"
#include "AuraAbilitySystemLibrary.generated.h"


UENUM(BlueprintType)
enum class EActorSearchType : uint8
{
	PlayersOnly,
	EnemiesOnly,
	AllLiveActors
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static void WriteLog(const UObject* WorldContext, const FString& Log);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta=(DefaultToSelf="WorldContext"))
	static bool MakeWidgetControllerParams(const UObject* WorldContext, FWidgetControllerParams& OUTParams, class AAuraHUD*& OUTAuraHUD);

	UFUNCTION(BlueprintPure, Category="AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static class UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static class UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static class USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static class UInventoryWidgetController* GetInventoryWidgetController(const UObject* WorldContext);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContext"))
	static class UChatController* GetChatController(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitDefaultAttributes(const UObject* WorldContext, UAbilitySystemComponent* InASC, ECharacterType CharacterType, float Level);

	UFUNCTION()
	static void InitLoadedAttributes(const UObject* WorldContext, UAbilitySystemComponent* InASC, const FAttributesSaveData& SaveStats);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitCommonAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitStartupAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC, ECharacterType CharacterType, float Level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static void InitPassiveAbilities(const UObject* WorldContext, UAbilitySystemComponent* ASC, const TArray<TSubclassOf<class UGameplayAbility>>& PassiveAbilities);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClass")
	static int32 GetRewardPoint(const UObject* WorldContext, ECharacterType CharacterType, float Level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|Gameplay", meta = (AutoCreateRefTerm = "ActorsToIgnore"))
	static void GetLiveActorsInRange(const UObject* WorldContext, TArray<AActor*>& OUTLiveActors, const TArray<AActor*>& ActorsToIgnore, const FVector& SphereCenter, float Radius, EActorSearchType SearchType);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|AbilityInfo")
	static class UAbilityInfo* GetAbilityInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|TreasureBoxItemsInfo")
	static class UTreasureBoxItemsInfo* GetTreasureBoxItemsInfo(const UObject* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> GetRotatorsSpread(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 NumRotator);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> GetVectorsSpread(const FVector& Forward, const FVector& Axis, float SpreadAngle, int32 NumVector);

	UFUNCTION()
	static FString ConvertClassToStringPath(const TSubclassOf<UObject>& ObjectClass);

	UFUNCTION()
	static TSubclassOf<class UGameplayAbility> ConvertPathToGameplayAbilityClass(const FString& Path);

	/*
	*  For Custom GE Context (FAuraGameplayEffectContext)
	*/
	
	// Blocked, CritHit
	static bool IsBlocked(const FGameplayEffectContextHandle& Handle);
	static bool IsCriticalHit(const FGameplayEffectContextHandle& Handle);
	static void SetIsBlocked(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool IsBlocked);
	static void SetIsCirticalHit(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool IsCriticalHit);

	// Debuff
	static bool ShouldDebuffActivate(const FGameplayEffectContextHandle& Handle);
	static float GetDebuffDamage(const FGameplayEffectContextHandle& Handle);
	static float GetDebuffDuration(const FGameplayEffectContextHandle& Handle);
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& Handle);
	static FGameplayTag GetDamageTypeTag(const FGameplayEffectContextHandle& Handle);

	static void SetDebuffActivate(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool bActivate);
	static void SetDebuffDamage(UPARAM(ref) FGameplayEffectContextHandle& Handle, float DebuffDamage);
	static void SetDebuffDuration(UPARAM(ref) FGameplayEffectContextHandle& Handle, float DebuffDuration);
	static void SetDebuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& Handle, float DebuffFrequency);
	static void SetDamageTypeTag(UPARAM(ref) FGameplayEffectContextHandle& Handle, const FGameplayTag& DamageTypeTag);

	// Knockback
	static FVector GetKnockbackPos(const FGameplayEffectContextHandle& Handle);
	static void SetKnockbackPos(UPARAM(ref) FGameplayEffectContextHandle& Handle, FVector InPos);

	// Radial Damage
	static bool IsRadialDamage(const FGameplayEffectContextHandle& Handle);
	static float GetRadialInnerRadius(const FGameplayEffectContextHandle& Handle);
	static float GetRadialOuterRadius(const FGameplayEffectContextHandle& Handle);
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& Handle);

	static void SetIsRadialDamage(UPARAM(ref) FGameplayEffectContextHandle& Handle, bool IsRadialDamage);
	static void SetRadialInnerRadius(UPARAM(ref) FGameplayEffectContextHandle& Handle, float InRadius);
	static void SetRadialOuterRadius(UPARAM(ref) FGameplayEffectContextHandle& Handle, float InRadius);
	static void SetRadialDamageOrigin(UPARAM(ref) FGameplayEffectContextHandle& Handle, const FVector& DamageOrigin);

	UFUNCTION(BlueprintCallable)
	static const TArray<FVector_NetQuantize>& GetLocations(FGameplayEffectContextHandle Handle);
	
private:
	static const FAuraGameplayEffectContext* GetAuraGEContext(const FGameplayEffectContextHandle& Handle);
	
};
