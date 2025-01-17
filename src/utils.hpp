#pragma once

/*
    Copyright (c) 2017-2020 ByteBit

    This file is part of BetterSpades.

    BetterSpades is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    BetterSpades is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with BetterSpades.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdint.h>
#include <stdbool.h>

#include <hashtable.hpp>

int base64_decode(char* data, int len);

int int_cmp(void* first_key, void* second_key, size_t key_size);
size_t int_hash(void* raw_key, size_t key_size);
void ht_iterate_remove(HashTable* ht, void* user, bool (*callback)(void* key, void* value, void* user));
bool ht_iterate(HashTable* ht, void* user, bool (*callback)(void* key, void* value, void* user));
