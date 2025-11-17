char *loadlib_c = "$Id: loadlib.c,v 1.18 1999/04/28 23:41:15 rborges Exp $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <LuaLib\lua.h>
#include <LuaLib\lualib.h>
#include <LuaLib\lauxlib.h>

#include "loadlib.h"

/*
  define one of these macros to define the dl interface: WIN32, SHL, RLD or DLFCN

  systems that support some kind of dynamic linking:
  WIN32: MS Windows 95/98/NT
  SHL: HP-UX
  RLD: NeXT
  DLFCN: Linux, SunOS, IRIX, UNIX_SV, OSF1, SCO_SV, BSD/OS
  DLFCN (simulation): AIX
*/

#if defined(WIN32)
#define MAP_FORMAT "%s%s.dll"
#else
#define MAP_FORMAT "%slib%s.so"
#endif


#if defined(WIN32)

#include <windows.h>
#include <winerror.h>
typedef HINSTANCE libtype;
typedef FARPROC   functype;
#define loadfunc(lib,name) GetProcAddress( lib, name )
#define unloadlibrary(lib) FreeLibrary( lib )
#define liberror()         dll_error( "Could not load library." )
#define funcerror()        dll_error( "Could not load function." )

static libtype loadlibrary( char* path )
{
  libtype libhandle = LoadLibrary( path );
  if (!libhandle)
  {
    int maxtries = 10;
    do 
    {
      maxtries--;
      Sleep( 2 );
      libhandle = LoadLibrary( path );
    } while( !libhandle &&
             maxtries>0 &&
             GetLastError()==ERROR_SHARING_VIOLATION );
  }
  return libhandle;
}
 
#define BUFFER_SIZE 100
 
static char* dll_error( char* altmsg )
{
  static char buffer[BUFFER_SIZE+1];
  if ( FormatMessage( FORMAT_MESSAGE_IGNORE_INSERTS |
                      FORMAT_MESSAGE_FROM_SYSTEM,
                      0, /* source */
                      GetLastError(),
                      0, /* langid */
                      buffer,
                      BUFFER_SIZE,
                      0 /* arguments */ ) )
  {
    return buffer;
  }
  else
  {
    return altmsg;
  }
}
 
#elif defined(DLFCN)

#include <dlfcn.h>
#ifndef RTLD_GLOBAL
#define RTLD_GLOBAL 0
#endif
typedef void* libtype;
typedef lua_CFunction functype;
#define loadlibrary(path)  dlopen( path, RTLD_LAZY | RTLD_GLOBAL )
#define loadfunc(lib,name) (functype)dlsym( lib, name )
#define unloadlibrary(lib) dlclose( lib )
#define liberror()         dlerror()
#define funcerror()        dlerror()

#elif defined(SHL)

#include <dl.h>
typedef shl_t libtype;
typedef lua_CFunction functype;
#define loadlibrary(path)  shl_load( path, BIND_DEFERRED | BIND_NOSTART, 0L )
#define unloadlibrary(lib) shl_unload( lib )
#define liberror()         "Could not load library."
#define funcerror()        "Could not load function."

static functype loadfunc( libtype lib, char *name )
{
  functype fn;
  if ( shl_findsym( &lib, name, TYPE_PROCEDURE, &fn ) == -1 )
    return 0;
  return fn;
}

#elif defined(RLD)

#include <rld.h>
typedef long libtype;
typedef lua_CFunction functype;
#define loadlibrary(path)  rldload( 0, 0, path, 0  )
#define unloadlibrary(lib) ;
#define liberror()         "Could not load library."
#define funcerror()        "Could not load function."

static functype loadfunc( libtype lib, char *name )
{
  functype fn;
  char* _name = (char*)malloc( (strlen( name )+2 )*sizeof( char ));
  if ( !_name ) return 0;
  _name[0] = '_';
  strcpy( _name+1, name );
  if ( !rld_lookup( 0, _name, &fn ) )
  {
    free( _name );
    return 0;
  }
  free( _name );
  return fn;
}

#else

typedef void* libtype;
typedef lua_CFunction functype;
#define loadlibrary(path)  (0)
#define loadfunc(lib,name) (0)
#define unloadlibrary(lib) ;
#define liberror()         "Dynamic libraries not supported."
#define funcerror()        ""

#endif

#define LIBTAG      1
#define UNLOADEDTAG 2

#define FIRSTARG    3


static int gettag ( int i )
{
  return (int)lua_getnumber( lua_getparam( i ) );
}


static libtype check_libhandle( int nparam, lua_Object lh )
{
  luaL_arg_check( lua_isuserdata( lh ),
                  nparam, "userdata expected" );
  luaL_arg_check( lua_tag( lh )==gettag( LIBTAG ),
                  nparam, "not a valid library handle" );
  return (libtype)lua_getuserdata( lh );
}

static void loadlib( void )
{
  int tag = gettag( LIBTAG );
  char *libname = luaL_check_string( FIRSTARG );
  char *path;
  libtype lib;

  if ( strpbrk( libname, ".:/\\" ) )
  {
    path = libname;
  }
  else
  {
    lua_Object param = lua_getparam( FIRSTARG+1 );
    char *dir = "";
    if ( param != LUA_NOOBJECT )
    {
      dir = luaL_check_string( FIRSTARG+1 );
    } 

    path = (char*)malloc( sizeof( char )*( strlen( dir ) +
                                       strlen( libname ) +
                                       strlen( MAP_FORMAT ) + 1 ) );
    if ( !path ) lua_error( "not enough memory." );
    sprintf( path, MAP_FORMAT, dir, libname );
  }

  lib = loadlibrary( path );
  if ( path != libname ) free( path );
  if ( !lib )
  {
    lua_pushnil();
    lua_pushstring( liberror() );
    return;
  }
  lua_pushusertag( lib, tag );
}

static void callfromlib( void )
{
  libtype lh = check_libhandle( FIRSTARG, lua_getparam( FIRSTARG ) );
  char *funcname = luaL_check_string( FIRSTARG+1 );

  functype fn = loadfunc( lh, funcname );
  if ( fn )
  {
    fn();
  }
  else
  {
    lua_error( funcerror() );
  }
}

static void unloadlib( void )
{
  lua_Object lh = lua_getparam( FIRSTARG ); 
  unloadlibrary( check_libhandle( FIRSTARG, lh ) );
  lua_pushobject( lh );
  lua_settag( gettag( UNLOADEDTAG ) );
}

void loadlib_open( void )
{
  static struct luaL_reg funcs[] = {
    {"loadlib",     loadlib},
    {"unloadlib",   unloadlib},
    {"callfromlib", callfromlib}
  };
  int libtag, unloadedtag;
  int i;
  lua_open();
  libtag = lua_newtag();
  unloadedtag = lua_newtag();
  for (i=0; i<sizeof( funcs )/sizeof( funcs[0] ); i++ )
  {
    lua_pushnumber( libtag );
    lua_pushnumber( unloadedtag );
    lua_pushcclosure( funcs[i].func, 2 );
    lua_setglobal( funcs[i].name );
  }
  lua_pushstring( LOADLIB_VERSION );
  lua_setglobal( "LOADLIB_VERSION" );
}
  
