// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * 
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitAuraGameplayTags();

	FGameplayTag Attribute_Vital_Health;
	FGameplayTag Attribute_Vital_Mana;

	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_ArmorPenetration;
	FGameplayTag Attribute_Secondary_BlockChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_CriticalHitResistance;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

	FGameplayTag Attribute_Resistance_Fire;
	FGameplayTag Attribute_Resistance_Lightning;
	FGameplayTag Attribute_Resistance_Arcane;
	FGameplayTag Attribute_Resistance_Physical;

	FGameplayTag Attribute_Meta_RewardXP;

	// Input
	FGameplayTag Input_Action_LMB;
	FGameplayTag Input_Action_RMB;
	FGameplayTag Input_Action_Key1;
	FGameplayTag Input_Action_Key2;
	FGameplayTag Input_Action_Key3;
	FGameplayTag Input_Action_Key4;

	FGameplayTag Input_Consume_Key5;
	FGameplayTag Input_Consume_Key6;
	FGameplayTag Input_Consume_Key7;
	FGameplayTag Input_Consume_Key8;

	FGameplayTag Input_Chat_Enter;

	FGameplayTag Input_Passive_1;
	FGameplayTag Input_Passive_2;

	TArray<FGameplayTag> SlotTags;    // Input Tags Also Work As SlotTags At Each Slot.

	// Event
	FGameplayTag Event_Montage_FireBolt;
	FGameplayTag Event_Montage_Electrocute;
	FGameplayTag Event_Montage_ArcaneShards;

	// Damage
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	// Debuff
	FGameplayTag Debuff;
	FGameplayTag Debuff_Burn;
	FGameplayTag Debuff_Stun;
	FGameplayTag Debuff_Arcane;
	FGameplayTag Debuff_Physical;

	FGameplayTag Debuff_Chance;
	FGameplayTag Debuff_Duration;
	FGameplayTag Debuff_Damage;
	FGameplayTag Debuff_Frequency;

	TMap<FGameplayTag, FGameplayTag> DamageTypeToResistance;
	TMap<FGameplayTag, FGameplayTag> DamageTypeToDebuff;

	// Effects
	FGameplayTag Effects_HitReact;

	// Cooldown
	FGameplayTag Cooldown_Fire_FireBolt;
	FGameplayTag Cooldown_Lightning_Electrocute;
	FGameplayTag Cooldown_Arcane_Shards;
	

	// Active Abilities
	FGameplayTag Abilities_None;
	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Summon;
	FGameplayTag Abilities_HitReact;

	FGameplayTag Abilities_Fire_FireBolt;
	FGameplayTag Abilities_Lightning_Electrocute;
	FGameplayTag Abilities_Arcane_ArcaneShards;

	FGameplayTag Abilities_Status_Locked;
	FGameplayTag Abilities_Status_Eligible;
	FGameplayTag Abilities_Status_UnLocked;
	FGameplayTag Abilities_Status_Equipped;

	FGameplayTag Abilities_Type_Offensive;
	FGameplayTag Abilities_Type_Passive;
	FGameplayTag Abilities_Type_None;

	// Passive Abilityies
	FGameplayTag Abilities_Passive_HaloOfProtection;
	FGameplayTag Abilities_Passive_HealthSiphon;
	FGameplayTag Abilities_Passive_ManaSiphon;
	FGameplayTag Abilities_Passive_ListenForEvents;

	 
	// Attack Socket Tag
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_Tail;

	// Tagged Montage
	FGameplayTag Montage_Attack1;
	FGameplayTag Montage_Attack2;
	FGameplayTag Montage_Attack3;
	FGameplayTag Montage_Attack4;

	// GC
	FGameplayTag GameplayCue_MeleeAttack;
	FGameplayTag GameplayCue_Summoning;
	FGameplayTag GameplayCue_ShockBurst;
	FGameplayTag GameplayCue_ShockLooping;
	FGameplayTag GameplayCue_ArcaneShards;
	FGameplayTag GameplayCue_BoxOpen;
	FGameplayTag GameplayCue_GetRareItem;

	// Block 
	FGameplayTag Player_Block_InputPressed;
	FGameplayTag Player_Block_InputHeld;
	FGameplayTag Player_Block_InputReleased;
	FGameplayTag Player_Block_CursorTrace;

	// Boss Enemy
	FGameplayTag Boss_Combo_Completed;

	// Item
	FGameplayTag Item_Vacant;
	FGameplayTag Item_Buyable_AddHealth;
	FGameplayTag Item_Buyable_AddMana;
	FGameplayTag Item_Gold;
	FGameplayTag Item_Rare_Collecting;

	FGameplayTag Item_Ingredient_Earth;
	FGameplayTag Item_Ingredient_Fire;
	FGameplayTag Item_Ingredient_Water;
	FGameplayTag Item_Ingredient_Wind;

	FGameplayTag Item_Collecting_Stone;
	FGameplayTag Item_Collecting_Elixir;
	FGameplayTag Item_Collecting_DragonsBlood;
	FGameplayTag Item_Collecting_Mercury;
	FGameplayTag Item_Collecting_Salt;
	FGameplayTag Item_Collecting_AlchemistsFire;
	FGameplayTag Item_Collecting_HeartOfImmortality;
	
	TArray<FGameplayTag> CollectingItemTags;
	

	FORCEINLINE bool IsLoaded() const { return bLoaded; }

private:
	static FAuraGameplayTags GameplayTags;
	bool bLoaded = false;
};
