#pragma once

#include <cstdint>

namespace EcoSystem {

// Уникальный идентификатор для существ
class EntityID {
private:
    static uint64_t s_NextID;
    uint64_t m_ID;
    
public:
    EntityID() : m_ID(0) {}
    explicit EntityID(uint64_t id) : m_ID(id) {}
    
    static EntityID Generate() {
        return EntityID(++s_NextID);
    }
    
    uint64_t getValue() const { return m_ID; }
    
    bool operator==(const EntityID& other) const {
        return m_ID == other.m_ID;
    }
    
    bool operator!=(const EntityID& other) const {
        return m_ID != other.m_ID;
    }
    
    bool operator<(const EntityID& other) const {
        return m_ID < other.m_ID;
    }
    
    explicit operator bool() const {
        return m_ID != 0;
    }
};

// Инициализация статического счетчика
inline uint64_t EntityID::s_NextID = 0;

} // namespace EcoSystem
