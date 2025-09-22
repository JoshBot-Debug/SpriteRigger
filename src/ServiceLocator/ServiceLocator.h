#pragma once

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

class ServiceLocator {
private:
  static inline std::unordered_map<std::type_index, std::shared_ptr<void>>
      s_Services;

public:
  template <typename T, typename... Args> static void Register(Args &&...args) {
    s_Services[typeid(T)] = std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T> static std::shared_ptr<T> Get() {
    return std::static_pointer_cast<T>(s_Services.at(typeid(T)));
  }

  template <typename T> static void Unregister() {
    s_Services.erase(typeid(T));
  }
};