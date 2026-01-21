#pragma once

#include "fetch.h"

// dependency cycle breaker
namespace SwosVM {
    extern uint32_t ptrToOffset(const void *ptr);
    extern char *offsetToPtr(uint32_t offset);
    using VoidFunction = void(*)();
    extern VoidFunction fetchProc(int index);
    extern void invokeProc(int index);
    enum Offsets : uint32_t;
    enum class Procs : int;
}

template<typename Type>
class SwosDataPointer
{
public:
    SwosDataPointer(Type *t) {
        assert(reinterpret_cast<uintptr_t>(&m_offset) % 4 == 0);
        m_offset = SwosVM::ptrToOffset(t);
    }
    constexpr SwosDataPointer() : m_offset(0) {}
    constexpr SwosDataPointer(uint32_t offset) : m_offset(offset) {}
    constexpr SwosDataPointer(SwosVM::Offsets offset) : m_offset(static_cast<uint32_t>(offset)) {}
    bool isNull() const {
        return !m_offset || m_offset == -1;
    }
    SwosDataPointer& operator=(Type *t) {
        assert(reinterpret_cast<uintptr_t>(&m_offset) % 4 == 0);
        m_offset = SwosVM::ptrToOffset(t);
        return *this;
    }
    template<typename PtrType, std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
    void loadFrom(PtrType ptr) {
        auto offset = ::fetch((uint32_t *)ptr);
        assert(offset == -1 || (SwosVM::offsetToPtr(offset), true));
        store(&m_offset, offset);
    }
    void set(uint32_t offset) {
        assert((SwosVM::offsetToPtr(offset), true));
        store(&m_offset, offset);
    }
    void setRaw(uint32_t val) {
        store(&m_offset, val);
    }
    void set(Type *t) {
        auto offset = SwosVM::ptrToOffset(t);
        store(&m_offset, offset);
    }
    void reset() {
        store(&m_offset, 0);
    }
    operator Type *() { return get(); }
    operator const Type *() const { return get(); }
    Type *operator->() { return get(); }
    const Type *operator->() const { return get(); }
    SwosDataPointer operator++(int) {
        auto tmp(*this);
        m_offset += sizeof(Type);
        assert((SwosVM::offsetToPtr(m_offset), true));
        return tmp;
    }
    SwosDataPointer operator++() {
        return operator+=(sizeof(Type));
    }
    SwosDataPointer operator--(int) {
        auto tmp(*this);
        m_offset -= sizeof(Type);
        assert((SwosVM::offsetToPtr(m_offset), true));
        return tmp;
    }
    SwosDataPointer& operator--() {
        return operator+=(-static_cast<int>(sizeof(Type)));
    }
    SwosDataPointer& operator+=(int inc) {
        m_offset += inc;
        assert((SwosVM::offsetToPtr(m_offset), true));
        return *this;
    }
    SwosDataPointer& operator-=(int inc) {
        return operator+=(-inc);
    }
    template <typename T> T as() { return reinterpret_cast<T>(get()); }
    template <typename T = Type *> T asAligned() const {
        auto offset = ::fetch(&m_offset);
        assert((SwosVM::offsetToPtr(offset), true));
        return reinterpret_cast<T>(SwosVM::offsetToPtr(offset));
    }
    uint32_t asAlignedDword() const {
        return ::fetch(&m_offset);
    }
    std::decay_t<Type> fetch() const {
        return ::fetch(asAligned());
    }
    const Type *asConst() const { return get(); }
    Type *asPtr() const { return (Type *)get(); }
    const char *asCharPtr() const { return (const char *)get(); }
    const char *asAlignedConstCharPtr() const { return asAligned<const char *>(); }
    char *asAlignedCharPtr() { return asAligned<char *>(); }
    char *asCharPtr() { return (char *)get(); }
    const char *asConstCharPtr() const { return (char *)get(); }
    uint32_t getRaw() const { return m_offset; }

private:
    Type *get() const {
        assert(reinterpret_cast<uintptr_t>(&m_offset) % 4 == 0);
        return reinterpret_cast<Type *>(SwosVM::offsetToPtr(m_offset));
    }

    uint32_t m_offset;
};

static_assert(sizeof(SwosDataPointer<char>) == 4, "SWOS VM data pointers must be 32-bit");

class SwosProcPointer
{
    int32_t m_index;
public:
    SwosProcPointer() = default;
    constexpr SwosProcPointer(int index) : m_index(index) {}
    constexpr SwosProcPointer(SwosVM::Procs index) : m_index(static_cast<int>(index)) {}
    int32_t index() const { return m_index; }
    SwosProcPointer& operator=(const SwosProcPointer *other) {
        assert(reinterpret_cast<uintptr_t>(&m_index) % 4 == 0);
        assert(!other || SwosVM::fetchProc(other->m_index));
        m_index = other ? other->m_index : -1;
        return *this;
    }
    template<typename PtrType, std::enable_if_t<std::is_pointer<PtrType>::value, int> = 0>
    void loadFrom(PtrType ptr) {
        auto index = fetch((int *)ptr);
        assert(SwosVM::fetchProc(index));
        store(&m_index, index);
    }
    void set(int index) {
        assert(SwosVM::fetchProc(index));
        store(&m_index, index);
    }
    void clearAligned() {
        store(&m_index, 0);
    }
    explicit operator bool() const { return !empty(); }
    bool empty() const { return !SwosVM::fetchProc(m_index); }
    void operator()() { SwosVM::invokeProc(m_index); }
    bool operator==(SwosVM::VoidFunction proc) const {
        return SwosVM::fetchProc(m_index) == proc;
    }
};
