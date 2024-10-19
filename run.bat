@echo off
color 7
echo Current directory: %cd%

if exist build (
    echo Deleting build directory...
    rmdir /s /q build
)

echo Uninstalling previous version of uniezgame...
pip uninstall uniezgame --yes

echo Building extension...
python .\setup.py build_ext --inplace
echo Build done

echo Compiling package...
pip install .
echo Install done

rem Add a delay before running the post_install script
echo Running post-install script...
timeout /t 3 > nul
python .\post_install.py

pause
