/* Provide Declarations */
#include <stdint.h>
#ifndef __cplusplus
typedef unsigned char bool;
#endif

#if defined(__GNUC__)
#define  __ATTRIBUTELIST__(x) __attribute__(x)
#else
#define  __ATTRIBUTELIST__(x)  
#endif

#ifdef _MSC_VER  /* Can only support "linkonce" vars with GCC */
#define __attribute__(X)
#endif



/* Global Declarations */

/* Types Declarations */
struct l_array_11_uint8_t;

/* Function definitions */

/* Types Definitions */
struct l_array_11_uint8_t {
  uint8_t array[11];
};

/* Global Variable Declarations */
const static struct l_array_11_uint8_t _OC_str;

/* Function Declarations */
int main(void) __ATTRIBUTELIST__((noinline, nothrow, stack_protect));
uint32_t printf(void* _5, ...);


/* Global Variable Definitions and Initialization */
static const struct l_array_11_uint8_t _OC_str = { "Value: %d\n" };


/* LLVM Intrinsic Builtin Function Bodies */


/* Function Bodies */

int main(void) {
  uint32_t _1;    /* Address-exposed local */
  uint32_t _2;    /* Address-exposed local */
  uint32_t _3;
  uint32_t _4;

  _1 = 0;
  _2 = 42;
  _3 = _2;
  _4 = printf((&_OC_str), _3);
  return 0;
}

