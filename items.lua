item_table={
  ["basic_armor"] = Item.new(Trigger.DAM_REDUCE, "armor_apply", "Armor", 0),
  ["stealth_kit"] = Item.new(Trigger.DAM_MOD, "stealth_attack", "Stealth Kit", 0)
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
  if ( you.turn_count >= last_attack+stealth_gap ) then
    last_attack = you.turn_count
    damage = damage * 1.5
  end
  return damage
end

function overflow_heal(target)
  -- ON_KILL
  you.hp = you.hp - target.hp
end

function thorns(target)
  -- ON_HIT
  you:do_attack_sans_triggers(target)
end
