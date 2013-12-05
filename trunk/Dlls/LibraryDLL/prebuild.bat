@echo off

if %1a==a (
  echo use "$PROJ_DIR$" "$TARGET_DIR$" as arguments
  goto :eof
)

cd /d %1

call :getprojname %2

del /q include.tmp
copy "target\%projectname%.h" include.tmp

goto :eof

:getprojname
for /f "tokens=1* delims=\/ " %%a in ( "%~1" ) do (
    set projectname=%prev%
    set prev=%%a
    call :getprojname "%%b"
)
