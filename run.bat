@echo off
echo 正在编译项目...
gcc -o gcn.exe main.c gcn.c TsMatrix.c -lm
if %errorlevel% neq 0 (
    echo 编译失败！
    pause
    exit /b %errorlevel%
)
echo 编译成功！
echo 正在运行程序...
echo.
gcn.exe
pause