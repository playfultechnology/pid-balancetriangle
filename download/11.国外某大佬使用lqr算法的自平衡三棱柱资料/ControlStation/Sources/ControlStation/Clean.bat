for /d %%a in (*) do (
cd %%a
call "Clean.bat"
cd ..
)