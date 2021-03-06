/* This -*- C -*- file is meant to be processed by the configure script */

/* Define this if your compiler doesn't support the new C++ 'bool' type */
#define HAVE_BOOL

/* Define if your system supports rint() */
#undef HAVE_RINT

/* Define if your system supports getrusage() */
#undef HAVE_GETRUSAGE

/* Define if your system does not support getrusage() but supports times() */
#undef HAVE_TIMES

/* Define if your system supports random() as opposed to just rand() */
#undef HAVE_RANDOM

/* Define if the STL hash_map template is available */
#undef HAVE_HASH_MAP

/* Define if the STL hash_set template is available */
#undef HAVE_HASH_SET

/* GCC v3 puts hash_map and hash_set in an 'ext' directory */
#undef HAVE_EXT_HASH_MAP
#undef HAVE_EXT_HASH_SET

/* Define if the STL valarray  template is available */
#undef HAVE_VALARRAY

/* Define if the ANSI standard <sstream> is available */
#undef HAVE_SSTREAM

/* Define if the pre-ANSI <strstream> is available */
#undef HAVE_STRSTREAM

/* Define if the gzstream library (a zlib wrapper) is available */
#undef HAVE_GZSTREAM

/* ***** GUI & Image configuration section ***** */

/* Define if the FLTK GUI toolkit is available */
#undef HAVE_FLTK

/* Define if FLTK OpenGL support is available */
#undef HAVE_FLTK_GL

/* Define if Sam Leffler's libtiff is available */
#undef HAVE_LIBTIFF

/* Define the libtiff LZW is available */
#undef HAVE_LIBTIFF_LZW

/* Define if libpng is available */
#undef HAVE_LIBPNG

/* Define if the Independent JPEG Group's libjpeg is available */
#undef HAVE_LIBJPEG

/* ***** OpenGL configuration section ***** */

/* Define to the name of OpenGL implementation (e.g., "OpenGL" or "Mesa") */
#define HAVE_OPENGL

/* Define if glPolygonOffsetEXT is available */
#define HAVE_POLYOFFSET_EXT

/* Define if glPolygonOffset is available */
#define HAVE_POLYOFFSET

/* Define if <GL/glext.h> header is available */
#define HAVE_GL_GLEXT_H

/* Define if <GL/wglext.h> header is available */
#undef HAVE_GL_WGLEXT_H

/* Define if <OpenGL/glext.h> header is available (for Apple) */
#undef HAVE_OPENGL_GLEXT_H
