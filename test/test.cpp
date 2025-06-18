/**
 * Author: TheLazyFerret (https://github.com/TheLazyFerret)
 * Copyright (c) 2025 TheLazyFerret
 * Licensed under the MIT License. See LICENSE file in the project root for full license information.
 * 
 * test file used for testing RuntimeBitset
 */


// g++ -Wall -Wextra -Werror -I lib/ -g lib/RuntimeBitset/RuntimeBitset.cpp test/test.cpp

#include "RuntimeBitset/RuntimeBitset.hpp"
#include <iostream>
#include <bitset>

using namespace DynBitset;

int main() {
  RuntimeBitset one(70, ~0);
  RuntimeBitset::Reference ref = one[15];
  ref.flip();
  std::cout << ref << std::endl;
  one[10] = false;
  std::cout << one << std::endl;

  return 0;
}