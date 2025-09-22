#pragma once

#include <memory>
#include <stdexcept>
#include <typeindex>
#include <unordered_map>

class ServiceLocator {
private:
  static inline std::unordered_map<std::type_index, std::shared_ptr<void>>
      services;

public:
  template <typename T, typename... Args> static void Register(Args &&...args) {
    services[typeid(T)] = std::make_shared<T>(std::forward<Args>(args)...);
  }

  template <typename T> static std::shared_ptr<T> Get() {
    auto it = services.find(typeid(T));
    if (it != services.end()) {
      return std::static_pointer_cast<T>(it->second);
    }
    throw std::runtime_error("Service not registered");
  }

  template <typename T> static void Unregister() { services.erase(typeid(T)); }
};