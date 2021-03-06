//---------------------------------------------------------------------------
//		E L E N A   P r o j e c t:  ELENA Common Library
//
//		This file contains the common ELENA Project routine functions
//
//                                              (C)2005-2012, by Alexei Rakov
//---------------------------------------------------------------------------

#ifndef toolsH
#define toolsH 1

namespace _ELENA_
{

#if !defined(max)
#define max(a, b)       ((a) > (b) ? (a) : (b))
#endif

#if !defined(min)
#define min(a, b)       ((a) < (b) ? (a) : (b))
#endif

//#ifdef _LINUX32
//
//inline wchar_t* wcslwr(wchar_t* str)
//{
//   wchar_t* it = str;
//   while (*it != 0) { *it = towlower(*it); ++it; }
//
//   return str;
//}
//
//inline wchar_t* wcsupr(wchar_t* str)
//{
//   wchar_t* it = str;
//   while (*it != 0) { *it = towupper(*it); ++it; }
//
//   return str;
//}
//
//int _itow(int val, wchar_t* s, int radix)
//{
//    int len = 0;
//
//    if (val < 0) {
//        *s++ = '-';
//        val = 0 - val;
//
//        len++;
//    }
//
//    wchar_t* current = s;
//    unsigned int digit;
//    do {
//        digit = val % radix;
//        val /= radix;
//
//        *current++ = digit + '0';
//
//    } while (val > 0);
//
//    len += (int)(current - s);
//
//    *current-- = 0;
//
//    //swap
//    do {
//        wchar_t temp = *current;
//        *current = *s;
//        *s = temp;
//
//        --current;
//        ++s;
//
//    } while (s < current);
//
//    return len;
//}
//
//#endif

// --- miscellaneous string routines ---

//inline wchar_t wchlwr(wchar_t ch)
//{
//   wchar_t temp[2];
//   temp[0] = ch;
//   temp[1] = 0;
//
//   wcslwr(temp);
//
//   return temp[0];
//}
//
////inline char *strrev(char *str)
////{
////      char *p1, *p2;
////
////      if (! str || ! *str)
////            return str;
////
////      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
////      {
////            *p1 ^= *p2;
////            *p2 ^= *p1;
////            *p1 ^= *p2;
////      }
////      return str;
////}
////#endif

// --- resource freeing routines ---

template <class T> void freeobj(T obj)
{
   if (obj != NULL) {
      delete obj;
   }
}

inline void freestr(wchar_t* s)
{
   if (s != NULL) {
      free(s);
   }
}

inline void freestr(char* s)
{
   if (s != NULL) {
      free(s);
   }
}

inline void freestr(unsigned short* s)
{
   if (s != NULL) {
      free(s);
   }
}

// --- alignment routines ---

inline unsigned int align(unsigned int number, const unsigned int alignment)
{
   if (number & (alignment - 1)) {
      return (number & ~(alignment - 1)) + alignment;
   }
   else return number & ~(alignment - 1);
}

// --- miscellaneous routines ---

inline bool ifAny(int target, int value1, int value2)
{
   return target == value1 || target == value2;
}

inline bool test(int number, int mask)
{
   return ((number & mask) == mask);
}

inline bool test(int number, int mask, int value)
{
   return ((number & mask) == value);
}

inline bool isbetween(int starting, int len , int value)
{
   return (starting < value && value < starting + len);
}

inline bool isNumeric(const wchar16_t* s, int length)
{
   for (int i = 0 ; i < length ; i++)
   {
      if (s[i] < '0' || s[i] > '9')
         return false;
   }

   return true;
}

// --- calcTabShift ---

inline size_t calcTabShift(int col, int tabSize)
{
   int nextCol = (col / tabSize * tabSize) + tabSize;

   return nextCol - col;
}

// --- __abs ---

inline int __abs(int x)
{
   return (x ^ (x>>31)) - (x>>31);
}

} // _ELENA_

#endif // toolsH
