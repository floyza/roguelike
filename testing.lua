local item_table_addition={
  ["on_move"] = Item.new(Trigger.ON_MOVE, "on_move", "on_move", 0),
  ["on_kill"] = Item.new(Trigger.ON_KILL, "on_kill", "on_kill", 0),
  ["on_hit"] = Item.new(Trigger.ON_HIT, "on_hit", "on_hit", 0),
  ["on_turn"] = Item.new(Trigger.ON_TURN, "on_turn", "on_turn", 0),
  ["on_dam"] = Item.new(Trigger.ON_DAM, "on_dam", "on_dam", 0),
  ["dam_mod"] = Item.new(Trigger.DAM_MOD, "dam_mod", "dam_mod", 0),
  ["dam_reduce"] = Item.new(Trigger.DAM_REDUCE, "dam_reduce", "dam_reduce", 0)
}

for k,v in pairs(item_table_addition) item_table[k] = v end

--[[
add this to main.cpp:
  g->you->aquire("on_move");
  g->you->aquire("on_turn");
  g->you->aquire("on_dam");
  g->you->aquire("on_kill");
  g->you->aquire("on_hit");
  g->you->aquire("dam_mod");
  g->you->aquire("dam_reduce");
]]

function on_move()
  print("on move")
end

function on_turn()
  print("on turn")
  print()
end

function on_dam(c)
  print("on dam")
end

function on_kill(c)
  print("on kill")
end

function on_hit(c)
  print("on hit")
end

function dam_mod(i,c)
  print("dam mod")
  return i
end

function dam_reduce(i,c)
  print("dam reduce")
  return i
end
