#include "effect.hpp"
#include "game.hpp"
#include "lua.hpp"
#include "creature.hpp"

Effect::Effect(Trigger trigger, const std::string &func_name)
  : trigger(trigger)
{
  if (is_trigger_type<modify_func>(trigger))
    func = modify_func([func_name](int &i) {
			 auto result = game->lua_manager->get_func(func_name)(i);
			 if (result.valid())
			   i = result;
                         else
                           throw sol::error { result };
		       });
  else if (is_trigger_type<generic_func>(trigger))
    func = generic_func([func_name] {
			  auto result = game->lua_manager->get_func(func_name)();
			  if(!result.valid())
                            throw sol::error { result };
			});
  else if (is_trigger_type<target_modify_func>(trigger))
    func = target_modify_func([func_name](int &i, Creature &c) {
				auto result = game->lua_manager->get_func(func_name)(i, c);
				if (result.valid())
				  i=result;
				else
                                  throw sol::error { result };
			      });
  else if (is_trigger_type<target_generic_func>(trigger))
    func = target_generic_func([func_name](Creature &c) {
				 auto result = game->lua_manager->get_func(func_name)(c);
				 if(!result.valid())
                                   throw sol::error { result };
			       });
}

Trigger Effect::get_trigger() {
  return trigger;
}
