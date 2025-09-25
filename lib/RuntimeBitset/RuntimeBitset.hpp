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
#include <iostream>
#include <bitset>
#include <sstream>
#include <algorithm>
#include <cassert>

namespace RunBitset {

class RuntimeBitset {
  public:
    // SPECIAL MEMBERS
    RuntimeBitset(const std::size_t t_size, const std::size_t t_num);
    RuntimeBitset(const std::size_t t_size);
    RuntimeBitset(const std::string& t_string);
    RuntimeBitset(); // Default constructor
    ~RuntimeBitset(); // Destructor
    inline RuntimeBitset(const RuntimeBitset& t_RuntimeBitset); // Copy constructor
    inline RuntimeBitset& operator=(const RuntimeBitset& t_RuntimeBitset); // Copy assignment
    inline RuntimeBitset(RuntimeBitset&& t_RuntimeBitset); // Move constructor
    inline RuntimeBitset& operator=(RuntimeBitset&& t_RuntimeBitset); // Move assignment

    inline std::string to_string() const noexcept;
    inline unsigned long long to_ullong() const noexcept;
    inline unsigned long to_ulong() const noexcept;

    // NORMAL MEMBERS
    inline bool operator[](std::size_t t_position) const;

    inline bool test(std::size_t t_position) const;

    inline bool all() const noexcept;
    inline bool any() const noexcept;
    inline bool none() const noexcept;

    inline std::size_t count() const noexcept;

    // Capacity
    inline std::size_t size() const noexcept {return m_size;}

    // Extra
    inline void printDebug() const noexcept;
 
    // Modifiers
    inline RuntimeBitset& set() noexcept;
    inline RuntimeBitset& set(const std::size_t t_position);
    inline RuntimeBitset& reset() noexcept;
    inline RuntimeBitset& reset(const std::size_t t_position);   
    inline RuntimeBitset& flip() noexcept;
    inline RuntimeBitset& flip(const std::size_t t_position);

    // Modifiers
    inline RuntimeBitset& operator&=(const RuntimeBitset& t_other);
    inline RuntimeBitset& operator|=(const RuntimeBitset& t_other);
    inline RuntimeBitset& operator^=(const RuntimeBitset& t_other);
    inline RuntimeBitset operator~();

    inline RuntimeBitset operator<<(std::size_t t_pos) const;
    inline RuntimeBitset& operator<<=(std::size_t t_pos);
    inline RuntimeBitset operator>>(std::size_t t_pos) const;
    inline RuntimeBitset& operator>>=(std::size_t t_pos);

    // Binary logic operators
    friend inline RuntimeBitset operator&(const RuntimeBitset& t_1, const RuntimeBitset& t_2);
    friend inline  RuntimeBitset operator|(const RuntimeBitset& t_1, const RuntimeBitset& t_2);
    friend inline RuntimeBitset operator^(const RuntimeBitset& t_1, const RuntimeBitset& t_2);

    // iostream operators
    friend inline std::ostream& operator<<(std::ostream& os, const RuntimeBitset& t_bitset);
    friend inline std::istream& operator>>(std::istream& is, RuntimeBitset& t_bitset);

    class Reference {
      public:
        Reference(RuntimeBitset& t_reference, const std::size_t t_pos) 
        : m_position(t_pos), m_bitset(t_reference) {}
        Reference() = default;
        ~Reference() = default;
        Reference(const Reference&) = default;
        Reference& operator=(const Reference&) = default;
        Reference(Reference&&) = default;
        Reference& operator=(Reference&&) = default;

        inline Reference& operator=(const bool t_value);

        inline operator bool() const;
        inline bool operator~() const;
        inline Reference& flip();
      private:
        std::size_t m_position;
        RuntimeBitset& m_bitset;
    };

    inline Reference operator[](std::size_t t_pos);
  private:
    // STATIC MEMBERS
    // Number of bits of each block
    inline static constexpr std::size_t BLOCK_SIZE = sizeof(std::size_t) * 8;
    inline static constexpr std::size_t ALL_BITS_ONE = ~(0);

    // PRIVATE METHODS
    inline void buildBlocks();
    inline void buildMask();
    // Call buildBlocks, buildMask
    inline void build(const std::size_t t_size);
    inline void clean(); // Put all bits to 0
    inline void destroy(); // Destroy the object
    inline static void copy(RuntimeBitset& t_copy, const RuntimeBitset& t_toCopy);
    inline static void move(RuntimeBitset& t_copy, RuntimeBitset& t_toMove);
    // Method to calculate the number of needed blocks
    inline static std::size_t getNumberBlocks(const std::size_t t_size) noexcept;
    // Method to calculate the mask of the last block
    inline static std::size_t getLastMask(const std::size_t t_number_bits);
    // First block position, second mask position
    inline std::pair<std::size_t, std::size_t> getPosition(std::size_t t_position) const;
    // Returns the mask position inside a block
    inline static std::size_t getMaskPosition(const std::size_t t_position);
    inline bool getValueInPosition(std::size_t t_position) const;
    
