#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <functional>
#include <string>
#include <type_traits>
#include <variant>
#include <stdexcept>

class Creature;

enum class Trigger { DAM_MOD, DAM_REDUCE, ON_ATTACK, ON_HIT, ON_KILL, ON_MOVE, ON_TURN, ON_DAM, last };

class Item {
public:
  typedef std::function<void(void)> generic_func;
  typedef std::function<void(Creature &)> target_generic_func;
  typedef std::function<void(int &)> modify_func;
  typedef std::function<void(int &,Creature &)> target_modify_func;
  Item(const std::string &id);
  Item(const std::string &id, int x, int y);

  // template<typename Args...>
  // call_effect(Args... args);
  std::variant<modify_func, generic_func, target_modify_func, target_generic_func> effect;

  Trigger trigger;
  std::string name;
  void draw() const;
  int x,y;
  ~Item();
};

template<Trigger trigger>
struct Trigger_type_impl {
  typedef void type;
};

template<>
struct Trigger_type_impl<Trigger::ON_MOVE> {
  typedef Item::generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::ON_TURN> {
  typedef Item::generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::ON_HIT> {
  typedef Item::target_generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::ON_ATTACK> {
  typedef Item::target_generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::ON_KILL> {
  typedef Item::target_generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::ON_DAM> {
  typedef Item::target_generic_func type;
};

template<>
struct Trigger_type_impl<Trigger::DAM_MOD> {
  typedef Item::target_modify_func type;
};

template<>
struct Trigger_type_impl<Trigger::DAM_REDUCE> {
  typedef Item::target_modify_func type;
};

template <Trigger trigger>
using Trigger_type = typename Trigger_type_impl<trigger>::type;



template <Trigger trigger, typename T>
constexpr bool is_of_type = std::is_same<Trigger_type<trigger>, T>::value;

template<typename T, int N, bool... Rest>
struct Array_impl {
  static constexpr auto& value = Array_impl<T, N-1, is_of_type<static_cast<Trigger>(N), T>, Rest...>::value;
};

template<typename T, bool... Rest>
struct Array_impl<T, 0, Rest...> {
  static constexpr bool value[] = { is_of_type<static_cast<Trigger>(0), T>, Rest... };
};

template<typename T, bool... Rest>
constexpr bool Array_impl<T, 0, Rest...>::value[];

template<typename T, int N>
struct Array {
  static_assert(N >= 0, "N must be at least 0");

  static constexpr auto& value = Array_impl<T, N>::value;

  Array() = delete;
  Array(const Array&) = delete;
  Array(Array&&) = delete;
};

template<typename T>
bool is_trigger_type(Trigger trigger) {
  return Array<T, (int)Trigger::last>::value[static_cast<int>(trigger)];
}

#endif //DEFINED_ITEM_HPP
