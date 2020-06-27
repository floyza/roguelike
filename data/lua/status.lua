status_table={
  ["burn"] = Status.new(Trigger.ON_MOVE, "take_damage", "Burn", 10)
}

function take_damage()
  game:send_msg(Message.new("You take 2 damage from burning!"))
  game.you:take_damage(2)
end