    // Bitwise methods
    inline void shiftBlocksLeft(std::size_t t_pos);
    inline void shiftBlocksRight(std::size_t t_pos);
    inline void bitwiseLeft(std::size_t t_pos);
    inline void bitwiseRight(std::size_t t_pos);

    inline void buildFromString(const std::string& t_string);

    // Attributes
    std::size_t* m_bits = nullptr; // little endian
    std::size_t* m_mask = nullptr; // little endian
    std::size_t  m_size;
    std::size_t  m_blocks;
};

} // namespace RunBitset

namespace RunBitsetException {
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

}

namespace RunBitset {

RuntimeBitset operator&(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RunBitsetException::RuntimeBitsetSizeDismatch());
  RuntimeBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    aux.m_bits[i] = t_1.m_bits[i] & t_2.m_bits[i];
  }
  return aux;
}

RuntimeBitset operator|(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RunBitsetException::RuntimeBitsetSizeDismatch());
  RuntimeBitset aux(t_1.size());
  for (std::size_t i = 0; i < aux.m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    aux.m_bits[i] = t_1.m_bits[i] | t_2.m_bits[i];
  }
  return aux;
}

RuntimeBitset operator^(const RuntimeBitset& t_1, const RuntimeBitset& t_2) {
  if (t_1.size() != t_2.size()) throw(RunBitsetException::RuntimeBitsetSizeDismatch());
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

}


RunBitset::RuntimeBitset::RuntimeBitset(const std::size_t t_size, const std::size_t t_num) {
  build(t_size);
  m_bits[0] = t_num;
}

RunBitset::RuntimeBitset::RuntimeBitset(const std::size_t t_size) {
  build(t_size);
  clean();
}

RunBitset::RuntimeBitset::RuntimeBitset(const std::string& t_string) {
  buildFromString(t_string);
}

RunBitset::RuntimeBitset::RuntimeBitset() {
  build(BLOCK_SIZE);
  clean();
}

RunBitset::RuntimeBitset::~RuntimeBitset() {
  destroy();
}

RunBitset::RuntimeBitset::RuntimeBitset(const RuntimeBitset& t_RuntimeBitset) {
  copy(*this, t_RuntimeBitset);
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::operator=(const RuntimeBitset& t_RuntimeBitset) {
  copy(*this, t_RuntimeBitset);
  return *this;
}

RunBitset::RuntimeBitset::RuntimeBitset(RuntimeBitset&& t_RuntimeBitset) {
  move(*this, t_RuntimeBitset);
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::operator=(RuntimeBitset&& t_RuntimeBitset) {
  move(*this, t_RuntimeBitset);
  return *this;
}

std::string RunBitset::RuntimeBitset::to_string() const noexcept {
  std::string toReturn;
  for (long long i = m_blocks - 1; i >= 0; --i) {
    std::stringstream buffer; // Auxiliar buffer, pending of improving
    std::size_t block = m_bits[i] & m_mask[i];
    std::size_t mask = m_mask[i];
    for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
      if (mask % 2 != 0) { // Check the less significant bit of each block
        if (block % 2 != 0) {
          buffer << '1';
        }
        else {
          buffer << '0';
        }
      }
      // next bit
      block >>= 1;
      mask >>= 1; 
    }  
    std::string aux = buffer.str();
    std::reverse(aux.begin(), aux.end()); // quite unneficient, but cleanest approach
    toReturn += aux;
  }
  return toReturn;
}

void RunBitset::RuntimeBitset::build(const std::size_t t_size) {
  // Bitsets of size 0 breaks the implementation
  if (t_size == 0) throw (RunBitsetException::RuntimeBitsetInvalidSize());
  destroy();
  m_size = t_size;
  // Get the minimal number of blocks needed to represent the numbe of bits
  m_blocks = getNumberBlocks(t_size);
  buildBlocks();
  buildMask();
}

std::size_t RunBitset::RuntimeBitset::getNumberBlocks(const std::size_t t_size) noexcept {
  assert (t_size != 0);
  std::size_t numberOfBlocks = 0;
  for (std::size_t i = 0; i < t_size; i+=BLOCK_SIZE) {
    ++numberOfBlocks;
  }
  return numberOfBlocks;
}

void RunBitset::RuntimeBitset::buildBlocks() {
  m_bits = new std::size_t[m_blocks];
  m_mask = new std::size_t[m_blocks];
}

void RunBitset::RuntimeBitset::buildMask() {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_mask[i] = ALL_BITS_ONE;
  }
  std::size_t lastMask = m_size - ((m_blocks -1) * BLOCK_SIZE);
  // Only the most significant block has a no ~0 mask
  m_mask[m_blocks - 1] = getLastMask(lastMask);
}

