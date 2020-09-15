/*****************************************************************************

  File:  oamalloc.h

  Description: This file is gives prototypes for the memory managment functions
               OaMalloc, OaFree and OaRealloc.
 
  Author:  Steve Monk, University of Colorado LASP
 
  Creation Date:   6 Dec   1995
  Last Modified:  21 Mar   1997

  History:

    Creation - This include file was part of the first post-Beta Release of the
               OA library.  These prototypes were previously located in oal.h.
    03/21/97 - Added old_size argument to OaRealloc. SM

*****************************************************************************/

#ifndef OA_MALLOC_INCLUDED
#define OA_MALLOC_INCLUDED 1

// !!! MECHSOFT START !!!
#ifdef _MSC_VER
#	define LABLIB3_EXPORT		__declspec(dllexport)
#	define LABLIB3_IMPORT		__declspec(dllimport)
#else
//#	define LABLIB3_EXPORT		__attribute__((__visibility__("default")))
#	define LABLIB3_EXPORT
#	define LABLIB3_IMPORT
#endif

#ifdef __cplusplus
#	define LABLIB3_EXTERN_C		extern "C"
#else
#	define LABLIB3_EXTERN_C
#endif

#ifdef BUILDING_LABLIB3
#	define LABLIB3_DLL			LABLIB3_EXTERN_C LABLIB3_EXPORT
#else
#	define LABLIB3_DLL			LABLIB3_EXTERN_C LABLIB3_IMPORT
#endif
// !!! MECHSOFT END !!!

#include <stdlib.h>
#if (defined( IBM_PC) && defined( MS_DOS))
#include <alloc.h>
#endif

#ifdef _NO_PROTO

extern char *    OaMalloc();
extern void      OaFree();
extern char *    OaRealloc();

#else

LABLIB3_DLL char *    OaMalloc( long bytes);
LABLIB3_DLL void      OaFree( char *ptr);
LABLIB3_DLL char *    OaRealloc( char *old_ptr, long old_size, long bytes);

#endif  /* ifdef _NO_PROTO  */
#endif  /* ifndef OA_MALLOC_INCLUDED  */
