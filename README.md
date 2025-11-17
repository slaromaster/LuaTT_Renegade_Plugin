# Introduction
LuaTT is a plugin created by Sla Master (Thanks jnz for original LuaPlugin and jonwil for various help with the plugin and ExEric3 for server testing and hosting) for SSGM TT (C&C Renegade server) or Dragonade. 

To use this plugin, you must install Tiberian Technologies into your C&C Renegade Server (FDS - Free Dedicated Server).

This plugin enables Lua programming language for your server.

LuaTT v6 uses LuaJIT (Lua 5.1) and SQLite. Is compatible with LuaPlugin from SSGM 3.4.1, some functions might not work, read the documentation.

# Building
You must have the Visual Studio 2012 or 2013 installed.

You must get [SSGM Source](https://www.tiberiantechnologies.org/Downloads) or [Dragonade Source](https://github.com/TheUnstoppable/Dragonade).

LuaTT was built using Visual Studio 2013 (v120). You can also build with v110.

* Add the LuaTT source in the folder alongside SSGM/Dragonade source.

* Set the LuaTT build as 'Release SSGM' if you are using SSGM or 'Release DA' if you are using Dragonade (we recommend to use DA because of more functions/options).

* Build the scripts first and then LuaTT, should produce the LuaTT.dll

# Download
Download from 'Releases'

# How to use
* You must have SSGM or DA server setup. 

* Load the plugin into DA.ini or SSGM.ini.

* Have your Lua scripts in LuaPlugin (see example)

* Start FDS server

# LuaTT Documentation
TODO