void RunBitset::RuntimeBitset::clean() {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = 0;
  }
}

std::size_t RunBitset::RuntimeBitset::getLastMask(const std::size_t t_number_bits) {
  std::size_t lastMask = ALL_BITS_ONE;
  lastMask >>= (BLOCK_SIZE - t_number_bits);
  return lastMask;
}

void RunBitset::RuntimeBitset::destroy() {
  if (m_bits != nullptr) { // Avoid double deletion
    delete[] m_bits;
    m_bits = nullptr;
  }
  if (m_mask != nullptr) { // Avoid double deletion
    delete[] m_mask;
    m_mask = nullptr;
  }
  m_size = 0;
  m_blocks = 0;
}

void RunBitset::RuntimeBitset::copy
(RuntimeBitset& t_copy, const RuntimeBitset& t_toCopy) {
  t_copy.destroy();
  t_copy.build(t_toCopy.size()); // bitset of same size as t_toCopy
  for (std::size_t i = 0; i < t_copy.m_blocks; ++i) {
    const std::size_t blockToCopy = t_toCopy.m_bits[i]; // copy all the blocks
    t_copy.m_bits[i] = blockToCopy;
  }
}

void RunBitset::RuntimeBitset::move
(RuntimeBitset& t_move, RuntimeBitset& t_toMove) {
  t_move.destroy();
  // MOVE
  t_move.m_bits = t_toMove.m_bits;
  t_move.m_mask = t_toMove.m_mask;
  t_move.m_size = t_toMove.m_size;
  t_move.m_blocks = t_toMove.m_blocks;
  // CLEAN
  t_toMove.m_bits = nullptr;
  t_toMove.m_mask = nullptr;
  t_toMove.m_size = 0;
  t_toMove.m_blocks = 0;
  t_toMove.build(1);
}

unsigned long long RunBitset::RuntimeBitset::to_ullong() const noexcept {
  std::size_t bits = m_bits[0] & m_mask[0]; // Aply mask to avoid useless bits
  // return the less significant <sizeof(ullong) * 8 bits> of the less significant block
  return static_cast<unsigned long long>(bits);

}

unsigned long RunBitset::RuntimeBitset::to_ulong() const noexcept {
  std::size_t bits = m_bits[0] & m_mask[0];// Aply mask to avoid useless bits
  // return the less significant <sizeof(ulong) * 8 bits> of the less significant block
  return static_cast<unsigned long>(bits);
}

bool RunBitset::RuntimeBitset::all() const noexcept {
  for ( std::size_t i = 0; i < m_blocks; ++i) {
    // If applying the mask is equal to mask, then it is true
    if ((m_bits[i] & m_mask[i]) != m_mask[i]) return false;
  }
  return true;
}

bool RunBitset::RuntimeBitset::any() const noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    // If applying the mask is not 0, then atleast 1 bit is set
    if ((m_bits[i] & m_mask[i]) != 0) return true;
  }
  return false;
}

bool RunBitset::RuntimeBitset::none() const noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    // exactly the opposite to any
    if ((m_bits[i] & m_mask[i]) != 0) return false;
  }
  return true;
}

/// At first, my idea was the .second was t_position (relative position inside the block)
/// But for more comfortable code, I decided the .second was the mask of the relative position
std::pair<std::size_t, std::size_t> 
RunBitset::RuntimeBitset::getPosition(std::size_t t_position) const {
  if (t_position >= m_size) throw(RunBitsetException::RuntimeBitsetOutOfRange());
  std::size_t blockPosition = 0;
  // Get in what block is allocated
  while (t_position >= BLOCK_SIZE) {
    t_position -= BLOCK_SIZE; // get internal block position
    ++blockPosition;
  }
  return std::make_pair(blockPosition, getMaskPosition(t_position));
}

