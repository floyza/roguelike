#include "effect.hpp"
#include "game.hpp"
#include "lua.hpp"
#include "creature.hpp"

Effect::Effect(Trigger trigger, const std::string &func_name)
  : trigger(trigger)
{
  if (is_trigger_type<modify_func>(trigger))
    func = modify_func([&func_name](int &i) { i = (*game->lua_state)[func_name](i); });
  else if (is_trigger_type<generic_func>(trigger))
    func = generic_func([&func_name] { (*game->lua_state)[func_name](); });
  else if (is_trigger_type<target_modify_func>(trigger))
    func = target_modify_func([&func_name](int &i, Creature &c) { i = (*game->lua_state)[func_name](i, c); });
  else if (is_trigger_type<target_generic_func>(trigger))
    func = target_generic_func([&func_name](Creature &c) { (*game->lua_state)[func_name](c); });
}

Trigger Effect::get_trigger() {
  return trigger;
}