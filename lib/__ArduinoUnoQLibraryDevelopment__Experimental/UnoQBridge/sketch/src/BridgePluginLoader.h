#pragma once

#include <Arduino.h>

#if defined(__has_include)
#  if __has_include(<Arduino_RouterBridge.h>)
#    include <Arduino_RouterBridge.h>
#  endif
#endif

#ifndef UNOQ_BRIDGE_CUSTOM_REGISTER
#  if defined(__has_include) && __has_include(<Arduino_RouterBridge.h>)
#    define UNOQ_BRIDGE_CUSTOM_REGISTER(name, fn) Bridge.provide_safe(name, fn)
#  else
#    define UNOQ_BRIDGE_CUSTOM_REGISTER(name, fn) ((void)0)
#  endif
#endif

#ifndef UNOQ_BRIDGE_MAX_PLUGINS
#   define UNOQ_BRIDGE_MAX_PLUGINS 150
#endif

#ifdef OVERRIDE_UNOQ_BRIDGE_MAX_PLUGINS
#  define UNOQ_BRIDGE_MAX_PLUGINS OVERRIDE_UNOQ_BRIDGE_MAX_PLUGINS
#endif
namespace UnoQBridgePluginLoader {

using BridgeRegistrar = void (*)();

struct PluginEntry {
    const char* name;
    BridgeRegistrar registrar;
};

class Registry {
public:
    static constexpr size_t kMaxPlugins = UNOQ_BRIDGE_MAX_PLUGINS;

    static void add(const char* name, BridgeRegistrar registrar) {
        if (_count < kMaxPlugins) {
            _entries[_count].name = name;
            _entries[_count].registrar = registrar;
            ++_count;
        }
    }

    static void registerAll() {
        for (size_t i = 0; i < _count; ++i) {
            if (_entries[i].registrar != nullptr) {
                _entries[i].registrar();
            }
        }
    }

    static size_t count() {
        return _count;
    }

private:
    static PluginEntry _entries[kMaxPlugins];
    static size_t _count;
};

class RegisterHook {
public:
    RegisterHook(const char* name, BridgeRegistrar registrar) {
        Registry::add(name, registrar);
    }
};

namespace Backend {
    template <typename Fn>
    inline void registerFunction(const char* name, Fn fn) {
        UNOQ_BRIDGE_CUSTOM_REGISTER(name, fn);
    }
}  // namespace Backend

}  // namespace UnoQBridgePluginLoader

#define UNOQ_BRIDGE_REGISTER_FUNCTION(name, fn) \
    namespace { \
        void register_bridge_plugin_##name() { \
            UnoQBridgePluginLoader::Backend::registerFunction(#name, fn); \
        } \
        static UnoQBridgePluginLoader::RegisterHook register_hook_##name(#name, &register_bridge_plugin_##name); \
    }

#define UNOQ_BRIDGE_REGISTER_ALL() \
    UnoQBridgePluginLoader::Registry::registerAll();
