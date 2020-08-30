#include "player.hpp"
#include "creature.hpp"
#include "game.hpp"
#include "lua.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "monster.hpp"
#include "item.hpp"
#include "status.hpp"
#include "input_handler.hpp"
#include "command.hpp"
#include <cassert>
#include <memory>
#include <string>

Player::Player(char icon, const TCODColor &color, int max_hp, int attack, const Pos &pos)
  : Creature(icon, color, max_hp, attack, pos), current_input{std::make_unique<Player_input_handler>(*this)}
{
}

Player::~Player() = default;

void Player::do_turn() {
  bool doing_actions=true;
  while (doing_actions && !TCODConsole::isWindowClosed()) {
    Command &action = current_input->get_input();
    int cost = action.execute();
    doing_actions = cost==0;
  }
  call_triggers<Trigger::ON_TURN>();
  for (Status &status : statuses) {
    status.tick();
  }
  ++total_turns;
}

bool Player::do_move(const Pos &new_pos) {
  for (Monster &mon : game->map->monsters) {
    if (mon.pos == new_pos) {
      do_attack(mon);
      return true;
    }
  }
  if (game->map->is_walkable(new_pos)) {
    pos = new_pos;
    call_triggers<Trigger::ON_MOVE>();
    return true;
  } else {
    return false;
  }
}

void Player::do_attack(Creature &target, bool triggers) {
  if (triggers)
    call_triggers<Trigger::ON_ATTACK>(std::ref(target));

  int damage = attack;
  call_triggers<Trigger::DAM_MOD>(std::ref(damage), std::ref(target));
  target.take_damage(damage, *this);

  if (triggers)
    call_triggers<Trigger::ON_HIT>(std::ref(target));
}

void Player::take_damage(int amount, Player &source) {
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(source));
  game->send_msg({"You hit yourself for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
  call_triggers<Trigger::ON_DAM>(std::ref(source));
}

void Player::take_damage(int amount, Monster &source) {
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(source));
  game->send_msg({"The " + source.name() + " attacks you for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
  call_triggers<Trigger::ON_DAM>(std::ref(source));
}

void Player::take_damage(int amount) {
  Monster dummy = Monster{0, *game->map}; // dummy monster
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(dummy));/*quick fix, TODO: probably have an actual source*/
  hp -= amount;
  if (hp<=0)
    die();  
  call_triggers<Trigger::ON_DAM>(std::ref(dummy));
}

void Player::die() {
  game->send_msg({"You are dead...", TCODColor::red});
  dead = true;
};

void Player::aquire_item(int id) {
  items.emplace_back(id);
}

void Player::aquire_item(const std::string &name) {
  items.emplace_back(name);
}

void Player::aquire_item(const Lua_item &base) {
  items.emplace_back(base);
}

void Player::aquire_status(int id) {
  statuses.emplace_back(id);
}

void Player::aquire_status(const std::string &name) {
  statuses.emplace_back(name);
}

void Player::aquire_status(const Lua_status &base) {
  statuses.emplace_back(base);
}

void Player::remove_status(int id) {
  auto iter = std::remove_if(statuses.begin(), statuses.end(), [id](const Status &s){return s.id() == id;});
  statuses.erase(iter, statuses.end());
}

void Player::remove_status(const std::string &name) {
  auto iter = std::remove_if(statuses.begin(), statuses.end(), [&name](const Status &s){return s.name() == name;});
  statuses.erase(iter, statuses.end());
}

int Player::turn_count() const {
  return total_turns;
}
