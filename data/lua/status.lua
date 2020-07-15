status_definitions = {
  {
    name = "Burn",
    trigger = Trigger.ON_MOVE,
    effect = "take_damage",
    duration = 10
  }
}

function take_damage()
  game:send_msg(Message.new("You take 2 damage from burning!"))
  game.you:take_damage(2)
end
