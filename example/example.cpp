/**
 * Author: TheLazyFerret (https://github.com/TheLazyFerret)
 * Copyright (c) 2025 TheLazyFerret
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.
 * example file about the usage of RuntimeBitset
 */

// Compilation: g++ -Wall -Werror -pedantic example/example.cpp -Ilib -o runtimebitset

#include "RuntimeBitset/RuntimeBitset.hpp"
#include <iostream>

using namespace RunBitset;

int main() {
  RuntimeBitset bitset1; // build a bitset of 64 bits with all set to 0
  RuntimeBitset bitset2(30); // build a bitset of 30 with all set to 0
  // build a bitset of 30 bits and append the first 64 bits (if the size is less than 64, append the first n bits)
  RuntimeBitset bitset3(30, 50);
  // build from standard input. The size will be the size of the input (previous size will be ignored)
  std::cin >> bitset2;

  std::cout << bitset2 << std::endl;

  std::cout << bitset3 << std::endl; // print the bitset
  std::cout << bitset3.to_string() << std::endl; // same as above

  const unsigned long num1 = bitset3.to_ulong(); // returns the first sizeof(unsigned long) * 8 bits  (less significant)
  const unsigned long long num2 = bitset3.to_ullong(); // returns the first sizeof(unsigned long long) * 8 (less significant)
  std::cout << num1 << std::endl << num2 << std::endl;

  bitset3 <<= 5;
  bitset3 >>= 5;
  bitset1 = RuntimeBitset(30, 1);
  RuntimeBitset bitset4 = bitset1 & bitset3;

  std::cout << bitset4 << std::endl;

  bitset4.set(); // all bits to 1
  bitset4.set(20); // bit in position 20 (starting from the less significant) will set to 1
  std::cout << bitset4 << std::endl;

  bitset4.reset(); // all bits to 0
  bitset4.reset(20); // bit in position 20 (starting from the less significant) will set to 0
  std::cout << bitset4 << std::endl;

  bitset4.flip(); // flip the value of all bits
  bitset4.flip(20); // flip the value in position 20 (starting from the less significant)
  std::cout << bitset4 << std::endl;

  ~bitset4; // Same as flip()
  std::cout << bitset4 << std::endl;

  std::cout << bitset4.all() << std::endl; // returns true if all bits are set to 1
  std::cout << bitset4.any() << std::endl; // returns true if any bit is set to 1
  std::cout << bitset4.none() << std::endl; // returns true if all bits are set to 0

  std::cout << bitset4.count() << std::endl; // returns the number of bits set to 1

  std::cout << bitset4[20] << std::endl; // returns the value at position 20 (starting from the less significant)
  std::cout << bitset4.test(20) << std::endl; // same as above

  return 0;
}