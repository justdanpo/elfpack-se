@echo off
if %1a==a (
  echo use "$PROJ_DIR$" "$TARGET_DIR$" as arguments
  goto :eof
)

cd /d %1

call :getprojname %2

rmdir /q /s temp
mkdir temp
echo #include "target\%projectname%.h" >temp\temp.h
for /F %%a in (..\include\DYN_syntax.cfg) do (
echo #ifndef %%a^

  #define %%a UNDEFINED^

#endif >>temp\temp.h
)
goto :eof

:getprojname
for /f "tokens=1* delims=\/ " %%a in ( "%~1" ) do (
    set projectname=%prev%
    set prev=%%a
    call :getprojname "%%b"
)
