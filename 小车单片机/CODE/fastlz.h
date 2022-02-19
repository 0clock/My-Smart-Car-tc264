/*
  FastLZ - Byte-aligned LZ77 compression library
  Copyright (C) 2005-2020 Ariya Hidayat <ariya.hidayat@gmail.com>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef FASTLZ_H
#define FASTLZ_H

#include <stdint.h>

#define FASTLZ_VERSION 0x000500

#define FASTLZ_VERSION_MAJOR 0
#define FASTLZ_VERSION_MINOR 5
#define FASTLZ_VERSION_REVISION 0

#define FASTLZ_VERSION_STRING "0.5.0"


int fastlz_compress_level(int level, const void* input, int length, void* output);
int fastlz_decompress(const void* input, int length, void* output, int maxout);
int fastlz_compress(const void* input, int length, void* output);
int fastlz2_decompress(const void* input, int length, void* output, int maxout);
int fastlz2_compress(const void* input, int length, void* output);
static uint8_t* flz2_match(uint32_t len, uint32_t distance, uint8_t* op);
int fastlz1_decompress(const void* input, int length, void* output, int maxout);
int fastlz1_compress(const void* input, int length, void* output);
static uint8_t* flz1_match(uint32_t len, uint32_t distance, uint8_t* op);
static uint8_t* flz_finalize(uint32_t runs, const uint8_t* src, uint8_t* dest);
static void flz_smallcopy(uint8_t* dest, const uint8_t* src, uint32_t count);
static uint8_t* flz_literals(uint32_t runs, const uint8_t* src, uint8_t* dest);
static uint16_t flz_hash(uint32_t v);
static void flz_copy256(void* dest, const void* src);
static void flz_copy64(uint8_t* dest, const uint8_t* src, uint32_t count);
static uint32_t flz_cmp(const uint8_t* p, const uint8_t* q, const uint8_t* r);
static uint32_t flz_readu32(const void* ptr);
static void fastlz_memcpy(uint8_t* dest, const uint8_t* src, uint32_t count);
static void fastlz_memmove(uint8_t* dest, const uint8_t* src, uint32_t count);


#endif /* FASTLZ_H */
