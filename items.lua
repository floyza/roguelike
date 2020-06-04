item_table={["armor"] = Item.new(Trigger.DAM_REDUCE, "armor_apply")}

function armor_apply(armor_val)
  if ( armor_val == 0 ) then return 0 end
  return armor_val - 1
end

