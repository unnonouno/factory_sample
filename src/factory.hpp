#ifndef FACTORY_HPP_747F780A_951D_4489_9502_F56051AA6C40_
#define FACTORY_HPP_747F780A_951D_4489_9502_F56051AA6C40_

#include <map>
#include <stdexcept>
#include <string>

#include <pficommon/lang/function.h>
#include <pficommon/lang/shared_ptr.h>
#include <pficommon/text/json.h>

template <typename T>
class factory {
 public:
  typedef pfi::lang::shared_ptr<factory> ptr;

  virtual ~factory() {}

  virtual pfi::lang::function<pfi::lang::shared_ptr<T>()> make_generator(
      const pfi::text::json::json& config) const = 0;
};

template <typename T>
class factory_manager {
 public:
  typedef typename factory<T>::ptr factory_ptr;

  void register_factory(
      const std::string& name,
      factory_ptr factory) {
    if (factories_.count(name)) {
      // TODO: change erro type
      throw std::runtime_error(name + " is already registered.");
    } else {
      factories_.insert(std::make_pair(name, factory));
    }
  }

  pfi::lang::function<pfi::lang::shared_ptr<T>()> make_generator(
      const std::string& name,
      const pfi::text::json::json& config) const {
    typename std::map<std::string, factory_ptr>::const_iterator it
        = factories_.find(name);
    if (it == factories_.end()) {
      // TODO: change error type
      throw std::runtime_error("Unknonw name: " + name);
    } else {
      return it->second->make_generator(config);
    }
  }

  pfi::lang::shared_ptr<T> create(
      const std::string& name,
      const pfi::text::json::json& config) const {
    return make_generator(name, config)();
  }

 private:
  std::map<std::string, factory_ptr> factories_;
};

#endif  // FACTORY_HPP_747F780A_951D_4489_9502_F56051AA6C40_
