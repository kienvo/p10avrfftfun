#ifndef __P10_BIT_UTILS_H__

#define setbit(byte, bit) (byte) |= _BV(bit);
#define clearbit(byte, bit) (byte) &= ~_BV(bit);


#endif /* __P10_BIT_UTILS_H__ */