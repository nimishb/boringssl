/* Copyright (c) 2015, Google Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

#include <openssl/rand.h>

#include <stdlib.h>
#include <string.h>

#include <openssl/cpu.h>


#if defined(OPENSSL_X86_64)

int CRYPTO_have_hwrand(void) {
  return (OPENSSL_ia32cap_P[1] & (1u << 30)) != 0;
}

/* CRYPTO_rdrand is defined in asm/rdrand-x86_64.pl */
extern uint64_t CRYPTO_rdrand(void);

void CRYPTO_hwrand(uint8_t *buf, size_t len) {
  while (len >= 8) {
    uint64_t rand = CRYPTO_rdrand();
    memcpy(buf, &rand, sizeof(rand));
    len -= sizeof(rand);
    buf += sizeof(rand);
  }

  if (len > 0) {
    uint64_t rand = CRYPTO_rdrand();
    memcpy(buf, &rand, len);
  }
}

#else

int CRYPTO_have_hwrand(void) {
  return 0;
}

void CRYPTO_hwrand(uint8_t *buf, size_t len) {
  abort();
}

#endif
