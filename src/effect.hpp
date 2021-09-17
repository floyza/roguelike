#ifndef DEFINED_EFFECT_HPP
#define DEFINED_EFFECT_HPP

#include <cassert>
#include <functional>
#include <string>
#include <variant>

class Creature;

enum class Trigger {
  DAM_MOD,
  DAM_REDUCE,
  ON_ATTACK,
  ON_HIT,
  ON_KILL,
  ON_MOVE,
  ON_TURN,
  ON_DAM,
  last
};

struct Effect {
  typedef std::function<void(void)> generic_func;
  typedef std::function<void(Creature &)> target_generic_func;
  typedef std::function<void(int &)> modify_func;
  typedef std::function<void(int &, Creature &)> target_modify_func;

  std::variant<modify_func, generic_func, target_modify_func,
               target_generic_func>
      func;
  Trigger trigger;

public:
  Effect() = default;
  Effect(Trigger trigger, const std::string &func_name);

  template <Trigger T, typename... Args> void call_effect(Args... args);
  Trigger get_trigger();
};

template <Trigger trigger> struct Trigger_type_impl { typedef void type; };

template <> struct Trigger_type_impl<Trigger::ON_MOVE> {
  typedef Effect::generic_func type;
};

template <> struct Trigger_type_impl<Trigger::ON_TURN> {
  typedef Effect::generic_func type;
};

template <> struct Trigger_type_impl<Trigger::ON_HIT> {
  typedef Effect::target_generic_func type;
};

template <> struct Trigger_type_impl<Trigger::ON_ATTACK> {
  typedef Effect::target_generic_func type;
};

template <> struct Trigger_type_impl<Trigger::ON_KILL> {
  typedef Effect::target_generic_func type;
};

template <> struct Trigger_type_impl<Trigger::ON_DAM> {
  typedef Effect::target_generic_func type;
};

template <> struct Trigger_type_impl<Trigger::DAM_MOD> {
  typedef Effect::target_modify_func type;
};

template <> struct Trigger_type_impl<Trigger::DAM_REDUCE> {
  typedef Effect::target_modify_func type;
};

template <Trigger trigger>
using Trigger_type = typename Trigger_type_impl<trigger>::type;

template <Trigger trigger, typename T>
constexpr bool is_of_type = std::is_same<Trigger_type<trigger>, T>::value;

// compute an table of trigger types ahead of time
// https://stackoverflow.com/questions/19019252/create-n-element-constexpr-array-in-c11

template <typename T, int N, bool... Rest> struct Trigger_array_impl {
  static constexpr auto &value =
      Trigger_array_impl<T, N - 1, is_of_type<static_cast<Trigger>(N), T>,
                         Rest...>::value;
};

template <typename T, bool... Rest> struct Trigger_array_impl<T, 0, Rest...> {
  static constexpr bool value[] = {is_of_type<static_cast<Trigger>(0), T>,
                                   Rest...};
};

template <typename T, bool... Rest>
constexpr bool Trigger_array_impl<T, 0, Rest...>::value[];

template <typename T, int N> struct Trigger_array {
  static_assert(N >= 0, "N must be at least 0");

  static constexpr auto &value = Trigger_array_impl<T, N>::value;

  Trigger_array() = delete;
  Trigger_array(const Trigger_array &) = delete;
  Trigger_array(Trigger_array &&) = delete;
};

template <typename T> bool is_trigger_type(Trigger trigger) {
  return Trigger_array<T, (int)Trigger::last>::value[static_cast<int>(trigger)];
}

template <Trigger T, typename... Args> void Effect::call_effect(Args... args) {
  // we have to take a template argument of our trigger because it needs
  // to be constexpr
  assert(T == trigger);
  auto x = std::get<Trigger_type<T>>(func);
  x(args...);
}

#endif // DEFINED_EFFECT_HPP