/// Returns a mask with all 0 except in the t_position
std::size_t RunBitset::RuntimeBitset::getMaskPosition(const std::size_t t_position) {
  assert(t_position <= BLOCK_SIZE);
  constexpr std::size_t auxMask = 1;
  return (auxMask << t_position);
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::set() noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = ALL_BITS_ONE;
  }
  return *this;
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::set(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;
  m_bits[blockPosition] |= positionMask; // will apply X | 1 in the position, the rest X | 0
  return *this;
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::reset() noexcept {
  clean();
  return *this;
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::reset(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = ~position.second; // Reversed position mask
  m_bits[blockPosition] &= positionMask; // will aply X & 0 in the position, the rest X & 1
  return *this;
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::flip() noexcept {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    m_bits[i] = ~m_bits[i];
  }
  return *this;
}

RunBitset::RuntimeBitset& RunBitset::RuntimeBitset::flip(const std::size_t t_position) {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;

  // flip the bit in the position
  //   first, reverse all the block, then, apply mask, all 0 except in the position
  const std::size_t positionValueReversed = ~m_bits[blockPosition] & positionMask;
  // original block with the position bit = 0
  const std::size_t allExceptPositionValue = m_bits[blockPosition] & ~positionMask;
  // example: 000R000 | XXX0XXX
  m_bits[blockPosition] = allExceptPositionValue | positionValueReversed;
  return *this;
}

void RunBitset::RuntimeBitset::printDebug() const noexcept {
  std::cout << "size: " << m_size << std::endl << "blocks: " << m_blocks << std::endl;
}

std::size_t RunBitset::RuntimeBitset::count() const noexcept {
  std::size_t numberOfActive = 0;
  for (std::size_t i = 0; i < m_blocks; ++i) {
    std::size_t block = m_bits[i] & m_mask[i]; // apply mask; remove no significant bits
    for (std::size_t j = 0; j < BLOCK_SIZE; ++j) {
      if (block % 2 != 0) ++numberOfActive; // check less significant bit
      block >>= 1;
    }
  }
  return numberOfActive;
}

bool RunBitset::RuntimeBitset::operator[](std::size_t t_position) const {
  return getValueInPosition(t_position);
}

bool RunBitset::RuntimeBitset::test(std::size_t t_position) const {
  return getValueInPosition(t_position);
}

bool RunBitset::RuntimeBitset::getValueInPosition(std::size_t t_position) const {
  const std::pair<std::size_t, std::size_t> position(getPosition(t_position));
  const std::size_t blockPosition = position.first;
  const std::size_t positionMask = position.second;
  
  // Get all 0 and the value in t_position
  const std::size_t auxBlock = m_bits[blockPosition] & positionMask;
  if ((auxBlock | 0) == 0) return false; // if BXB | 0 == 0, then X = 0
  return true;
}

RunBitset::RuntimeBitset& 
RunBitset::RuntimeBitset::operator&=(const RuntimeBitset& t_other) {
  *this = *this & t_other;
  return *this;
}

RunBitset::RuntimeBitset& 
RunBitset::RuntimeBitset::operator|=(const RuntimeBitset& t_other) {
  *this = *this | t_other;
  return *this;
}

RunBitset::RuntimeBitset& 
RunBitset::RuntimeBitset::operator^=(const RuntimeBitset& t_other) {
  *this = *this ^ t_other;
  return *this;
}

RunBitset::RuntimeBitset RunBitset::RuntimeBitset::operator~() {
  for (std::size_t i = 0; i < this->m_blocks; ++i) {
    // Don´t need to apply mask due it doesn´t affect the significant bits
    m_bits[i] = ~m_bits[i];
  }
  return *this;
}

RunBitset::RuntimeBitset 
RunBitset::RuntimeBitset::operator<<(std::size_t t_pos) const {
  RuntimeBitset aux = *this;
  aux.bitwiseLeft(t_pos);
  return aux;
}

RunBitset::RuntimeBitset 
RunBitset::RuntimeBitset::operator>>(std::size_t t_pos) const {
  RuntimeBitset aux = *this;
  aux.bitwiseRight(t_pos);
  return aux;
}

RunBitset::RuntimeBitset& 
RunBitset::RuntimeBitset::operator<<=(std::size_t t_pos) {
  this->bitwiseLeft(t_pos);
  return *this;
}

RunBitset::RuntimeBitset& 
RunBitset::RuntimeBitset::operator>>=(std::size_t t_pos) {
  this->bitwiseRight(t_pos);
  return *this;
}

// Example of the algorithm in i block (blocks of 8 bits):
//   t_pos == 4
//   mask[i] and [i + 1] == 11111111
//   original block[i] == 10110100 
//   original block[i + 1] == 00110000 (the algorithm has been always already passed from i + 1)
//   remain -> 00001011
//   block[i] -> 0100000
//   block[i + 1] -> 00110000 | 00001011 -> 00111011
void RunBitset::RuntimeBitset::bitwiseLeft(std::size_t t_pos) {
  std::size_t blockWise = 0;
  while (t_pos >= BLOCK_SIZE) {
    ++blockWise;
    t_pos -= BLOCK_SIZE;
  }
  if (blockWise > 0) this->shiftBlocksLeft(blockWise);
  
  for (long long i = this->m_blocks - 1; i >= 0; --i) {
    // Apply mask
    const std::size_t remain = (this->m_bits[i] & this->m_mask[i]) >> (BLOCK_SIZE - t_pos);
    this->m_bits[i] = (this->m_bits[i] & this->m_mask[i]) << t_pos;
    if ((i + 1) < static_cast<long long>(this->m_blocks)) this->m_bits[i + 1] = this->m_bits[i + 1] | remain;
  }
}

// Example of the algorithm in i block (blocks of 8 bits):
//   t_pos == 4
//   mask[i] and [i - 1] == 11111111
//   original block[i] == 10110100 
//   original block[i - 1] == 00000011 (the algorithm has been always already passed from i - 1)
//   remain -> 01000000
//   block[i] -> 00001011
//   block[i - 1] -> 00000011 | 01000000 -> 01000011
void RunBitset::RuntimeBitset::bitwiseRight(std::size_t t_pos) {
  std::size_t blockWise = 0;
  while (t_pos >= BLOCK_SIZE) {
    ++blockWise;
    t_pos -= BLOCK_SIZE;
  }
  if (blockWise > 0) this->shiftBlocksRight(blockWise);
  for (long long i = 0; i < static_cast<long long>(this->m_blocks); ++i) {
    // Apply mask
    const std::size_t remain = (this->m_bits[i] & this->m_mask[i]) << (BLOCK_SIZE - t_pos);
    this->m_bits[i] = (this->m_bits[i] & this->m_mask[i]) >> t_pos;
    if ((i - 1) >= static_cast<long long>(0)) this->m_bits[i - 1] = this->m_bits[i - 1] | remain;
  }
}

// SHIFT BLOCKS(LEFT|RIGHT)
// Due this implementation is not a continuous set of bits, 
//   when you apply a shift of more than the size of the block
// It is more easy just move the blocks of position
void RunBitset::RuntimeBitset::shiftBlocksLeft(std::size_t t_pos) {
  for (long long i = m_blocks - 1; i >= 0; --i) {
    const long long newPos = i + t_pos;
    if (newPos < static_cast<long long>(m_blocks)) {
      // apply mask to avoid bringing crap from the no significant bits
      m_bits[newPos] = (m_bits[i] & m_mask[i]);
    }
    m_bits[i] = 0;
  }
}

void RunBitset::RuntimeBitset::shiftBlocksRight(std::size_t t_pos) {
  for (std::size_t i = 0; i < m_blocks; ++i) {
    const long long newPos = i - t_pos;
    if (newPos >= 0) {
      // apply mask to avoid bringing crap from the no significant bits
      m_bits[newPos] = (m_bits[i] & m_mask[i]);
    }
    m_bits[i] = 0;
  }
}

// it is more easy and logical resize the bitset with the size of the string
void RunBitset::RuntimeBitset::buildFromString(const std::string& t_string) {
  destroy();
  build(t_string.size());
  const std::size_t sizeAux = t_string.size() - 1;
  for (std::size_t i = 0; i < t_string.size(); ++i) {
    if (t_string[i] == '1') {
      set(sizeAux - i);
    }
    else if (t_string[i] == '0') {
      continue;
    }
    else {
      throw(RunBitsetException::RuntimeBitsetUnknownChar());
    }
  }
}

RunBitset::RuntimeBitset::Reference 
RunBitset::RuntimeBitset::operator[](std::size_t t_pos) {
  if (t_pos >= m_size) throw(RunBitsetException::RuntimeBitsetOutOfRange());
  return Reference(*this, t_pos);
}

// REFERENCE
RunBitset::RuntimeBitset::Reference& 
RunBitset::RuntimeBitset::Reference::operator=(const bool t_value) {
  if (t_value == true) {
    m_bitset.set(m_position);
  }
  else {
    m_bitset.reset(m_position);
  }
  return *this;
}

RunBitset::RuntimeBitset::Reference::operator bool() const {
  return m_bitset.getValueInPosition(m_position);
}

bool RunBitset::RuntimeBitset::Reference::operator~() const {
  return !(m_bitset.getValueInPosition(m_position));
}

RunBitset::RuntimeBitset::Reference& RunBitset::RuntimeBitset::Reference::flip() {
  m_bitset.flip(m_position);
  return *this;
}