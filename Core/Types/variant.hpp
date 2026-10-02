/************************************/
/*           variant.hpp            */
/*                                  */
/*       RatLab Game Engine         */
/*          2026-Present            */
/*         On MIT License           */
/************************************/

#pragma once

#include "../Essentials/essentials.hpp"

class Variant {
    public:
    // All available types for the Variant, used for comparisons.
    enum Types {
        VARIANT,
        U8,
        U16,
        U32,
        U64,
        I8,
        I16,
        I32,
        I64,
        F32,
        F64,
        STRING,
        BOOL,
        ARRAY,
        DICTIONARY,
        VECTOR2,
        VECTOR2I,
        VECTOR3,
        VECTOR3I,
        VECTOR4,
        VECTOR4I,
        QUATERNION,
        BASIS,
        RECT2,
        RECT2I,
    };
    // Specifies different access permissions for both the Variant and other types extending from Variant.
    enum Accessability : unsigned char {
        ReadOnly, // Can read, but not write. Access can be changed.
        WriteOnly, // Can write, but not read. Access can be changed.
        ReadAndWrite, // Can read and write. Access can be changed.
        Agressive, // Can read, but not write. Access cannot be overriden.
        Neutral, // Can write, but not read. Access cannot be overriden.
        Passive, // Can read and write. Access cannot be overriden.
    };
    // The accessibility of both the Variant and other types extending from Variant. (Default: ReadAndWrite)
    // If the access is aggressive, neutral, or passive: the access will be permanently set, until freed from memory.
    // NOTE: Other types extending from Variant has to set up their own accessability logic.
    Accessability access = Accessability::ReadAndWrite;
    /*--------------------------------------------------------------------------------------------------------------*/

    // Default constructor.
    func Variant() = default;
    // Constructor with accessability parameter.
    func Variant(Accessability p_access) : access(p_access) {}
    /*--------------------------------------------------------------------------------------------------------------*/

    // Returns whether the Variant is read-only.
    func bool is_read_only() const noexcept { return access == Accessability::ReadOnly; }
    // Returns whether the Variant is write-only.
    func bool is_write_only() const noexcept { return access == Accessability::WriteOnly; }
    // Returns whether the Variant is read-and-write.
    func bool is_read_and_write() const noexcept { return access == Accessability::ReadAndWrite; }
    // Returns whether the Variant is aggressive (read-only).
    // Access cannot be overriden.
    func bool is_aggressive() const noexcept { return access == Accessability::Agressive; }
    // Returns whether the Variant is neutral (write-only).
    // Access cannot be overriden.
    func bool is_neutral() const noexcept { return access == Accessability::Neutral; }
    // Returns whether the Variant is passive (read-and-write).
    // Access cannot be overriden.
    func bool is_passive() const noexcept { return access == Accessability::Passive; }
    // Returns whether the Variant can be read, regardless of access.
    func bool can_read() const noexcept { return !is_write_only() || !is_neutral() ; }
    // Returns whether the Variant can be written, regardless of access.
    func bool can_write() const noexcept { return !is_read_only() || !is_aggressive(); }
    // Returns whether the Variant can overwrite access.
    func bool can_overwrite_access() const noexcept { return !is_aggressive() || !is_neutral() || !is_passive(); }
    /*--------------------------------------------------------------------------------------------------------*/

    // Returns the value of the type which inherits from this Variant.
    // Manual get() implementation is required inside the derived class.
    template<typename T>
    func T get() const;

    // Returns the type of the value stored in this Variant.
    static func Types get_type() noexcept { return Types::VARIANT; }
    /*---------------------*/

    // Returns whether this Variant is equal to the specified Variant.
    template<typename T>
    func bool operator==(const Variant &other) const noexcept { return get<T>() == other.get<T>(); }
    // Returns whether this Variant is not equal to the specified Variant.
    template<typename T>
    func bool operator!=(const Variant &other) const noexcept { return get<T>() != other.get<T>(); }
};
