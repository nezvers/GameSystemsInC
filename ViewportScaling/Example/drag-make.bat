set NAME=%~n1

del %NAME%.exe
mingw32-make OUTPUT=%NAME%
%NAME%.exe
pause