#ifndef COMPONENT_TYPE_ID_GETTER
#define COMPONENT_TYPE_ID_GETTER

#include <cstdint>

inline uint32_t g_componentTypeCounter = 0;

template <typename T>
uint32_t ComponentTypeID() {
    static uint32_t id = g_componentTypeCounter++;
    return id;
}

template <typename T>
uint32_t ComponentBit() {
    return 1u << ComponentTypeID<T>();
}

#endif