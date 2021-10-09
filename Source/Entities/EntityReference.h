#pragma once
#include <memory>

template<typename E>
class EntityReference {
    private:
        std::shared_ptr<E> pointer;

        template<typename T>
        static std::shared_ptr<E> CastReference(EntityReference<T> reference) {
            static_assert(T::IsOfType(E::Type), "Can not cast polymorphically incompatible types.");
            return std::dynamic_pointer_cast<E>(reference.Pointer());
        }
    public:
        EntityReference(E* entity) : pointer{std::unique_ptr<E>(entity)} {}

        template<typename T>
        EntityReference(EntityReference<T> reference) : pointer{CastReference(reference)} {}

        E& Value() { return *pointer; }
        const E& Value() const { return *pointer; }

        E* operator->() { return pointer.get(); }
        const E* operator->() const { return pointer.get(); }
        operator E*() { return pointer.get(); }

        std::shared_ptr<E> Pointer() { return pointer; }
        const std::shared_ptr<E> Pointer() const { return pointer; }
        std::size_t Usages() const { return pointer.use_count(); }
};