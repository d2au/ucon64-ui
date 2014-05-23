uCON64 frontend 1.1 README


Note to Mac OS X users: You need to install a missing part of X11, available at
http://www.apple.com/macosx/features/x11/. You can also go directly to
http://www.apple.com/macosx/features/x11/download/. Once you have installed
X11User.dmg you can run X11 applications, like uCON64 frontend.

Before you try to use the frontend, take a minute to configure it. Go to
Frontend -> Options -> uCON64 tab sheet. You need to fill in at least the top
two fields ("Path to uCON64 executable" and "uCON64 configuration file
directory"). The frontend is meant for uCON64 2.0.0, so please do not use it
with older versions. The Windows version of the frontend only works with a
Windows version of uCON64. It does not work in combination with the DOS version
of uCON64.
I advise to also fill in the field "uCON64 configuration directory". Note to
Windows XP users: THIS IS THE DIRECTORY WHERE YOU SHOULD PUT AN I/O DRIVER DLL.
If you have no idea what I am talking about, see the uCON64 FAQ 
(http://ucon64.sourceforge.net/ucon64/faq.html#4).
Any setting you make in the section "Override configuration file settings" can
also be made in the configuration file itself. For example, instead of
specifying a configuration directory in the frontend you could also press the
button "Edit configuration file", search for the line that starts with
"ucon64_configdir=" (without the quotes) and specify a directory there.

One note about the parallel port address: by default uCON64 creates a
configuration file with a line that specifies a parallel port address. If the
parallel port of your PC is mapped to a standard address you can make uCON64
automatically detect it by *not* having that line in your uCON64 configuration
file. Either remove the line or disable it by putting a hash symbol ("#") in
front of it. If you want uCON64 to automatically detect the parallel port
address of your PC you obviously should not check the override check box for
the option "(Parallel) port address" either. That said, it is usually better to
specify an address.

On the Frontend tab sheet (Frontend -> Options -> Frontend tab sheet) you can
specify what tab sheets should be enabled in the main window. It is not
possible to disable the uCON64 output tab sheet. The Browse tab sheet
influences the behaviour of the buttons in the other tab sheets of the main
window. If it is enabled, you can select a file with it. The buttons in the
other tab sheets will use the files that are selected in the Browse tab sheet
(if they need a file) and will not specifically ask for a file. If the Browse
tab sheet is not enabled, the buttons that need a file ask for one every time
you press them.

The check box "Use intermediary files for output (use with care!)" should
usually not be checked. If the frontend does not catch all (display) output
produced by uCON64, checking this option may help. It slows down uCON64,
though.

After you have made the settings press the button "Save settings" if you want
the frontend to use the same settings the next time you start it.

Quite a few options need an argument and the frontend will ask you to specify
one if you forgot. However, some options take an optional argument. For
example, splitting a Genesis ROM dump will work if you simply press on the
Split button in the Genesis tab sheet. You can also specify a part size by
filling in the field "Part size". In other words, you do not have to specify an
argument if the frontend does not specifically ask for one.

I do not provide binaries for all platforms on which the frontend may compile
and run. The main reason is that the binary compatibility on Unix (GNU/Linux,
FreeBSD, Solaris etc.) is not that good. Users of Unix-like platforms have to
compile the frontend from source. It should take no more than installing FOX
(http://www.fox-toolkit.org, version 1.2, 1.3 and 1.4 have been tested with
success), editing the Makefile so that the correct include and library
directories will be used and then running make.
FOX does not compile on a default Red Hat Linux 8 set-up with the default
configuration options. It should compile fine when you use the configuration
option --without-xrandr (use "./configure --without-xrandr"). If you encounter
problems with FOX please contact the author, Jeroen van der Zijp. It is best to
post a message on the FOX GUI users mailing list. If you encounter problems
with compiling the frontend feel free to contact me.

If a feature does not work correctly please contact me and will fix it as soon
as possible.

uCON64 frontend (uf) is free software. You can redistribute it or modify it
under the terms of the GNU General Public License as published by the Free
Software Foundation, either version 2 of the License, or any later version.
uCON64 frontend should come with a copy of the GNU General Public License,
gpl.txt.

Any constructive feedback about the frontend is welcome.

dbjh (dbjh@users.sourceforge.net)
