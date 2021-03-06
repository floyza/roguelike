item_definitions = {
  {
    name = "Armor",
    trigger = Trigger.DAM_REDUCE,
    effect = "armor_apply",
    rarity = 0
  },
  {
    name = "Stealth Kit",
    trigger = Trigger.DAM_MOD,
    effect = "stealth_attack",
    rarity = 1
  },
  {
    name = "Overflow Heal",
    trigger = Trigger.ON_KILL,
    effect = "overflow_heal",
    rarity = 2
  },
  {
    name = "Thorns",
    trigger = Trigger.ON_HIT,
    effect = "thorns",
    rarity = 3
  },
  {
    name = "Recursive Attack",
    trigger = Trigger.ON_HIT,
    effect = "recursive_attack",
    rarity = 4
  },
  {
    name = "Test Item",
    trigger = Trigger.ON_HIT,
    effect = "test_effect",
    rarity = 10000
  }
}

function armor_apply(armor_val, target)
  -- DAM_REDUCE
  if ( armor_val == 0 ) then return 0 end
  return armor_val - 1
end

last_attack=-math.huge

function stealth_attack(damage, target)
  -- DAM_MOD
  local stealth_gap=3
  if ( game.you.turn_count >= last_attack+stealth_gap ) then
    last_attack = game.you.turn_count
    damage = damage * 1.5
  end
  return math.floor(damage)
end

function overflow_heal(target)
  -- ON_KILL
  game.you.hp = game.you.hp - target.hp
end

function thorns(target)
  -- ON_HIT
  game.you:do_attack_sans_triggers(target)
end

function recursive_attack(target)
  -- ON_HIT
  local attack_amount = 1.1
  local chance = 1 - (1/attack_amount)
  if ( percent_chance(chance) ) then
    game.you:do_attack(target)
  end
end

function test_effect(target)
	-- ON_HIT
	game.you:do_attack(target,false)
end
