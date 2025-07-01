// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AuraAbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

class UGameplayEffect;

template<typename T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


#define DEFINE_ONREP_FUNCTION(AttributeName) \
void UAuraAttributeSet::OnRep_##AttributeName(const FGameplayAttributeData& OldValue) \
{ \
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, AttributeName, OldValue); \
}


USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;

	UPROPERTY()
	AController* SourceController = nullptr;

	UPROPERTY()
	ACharacter* SourceCharacter = nullptr;

	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;

	UPROPERTY()
	AController* TargetController = nullptr;

	UPROPERTY()
	ACharacter* TargetCharacter = nullptr;
};

/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	UAuraAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	TMap<FGameplayTag, TStaticFuncPtr<FGameplayAttribute()>> TagsToAttributeMap;
	
	friend struct AuraDamageStatics;


	/*
	*  Accessors
	*/

	// Vital
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana);

	// Primary
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor);

	// Secondary
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana);

	// Resistance
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance);

	// Damage
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Damage);
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, RewardXP);

protected:
	
	/*  
	*  Attributes 
	*/

	// Vital
	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly, Category="Attribute")
	FGameplayAttributeData Health;

	UPROPERTY(ReplicatedUsing = OnRep_Mana, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Mana;

	// Primary
	UPROPERTY(ReplicatedUsing = OnRep_Strength, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Strength;

	UPROPERTY(ReplicatedUsing = OnRep_Intelligence, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Intelligence;

	UPROPERTY(ReplicatedUsing = OnRep_Resilience, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Resilience;

	UPROPERTY(ReplicatedUsing = OnRep_Vigor, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Vigor;

	// Secondary
	UPROPERTY(ReplicatedUsing = OnRep_Armor, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Armor;

	UPROPERTY(ReplicatedUsing = OnRep_ArmorPenetration, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ArmorPenetration;

	UPROPERTY(ReplicatedUsing = OnRep_BlockChance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData BlockChance;

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitChance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalHitChance;

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitDamage, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalHitDamage;

	UPROPERTY(ReplicatedUsing = OnRep_CriticalHitResistance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData CriticalHitResistance;

	UPROPERTY(ReplicatedUsing = OnRep_HealthRegeneration, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData HealthRegeneration;

	UPROPERTY(ReplicatedUsing = OnRep_ManaRegeneration, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ManaRegeneration;

	UPROPERTY(ReplicatedUsing = OnRep_MaxHealth, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_MaxMana, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData MaxMana;

	// Resistance
	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData FireResistance;

	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData LightningResistance;

	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData ArcaneResistance;

	UPROPERTY(ReplicatedUsing = OnRep_FireResistance, BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData PhysicalResistance;


	/*
	* Meta Attribute
	*/

	// Damage
	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData Damage;

	UPROPERTY(BlueprintReadOnly, Category = "Attribute")
	FGameplayAttributeData RewardXP;


	/*
	* OnRep Function
	*/
	
	// Vital
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue);


	// Primary
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldValue);


	// Secondary
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue);


	// Resistance
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue);


private:
	void HandleDamageAttribute(const FEffectProperties& Props, float DamageAmount, float ChangedHealth);
	void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& Props);
	void CallDie(const FEffectProperties& Props);
	void SendXPEvent(const FEffectProperties& Props);
	void ApplyXPAndLevel(class IPlayerInterface* PlayerInterface, int32 InXP);
	void HandleLevelUp(IPlayerInterface* PlayerInterface, int32 NewLevel);
	void Debuff(const FEffectProperties& Props);

	bool bHealthChangedByLevelUp = false;
	bool bManaChangedByLevelUp = false;

};
