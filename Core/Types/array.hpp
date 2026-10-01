/************************************/
/*           array.hpp              */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "variant.hpp"
#include <algorithm>
#include <vector>

class Array : public Variant {
    private:
    // A dynamic array that holds any type that extends from Variant.
    std::vector<Variant> data;
    /*------------------------------------------------------------------*/

    public:
    // Default constructor.
    Array() = default;
    // Constructor that takes a vector of Variant elements.
    Array(std::vector<Variant> elements) : data(elements) {}
    // Constructor that takes a vector of Variant elements and an accessability parameter.
    Array(std::vector<Variant> elements, Accessability p_access) : data(elements) { access = p_access; }
    /*-----------------------------------------------------------------------------------------------------*/

    // Returns the number of elements in the array.
    func unsigned int size() const { return is_write_only() ? 0 : data.size(); }
    /*----------------------------------------------------------------------------*/

    // Returns the element at the specified index.
    // If the index is out of bounds, it'll return an empty Variant.
    func Variant operator[](unsigned int index) { return is_write_only() ? Variant() : (index > size() ? data[size() - 1] : data[index]); }
    // Returns the element at the specified index.
    // If the index is out of bounds, it'll return an empty Variant.
    func const Variant operator[](unsigned int index) const { return is_write_only() ? Variant() : (index > size() ? data[size() - 1] : data[index]); }
    /*------------------------------------------------------------------------------------------------------------------------------------------------------*/

    // Returns true if both this Array and the specified Array contain the same data, otherwise false.
    // Returns false if the array is write-only.
    func bool operator==(const Array &other) const { return !is_write_only() && data == other.data; }
    // Returns true if neither this Array and the specified Array contain the same data, otherwise false.
    // Returns false if the array is write-only.
    func bool operator!=(const Array &other) const { return !is_write_only() && data != other.data; }

    /*--------------------------------------------------------------------------------------------------*/

    // Resizes the array to the specified number of elements.
    // Fails silently if the array is write-only.
    func void resize(int size) { if (!is_write_only()) data.resize(size); }
    // Reverses the order of the elements in the array.
    // Fails silently if the array is write-only.
    func void reverse() const { if (!is_write_only()) std::reverse(data.begin(), data.end()); }
    // Returns true if the array is empty, otherwise false.
    // Returns false if the array is write-only.
    func bool is_empty() const { return is_write_only() ? false : data.size() == 0; }
    // Returns true if the array contains the specified value, otherwise false.
    // Returns false if the array is write-only.
    template<typename T>
    func bool has(const Variant &value) const {
        if (is_write_only()) return false;

        for (const Variant &v : data) {
            if (v.get<T>() == value.get<T>()) {
                return true;
            }
        }
        return false;
    }
    // Returns the index of the first occurrence of the expected value.
    // Returns -1 if the array is write-only.
    func int find(const Variant &p_value) const {
        if (is_write_only()) return -1;

        unsigned int index = 0;

        for (const Variant &v : data) {
            if (v.get_type() == p_value.get_type()) {
                return index;
            }
            index++;
        }

        return -1;
    }
    // Returns the index of the last occurrence of the expected value.
    // Returns -1 if the array is write-only.
    func int rfind(const Variant &p_value) const {
        if (is_write_only()) return -1;

        unsigned int index = 0;
        reverse();

        for (const Variant &v : data) {
            if (v.get_type() == p_value.get_type()) {
                return index;
            }
            index++;
        }

        reverse();
        return -1;
    }

    // Appends a single value to the array.
    func void append(const Variant &p_value) { if (!is_write_only()) data.push_back(p_value); }
    // Appends an array to the array.
    func void append_array(const Array &p_array) { if (!is_write_only()) data.insert(data.end(), p_array.data.begin(), p_array.data.end()); }

    func void sort() {
        if (is_write_only()) return;


    }
};
