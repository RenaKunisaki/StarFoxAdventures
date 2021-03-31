/* eg:
 * int myVal;
 * GET_REGISTER(29, myVal);
 * OSReport("r29 = %d", myVal);
 */
#define GET_REGISTER(r, dest) __asm__ __volatile__("mr %0, " # r : "=r"(dest))
