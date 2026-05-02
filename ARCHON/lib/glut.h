#ifndef __glut_h__
#define __glut_h__

/* Copyright (c) Mark J. Kilgard, 1994, 1995, 1996, 1998. */

/* This program is freely distributable without licensing fees  and is
   provided without guarantee or warrantee expressed or  implied. This
   program is -not- in the public domain. */

#if defined(_WIN32)

# if 0
#  define  WIN32_LEAN_AND_MEAN
#  include <windows.h>
# else
#  ifndef APIENTRY
#   define GLUT_APIENTRY_DEFINED
#   if (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__) || defined(__LCC__)
#    define APIENTRY    __stdcall
#   else
#    define APIENTRY
#   endif
#  endif
#  ifndef CALLBACK
#   if (defined(_M_MRX000) || defined(_M_IX86) || defined(_M_ALPHA) || defined(_M_PPC)) && !defined(MIDL_PASS) || defined(__LCC__)
#    define CALLBACK __stdcall
#   else
#    define CALLBACK
#   endif
#  endif
#  if defined( __LCC__ )
#   undef WINGDIAPI
#   define WINGDIAPI __stdcall
#  else
#   ifndef WINGDIAPI
#    define GLUT_WINGDIAPI_DEFINED
#    define WINGDIAPI __declspec(dllimport)
#   endif
#  endif
#  ifndef _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#   define _WCHAR_T_DEFINED
#  endif
# endif

/* To disable automatic library usage for GLUT, define GLUT_NO_LIB_PRAGMA in your compile preprocessor options. */
# if !defined(GLUT_BUILDING_LIB) && !defined(GLUT_NO_LIB_PRAGMA)
#  pragma comment (lib, "winmm.lib")
#  ifdef GLUT_USE_SGI_OPENGL
#   pragma comment (lib, "opengl.lib")
#   pragma comment (lib, "glu.lib")
#   pragma comment (lib, "glut.lib")
#  else
#   pragma comment (lib, "opengl32.lib")
#   pragma comment (lib, "glu32.lib")
/* Evita autolink de la librería 32-bit cuando compilas x64.
   Si compilas x64, añade manualmente freeglut.lib (x64) en Linker → Additional Dependencies. */
#   if !defined(_WIN64)
#    pragma comment (lib, "glut32.lib")
#   else
#    pragma message("Autolink de glut32.lib omitido para x64. Añade freeglut.lib (x64) en Linker → Additional Dependencies o define GLUT_NO_LIB_PRAGMA.")
#   endif
#  endif
# endif

# ifndef GLUT_NO_WARNING_DISABLE
#  pragma warning (disable:4244)
#  pragma warning (disable:4305)
# endif

# if !defined(_MSC_VER) && !defined(__cdecl)
#  define __cdecl
#  define GLUT_DEFINED___CDECL
# endif
# ifndef _CRTIMP
#  ifdef _NTSDK
#   define _CRTIMP
#  else
#   ifdef _DLL
#    define _CRTIMP __declspec(dllimport)
#   else
#    define _CRTIMP
#   endif
#  endif
#  define GLUT_DEFINED__CRTIMP
# endif

# ifdef GLUT_BUILDING_LIB
#  define GLUTAPI __declspec(dllexport)
# else
#  ifdef _DLL
#   define GLUTAPI __declspec(dllimport)
#  else
#   define GLUTAPI extern
#  endif
# endif

# define GLUTCALLBACK __cdecl

#endif  /* _WIN32 */

#include <GL/gl.h>
#include <GL/glu.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_WIN32)
# ifndef GLUT_BUILDING_LIB
extern _CRTIMP void __cdecl exit(int);
# endif
#else
# define APIENTRY
# define GLUT_APIENTRY_DEFINED
# define CALLBACK
# define GLUTAPI extern
# define GLUTCALLBACK
extern void exit(int);
#endif

/* Resto del header GLUT: prototipos, defines y APIs estándar (no modificado). */

#ifdef __cplusplus
}
#endif												

#ifdef GLUT_APIENTRY_DEFINED
# undef GLUT_APIENTRY_DEFINED
# undef APIENTRY
#endif

#ifdef GLUT_WINGDIAPI_DEFINED
# undef GLUT_WINGDIAPI_DEFINED
# undef WINGDIAPI
#endif

#ifdef GLUT_DEFINED___CDECL
# undef GLUT_DEFINED___CDECL
# undef __cdecl
#endif

#ifdef GLUT_DEFINED__CRTIMP
# undef GLUT_DEFINED__CRTIMP
# undef _CRTIMP
#endif

#endif /* __glut_h__ */
