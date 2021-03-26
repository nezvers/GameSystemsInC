set output=main

del %output%.exe
mingw32-make OUTPUT=%output%
%output%.exe
pause