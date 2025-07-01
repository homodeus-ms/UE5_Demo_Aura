// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitAuraGameplayTags()
{
	if (GameplayTags.IsLoaded())
	{
		return;
	}

	UGameplayTagsManager& TagsManager = UGameplayTagsManager::Get();
	
	/*
	* Vital
	*/
	GameplayTags.Attribute_Vital_Health = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Vital.Health"), FString("Character Health"));

	GameplayTags.Attribute_Vital_Mana = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Vital.Mana"), FString("Character Mana"));

	/*
	* Primary
	*/
	GameplayTags.Attribute_Primary_Strength = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Primary.Strength"), FString("Increase Physical Damage"));

	GameplayTags.Attribute_Primary_Intelligence = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Primary.Intelligence"), FString("Increase Magical Damage"));

	GameplayTags.Attribute_Primary_Resilience = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Primary.Resilience"), FString("Increase Armor Resistance"));

	GameplayTags.Attribute_Primary_Vigor = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Primary.Vigor"), FString("Increase Physical Stat"));

	/*
	* Secondary
	*/
	GameplayTags.Attribute_Secondary_Armor = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.Armor"), FString("Character Armor"));

	GameplayTags.Attribute_Secondary_ArmorPenetration = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.ArmorPenetration"), FString("Character Armor Penetration"));

	GameplayTags.Attribute_Secondary_BlockChance = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.BlockChance"), FString("Character BlockChance"));

	GameplayTags.Attribute_Secondary_CriticalHitChance = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitChance"), FString("Character CriticalHitChance"));

	GameplayTags.Attribute_Secondary_CriticalHitDamage = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitDamage"), FString("Character CriticalHitDamage"));

	GameplayTags.Attribute_Secondary_CriticalHitResistance = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.CriticalHitResistance"), FString("Character CriticalHitResistance"));

	GameplayTags.Attribute_Secondary_HealthRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.HealthRegeneration"), FString("Character HealthRegeneration"));

	GameplayTags.Attribute_Secondary_ManaRegeneration = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.ManaRegeneration"), FString("Character ManaRegeneration"));

	GameplayTags.Attribute_Secondary_MaxHealth = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxHealth"), FString("Character MaxHealth"));

	GameplayTags.Attribute_Secondary_MaxMana = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Secondary.MaxMana"), FString("Character MaxMana"));



	GameplayTags.Attribute_Resistance_Fire = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Resistance.Fire"), FString("Resistance Against Fire Damage"));

	GameplayTags.Attribute_Resistance_Lightning = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Resistance.Lightning"), FString("Resistance Against Lightning Damage"));

	GameplayTags.Attribute_Resistance_Arcane = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Resistance.Arcane"), FString("Resistance Against Arcane Damage"));

	GameplayTags.Attribute_Resistance_Physical = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Resistance.Physical"), FString("Resistance Against Physical Damage"));


	// Meta
	GameplayTags.Attribute_Meta_RewardXP = TagsManager.AddNativeGameplayTag(
		FName("Attribute.Meta.RewardXP"), FString("Reward XP"));



	/*
	* Input
	*/
	GameplayTags.Input_Action_LMB = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.LMB"), FString("Left Mouse Button Clicked"));

	GameplayTags.Input_Action_RMB = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.RMB"), FString("Right Mouse Button Clicked"));

	GameplayTags.Input_Action_Key1 = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.Key1"), FString("Keyboard 1 Clicked"));

	GameplayTags.Input_Action_Key2 = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.Key2"), FString("Keyboard 2 Clicked"));

	GameplayTags.Input_Action_Key3 = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.Key3"), FString("Keyboard 3 Clicked"));

	GameplayTags.Input_Action_Key4 = TagsManager.AddNativeGameplayTag(
		FName("Input.Action.Key4"), FString("Keyboard 4 Clicked"));

	GameplayTags.Input_Chat_Enter = TagsManager.AddNativeGameplayTag(
		FName("Input.Chat.Enter"), FString("Keyboard Enter Key For Chat"));

	GameplayTags.Input_Consume_Key5 = TagsManager.AddNativeGameplayTag(
		FName("Input.Consume.Key5"), FString("Keyboard 5 Clicked"));

	GameplayTags.Input_Consume_Key6 = TagsManager.AddNativeGameplayTag(
		FName("Input.Consume.Key6"), FString("Keyboard 6 Clicked"));

	GameplayTags.Input_Consume_Key7 = TagsManager.AddNativeGameplayTag(
		FName("Input.Consume.Key7"), FString("Keyboard 7 Clicked"));

	GameplayTags.Input_Consume_Key8 = TagsManager.AddNativeGameplayTag(
		FName("Input.Consume.Key8"), FString("Keyboard 8 Clicked"));

	GameplayTags.Input_Passive_1 = TagsManager.AddNativeGameplayTag(
		FName("Input.Passive.1"), FString("Passive Tag 1"));

	GameplayTags.Input_Passive_2 = TagsManager.AddNativeGameplayTag(
		FName("Input.Passive.2"), FString("Passive Tag 2"));


	// SlotTags

	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_LMB);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_RMB);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_Key1);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_Key2);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_Key3);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Action_Key4);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Passive_1);
	GameplayTags.SlotTags.Add(GameplayTags.Input_Passive_2);


	/*
	* Event
	*/
	GameplayTags.Event_Montage_FireBolt = TagsManager.AddNativeGameplayTag(
		FName("Event.Montage.FireBolt"), FString("Send Montage Event FireBolt"));

	GameplayTags.Event_Montage_Electrocute = TagsManager.AddNativeGameplayTag(
		FName("Event.Montage.Electrocute"), FString("Send Montage Event Electrocute"));

	GameplayTags.Event_Montage_ArcaneShards = TagsManager.AddNativeGameplayTag(
		FName("Event.Montage.ArcaneShards"), FString("Send Montage Event ArcaneShards"));

	/*
	* Damage
	*/
	GameplayTags.Damage = TagsManager.AddNativeGameplayTag(
		FName("Damage"), FString("Damage"));

	GameplayTags.Damage_Fire = TagsManager.AddNativeGameplayTag(
		FName("Damage.Fire"), FString("Fire Type Damage"));

	GameplayTags.Damage_Lightning = TagsManager.AddNativeGameplayTag(
		FName("Damage.Lightning"), FString("Lightning Type Damage"));

	GameplayTags.Damage_Arcane = TagsManager.AddNativeGameplayTag(
		FName("Damage.Arcane"), FString("Arcane Type Damage"));

	GameplayTags.Damage_Physical = TagsManager.AddNativeGameplayTag(
		FName("Damage.Physical"), FString("Physical Type Damage"));

	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attribute_Resistance_Fire);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Lightning, GameplayTags.Attribute_Resistance_Lightning);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attribute_Resistance_Arcane);
	GameplayTags.DamageTypeToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attribute_Resistance_Physical);


	/*
	* Debuff
	*/
	GameplayTags.Debuff = TagsManager.AddNativeGameplayTag(
		FName("Debuff"), FString("Debuff Parent Tag"));

	GameplayTags.Debuff_Burn = TagsManager.AddNativeGameplayTag(
		FName("Debuff.Burn"), FString("Debuff Burn By AttackType Fire"));

	GameplayTags.Debuff_Stun = TagsManager.AddNativeGameplayTag(
		FName("Debuff.Stun"), FString("Debuff Stun By AttackType Lightning"));

	GameplayTags.Debuff_Arcane = TagsManager.AddNativeGameplayTag(
		FName("Debuff.Arcane"), FString("Debuff Arcane By AttackType Arcane"));

	GameplayTags.Debuff_Physical = TagsManager.AddNativeGameplayTag(
		FName("Debuff.Physical"), FString("Debuff Physical By AttackType Physical"));

	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypeToDebuff.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);

	GameplayTags.Debuff_Chance = TagsManager.AddNativeGameplayTag(
		FName("Debuf.Chance"), FString("Debuff Activate Chance"));

	GameplayTags.Debuff_Duration = TagsManager.AddNativeGameplayTag(
		FName("Debuf.Duration"), FString("Debuff Activate Duration"));

	GameplayTags.Debuff_Damage = TagsManager.AddNativeGameplayTag(
		FName("Debuf.Damage"), FString("Debuff Activate Damage"));

	GameplayTags.Debuff_Frequency = TagsManager.AddNativeGameplayTag(
		FName("Debuf.Frequency"), FString("Debuff Activate Frequency"));

	/*
	* Effects_HitReact
	*/
	GameplayTags.Effects_HitReact = TagsManager.AddNativeGameplayTag(
		FName("Effects.HitReact"), FString("Hit React"));

	/*
	* Cooldown
	*/
	GameplayTags.Cooldown_Fire_FireBolt = TagsManager.AddNativeGameplayTag(
		FName("Cooldown.Fire.FireBolt"), FString("FireBolt Skill Cooldown"));

	GameplayTags.Cooldown_Lightning_Electrocute = TagsManager.AddNativeGameplayTag(
		FName("Cooldown.Lightning.Electrocute"), FString("Electrocute Skill Cooldown"));

	GameplayTags.Cooldown_Arcane_Shards = TagsManager.AddNativeGameplayTag(
		FName("Cooldown.Arcane.Shards"), FString("Arcane Shards Skill Cooldown"));

	/*
	* Abilities_Attack
	*/
	GameplayTags.Abilities_None = TagsManager.AddNativeGameplayTag(
		FName("Abilities.None"), FString("Abilities None"));

	GameplayTags.Abilities_Attack = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Attack"), FString("Abilities Attack"));

	GameplayTags.Abilities_Summon = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Summon"), FString("Abilities Summon"));

	GameplayTags.Abilities_HitReact = TagsManager.AddNativeGameplayTag(
		FName("Abilities.HitReact"), FString("Abilities HitReact"));



	GameplayTags.Abilities_Fire_FireBolt = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Fire.FireBolt"), FString("FireBolt Ability"));

	GameplayTags.Abilities_Lightning_Electrocute = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Lightning.Electrocute"), FString("Electrocute Ability"));

	GameplayTags.Abilities_Arcane_ArcaneShards = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Arcane.ArcaneShards"), FString("Spell ArcaneShards Ability"));

	// Ability Status

	GameplayTags.Abilities_Status_Locked = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Locked"), FString("Ability Status Locked"));

	GameplayTags.Abilities_Status_Eligible = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Eligible"), FString("Ability Status Eligible"));

	GameplayTags.Abilities_Status_UnLocked = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.UnLocked"), FString("Ability Status UnLocked"));

	GameplayTags.Abilities_Status_Equipped = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Status.Equipped"), FString("Ability Status Equipped"));

	// Ability Type
	
	GameplayTags.Abilities_Type_Offensive = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.Offensive"), FString("Ability Type Offensive"));

	GameplayTags.Abilities_Type_Passive = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.Passive"), FString("Ability Type Passive"));

	GameplayTags.Abilities_Type_None = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Type.None"), FString("Ability Type None"));

	// Passive Abilities

	GameplayTags.Abilities_Passive_HaloOfProtection = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.HaloOfProtection"), FString("Whole Protected for a moment"));

	GameplayTags.Abilities_Passive_HealthSiphon = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.HealthSiphon"), FString("Health Sihopn When Kill an Enemy"));

	GameplayTags.Abilities_Passive_ManaSiphon = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.ManaSiphon"), FString("Mana Sihopn When Kill an Enemy"));

	GameplayTags.Abilities_Passive_ListenForEvents = TagsManager.AddNativeGameplayTag(
		FName("Abilities.Passive.ListenForEvents"), FString("Character Startup Passive Skill"));

	/*
	* CombatSocket
	*/
	GameplayTags.CombatSocket_Weapon = TagsManager.AddNativeGameplayTag(
		FName("CombatSocket.Weapon"), FString("Attack By Weapon"));

	GameplayTags.CombatSocket_LeftHand = TagsManager.AddNativeGameplayTag(
		FName("CombatSocket.LeftHand"), FString("Attack By LeftHand"));

	GameplayTags.CombatSocket_RightHand = TagsManager.AddNativeGameplayTag(
		FName("CombatSocket.RightHand"), FString("Attack By RightHand"));

	GameplayTags.CombatSocket_Tail = TagsManager.AddNativeGameplayTag(
		FName("CombatSocket.Tail"), FString("Attack By RightHand"));

	/*
	* Tagged Montage
	*/

	GameplayTags.Montage_Attack1 = TagsManager.AddNativeGameplayTag(
		FName("Montage.Attack1"), FString("AttackMontage 1"));

	GameplayTags.Montage_Attack2 = TagsManager.AddNativeGameplayTag(
		FName("Montage.Attack2"), FString("AttackMontage 2"));

	GameplayTags.Montage_Attack3 = TagsManager.AddNativeGameplayTag(
		FName("Montage.Attack3"), FString("AttackMontage 3"));

	GameplayTags.Montage_Attack4 = TagsManager.AddNativeGameplayTag(
		FName("Montage.Attack4"), FString("AttackMontage 4"));

	/*
	* GC
	*/
	GameplayTags.GameplayCue_MeleeAttack = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.MeleeAttack"), FString("GC For MeleeAttack"));

	GameplayTags.GameplayCue_Summoning = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.Summoning"), FString("GC For Summoning"));

	GameplayTags.GameplayCue_ShockBurst = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.ShockBurst"), FString("GC For Shock Burst"));

	GameplayTags.GameplayCue_ShockLooping = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.ShockLooping"), FString("GC For Shock Looping"));

	GameplayTags.GameplayCue_ArcaneShards = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.ArcaneShards"), FString("GC For Arcane Shards"));

	GameplayTags.GameplayCue_BoxOpen = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.BoxOpen"), FString("GC For TreasureBox Open"));

	GameplayTags.GameplayCue_GetRareItem = TagsManager.AddNativeGameplayTag(
		FName("GameplayCue.GetRareItem"), FString("GC For Get Rare Item"));

	/*
	* Block
	*/
	GameplayTags.Player_Block_CursorTrace = TagsManager.AddNativeGameplayTag(
		FName("Player.Block.CursorTrace"), FString("Block CursorTrace"));

	GameplayTags.Player_Block_InputPressed = TagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputPressed"), FString("Block InputPressed"));

	GameplayTags.Player_Block_InputHeld = TagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputHeld"), FString("Block InputHeld"));

	GameplayTags.Player_Block_InputReleased = TagsManager.AddNativeGameplayTag(
		FName("Player.Block.InputReleased"), FString("Block InputReleased"));

	/*
	* Boss Enemy
	*/

	GameplayTags.Boss_Combo_Completed = TagsManager.AddNativeGameplayTag(
		FName("Boss.Combo.Completed"), FString("Boss Combo Attack Completed"));

	/*
	* Item
	*/
	GameplayTags.Item_Vacant = TagsManager.AddNativeGameplayTag(
		FName("Item.Vacant"), FString("Item Vacant"));

	GameplayTags.Item_Buyable_AddHealth = TagsManager.AddNativeGameplayTag(
		FName("Item.Buyable.AddHealth"), FString("Health Add Item"));

	GameplayTags.Item_Buyable_AddMana = TagsManager.AddNativeGameplayTag(
		FName("Item.Buyable.AddMana"), FString("Mana Add Item"));

	GameplayTags.Item_Gold = TagsManager.AddNativeGameplayTag(
		FName("Item.Gold"), FString("Item Gold"));

	GameplayTags.Item_Rare_Collecting = TagsManager.AddNativeGameplayTag(
		FName("Item.Rare.Collecting"), FString("Rare Item For Collecting"));


	// Item Ingredient
	GameplayTags.Item_Ingredient_Earth = TagsManager.AddNativeGameplayTag(
		FName("Item.Ingredient.Earth"), FString("Item Ingredient Earth"));

	GameplayTags.Item_Ingredient_Fire = TagsManager.AddNativeGameplayTag(
		FName("Item.Ingredient.Fire"), FString("Item Ingredient Fire"));

	GameplayTags.Item_Ingredient_Water = TagsManager.AddNativeGameplayTag(
		FName("Item.Ingredient.Water"), FString("Item Ingredient Water"));

	GameplayTags.Item_Ingredient_Wind = TagsManager.AddNativeGameplayTag(
		FName("Item.Ingredient.Wind"), FString("Item Ingredient Wind"));


	// Item Collecting

	GameplayTags.Item_Collecting_Stone = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.Stone"), FString("Item Collecting Stone"));

	GameplayTags.Item_Collecting_Elixir = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.Elixir"), FString("Item Collecting Elixir"));

	GameplayTags.Item_Collecting_DragonsBlood = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.DragonsBlood"), FString("Item Collecting DragonsBlood"));

	GameplayTags.Item_Collecting_Mercury = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.Mercury"), FString("Item Collecting Mercury"));

	GameplayTags.Item_Collecting_Salt = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.Salt"), FString("Item Collecting Salt"));

	GameplayTags.Item_Collecting_AlchemistsFire = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.AlchemistsFire"), FString("Item Collecting AlchemistsFire"));

	GameplayTags.Item_Collecting_HeartOfImmortality = TagsManager.AddNativeGameplayTag(
		FName("Item.Collecting.HeartOfImmortality"), FString("Item Collecting HeartOfImmortality"));

	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_Stone);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_Elixir);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_DragonsBlood);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_Mercury);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_Salt);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_AlchemistsFire);
	GameplayTags.CollectingItemTags.Add(GameplayTags.Item_Collecting_HeartOfImmortality);

	GameplayTags.bLoaded = true;
}
