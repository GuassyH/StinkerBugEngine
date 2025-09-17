#ifndef TYPE_ID_GETTER
#define TYPE_ID_GETTER

#include <cstdint>

inline uint32_t g_componentTypeCounter = 0;

template <typename T>
uint32_t TypeID() {
    static uint32_t id = g_componentTypeCounter++;
    return id;
}

template <typename T>
uint32_t ComponentBit() {
    return 1u << TypeID<T>();
}

#endif