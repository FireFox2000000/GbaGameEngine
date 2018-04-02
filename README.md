# Devkit
Requires a GBA devkit installation for compliation\
This project specifically uses DevKit Advance Release 5 Beta 3, which can be downloaded at http://devkitadv.sourceforge.net/

# Compiling with Visual Studio
The Visual Studio .sln should open into a makefile project\
Create a "bin" folder next to the "src" folder if there isn't one already.\
Right-click on GbaGameEngine solution and go to Properties->Configuration Properties->NMake\
Under General, edit the Build Command Line commands to be
```	
	set path=[Path To Devkit Installation]\devkitadv-r5-beta-3\bin;%PATH%
	make
```
Build as per normal in Visual Studio\

# Compiling with the command line
Navigate to the directory with the included makefile\
Create a "bin" folder next to the "src" folder if there isn't one already.\
Set path using
```
	set path=[Path To Devkit Installation]\devkitadv-r5-beta-3\bin;%PATH%
```
Run the makefile using
```
	make
```
