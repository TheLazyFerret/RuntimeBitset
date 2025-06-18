/**
 * Author: TheLazyFerret (https://github.com/TheLazyFerret)
 * Copyright (c) 2025 TheLazyFerret
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.
 * header file, interface of the class RuntimeBitset, represents
 *   a bitset without template
 */

#pragma once

#include <exception>
#include <string>
#include <cstddef>

namespace DynBitset {

class RuntimeBitset {
  public:
    RuntimeBitset(const std::size_t t_size, const std::size_t t_num);
    RuntimeBitset(const std::size_t t_size);
    RuntimeBitset(const std::string& t_string);
    // SPECIAL MEMBERS
    RuntimeBitset(); // Default constructor
    ~RuntimeBitset(); // Destructor
    RuntimeBitset(const RuntimeBitset& t_RuntimeBitset); // Copy constructor
    RuntimeBitset& operator=(const RuntimeBitset& t_RuntimeBitset); // Copy assignment
    RuntimeBitset(RuntimeBitset&& t_RuntimeBitset); // Move constructor
    RuntimeBitset& operator=(RuntimeBitset&& t_RuntimeBitset); // Move assignment

    std::string to_string() const noexcept;
    unsigned long long to_ullong() const noexcept;
    unsigned long to_ulong() const noexcept;

    // Operator acess
    bool operator[](std::size_t t_position) const;

    bool test(std::size_t t_position) const;

    bool all() const noexcept;
    bool any() const noexcept;
    bool none() const noexcept;

    std::size_t count() const noexcept;

    // Capacity
    inline std::size_t size() const noexcept {return m_size;}

    // Extra
    void printDebug() const noexcept;
 
    // Modifiers
    RuntimeBitset& set() noexcept;
    RuntimeBitset& set(const std::size_t t_position);
    RuntimeBitset& reset() noexcept;
    RuntimeBitset& reset(const std::size_t t_position);   
    RuntimeBitset& flip() noexcept;
    RuntimeBitset& flip(const std::size_t t_position);

    // Modifiers
    RuntimeBitset& operator&=(const RuntimeBitset& t_other);
    RuntimeBitset& operator|=(const RuntimeBitset& t_other);
    RuntimeBitset& operator^=(const RuntimeBitset& t_other);
    RuntimeBitset operator~();

    RuntimeBitset operator<<(std::size_t t_pos) const;
    RuntimeBitset& operator<<=(std::size_t t_pos);
    RuntimeBitset operator>>(std::size_t t_pos) const;
    RuntimeBitset& operator>>=(std::size_t t_pos);

    // Binary logic operators
    friend inline RuntimeBitset operator&(const RuntimeBitset& t_1, const RuntimeBitset& t_2);
    friend inline  RuntimeBitset operator|(const RuntimeBitset& t_1, const RuntimeBitset& t_2);
    friend inline RuntimeBitset operator^(const RuntimeBitset& t_1, const RuntimeBitset& t_2);

    // iostream operators
    friend inline std::ostream& operator<<(std::ostream& os, const RuntimeBitset& t_bitset);
    friend inline std::istream& operator>>(std::istream& is, RuntimeBitset& t_bitset);

    class Reference {
      public:
        // Constructor
        Reference(RuntimeBitset& t_reference, const std::size_t t_pos) : m_position(t_pos), m_bitset(t_reference) {}
        // SPECIAL MEMBERS
        Reference() = default;
        ~Reference() = default;
        Reference(const Reference&) = default;
        Reference& operator=(const Reference&) = default;
        Reference(Reference&&) = default;
        Reference& operator=(Reference&&) = default;

        Reference& operator=(const bool t_value);

        operator bool() const;
        bool operator~() const;
        Reference& flip();
      private:
        std::size_t m_position;
        RuntimeBitset& m_bitset;
    };

    Reference operator[](std::size_t t_pos);
  private:
    std::size_t* m_bits = nullptr; // little endian
    std::size_t* m_mask = nullptr; // little endian
    std::size_t  m_size;
    std::size_t  m_blocks;

    // STATIC MEMBERS
    static constexpr std::size_t BLOCK_SIZE = sizeof(std::size_t) * 8; // Number of bits of each block
    static constexpr std::size_t ALL_BITS_ONE = ~(0);

    // PRIVATE METHODS
    void buildBlocks();
    void buildMask();
    void build(const std::size_t t_size); // Call buildBlocks, buildMask
    void clean(); // Put all bits to 0
    void destroy(); // Destroy the object
    static void copy(RuntimeBitset& t_copy, const RuntimeBitset& t_toCopy);
    static void move(RuntimeBitset& t_copy, RuntimeBitset& t_toMove);
    static std::size_t getNumberBlocks(const std::size_t t_size) noexcept; // Method to calculate the number of needed blocks
    static std::size_t getLastMask(const std::size_t t_number_bits); // Method to calculate the mask of the last block
    // First block position, second mask position
    std::pair<std::size_t, std::size_t> getPosition(std::size_t t_position) const;
    // Returns the mask position inside a block
    static std::size_t getMaskPosition(const std::size_t t_position);
    bool getValueInPosition(std::size_t t_position) const;
    
    // Bitwise methods
    void shiftBlocksLeft(std::size_t t_pos);
    void shiftBlocksRight(std::size_t t_pos);
    void bitwiseLeft(std::size_t t_pos);
    void bitwiseRight(std::size_t t_pos);

    void buildFromString(const std::string& t_string);
};

class RuntimeBitsetException : public std::exception {
  public:
    RuntimeBitsetException(const std::string& t_message) : m_errorMessage(t_message) {}
    const char* what() const noexcept override {return m_errorMessage.c_str();}
  private:
    std::string m_errorMessage;
};

class RuntimeBitsetInvalidSize : public RuntimeBitsetException {
  public:
    RuntimeBitsetInvalidSize() : RuntimeBitsetException("Invalid size creating the RuntimeBitset.") {}
};

class RuntimeBitsetOutOfRange : public RuntimeBitsetException {
  public:
    RuntimeBitsetOutOfRange() : RuntimeBitsetException("Position is out of range") {}
};

class RuntimeBitsetSizeDismatch : public RuntimeBitsetException {
  public:
    RuntimeBitsetSizeDismatch() : RuntimeBitsetException("The bitsets are of different size") {}
};

class RuntimeBitsetUnknownChar : public RuntimeBitsetException {
  public:
  RuntimeBitsetUnknownChar() : RuntimeBitsetException("Unkown character found") {}
};

RuntimeBitset operator&(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RuntimeBitsetSizeDismatch());
  RuntimeBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    aux.m_bits[i] = t_1.m_bits[i] & t_2.m_bits[i];
  }
  return aux;
}

RuntimeBitset operator|(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RuntimeBitsetSizeDismatch());
  RuntimeBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    aux.m_bits[i] = t_1.m_bits[i] | t_2.m_bits[i];
  }
  return aux;
}

RuntimeBitset operator^(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RuntimeBitsetSizeDismatch());
  RuntimeBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    aux.m_bits[i] = t_1.m_bits[i] ^ t_2.m_bits[i];
  }
  return aux;
}

std::ostream& operator<<(std::ostream& os, const RuntimeBitset& t_bitset) {
  os << t_bitset.to_string();
  return os;
}

std::istream& operator>>(std::istream& is, RuntimeBitset& t_bitset) {
  std::string aux;
  is >> aux;
  t_bitset.buildFromString(aux);

  return is;
}

} // namespace DynBitset