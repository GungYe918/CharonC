/**
 * Simple Types for UEFI
 */

#ifndef _UEFI_TYPES_H_
#define _UEFI_TYPES_H_

typedef unsigned long long  UINT64;
typedef long long           INT64;
typedef unsigned int        UINT32;
typedef int                 INT32;
typedef unsigned short      UINT16;
typedef unsigned short      CHAR16;
typedef short               INT16;
typedef unsigned char       BOOLEAN;
typedef unsigned char       UINT8;
typedef char                CHAR8;
typedef signed char         INT8;

// 보통 UINTN/INTN 정의도 추가
#if defined(__x86_64__) || defined(_M_X64)
typedef UINT64              UINTN;
typedef INT64               INTN;
#else
typedef UINT32              UINTN;
typedef INT32               INTN;
#endif

#endif /* _UEFI_TYPES_H_ */