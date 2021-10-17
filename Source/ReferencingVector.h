#pragma once
#include <vector>
#include <memory>

template<typename T>
class ReferencingVector {
    public:
        struct Reference {
            std::shared_ptr<std::size_t> index;
            Reference(std::size_t vIndex) : index{new std::size_t} {
                *index = vIndex;
            }

            std::size_t Index() const { return *index; }
            T& Value(ReferencingVector<T>& vector) { return vector.At(*index); }
            const T& Value(const ReferencingVector<T>& vector) const { return vector.Data().At(*index); }
        };
    private:
        std::vector<T> data;
        std::vector<Reference> references;

        void ShiftReferences(std::size_t index, int direction) {
            for (auto reference : references) {
                if ((*reference.index) >= index) {
                    *reference.index += direction;
                }
            }
        }
    public:
        ReferencingVector() {}
        ReferencingVector(const std::vector<T>& vData) : data{vData} {}

        Reference ReferenceAt(std::size_t index) {
            for (auto reference : references) {
                if ((*reference.index) == index) {
                    return reference;
                }
            }
            Reference reference(index);
            references.push_back(reference);
            return reference;
        }
        void Insert(std::size_t index, const std::vector<T>& values) {
            ShiftReferences(index, values.size());
            data.insert(data.begin() + index, values.begin(), values.end());
        }

        void Erase(std::size_t index, std::size_t count) {
            ShiftReferences(index, -((int)count));
            data.erase(data.begin() + index, data.begin() + (index + count));
        }

        void PushBack(const T& value) { data.push_back(value); }
        T PopBack() { return data.pop_back(); }
        T& At(std::size_t index) { return data.at(index); }
        std::vector<T>& Data() { return data; }
};