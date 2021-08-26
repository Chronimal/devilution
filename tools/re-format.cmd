mkdir ..\re-format
pushd ..\re-format

cmake -T ClangCl ..
msbuild Devilution.sln /t:Build /p:Configuration=Debug;Platform=Win32;RunCodeAnalysis=true

pushd Diablo\Diablo.dir\Debug\Diablo.ClangTidy
python ..\..\..\..\..\tools\run-clang-tidy.py -fix -checks=-*,readability-braces-around-statements
clang-format -i ..\..\..\..\..\Diablo\src\*.cpp
popd

pushd libs\DiabloUI\DiabloUI.dir\Debug\DiabloUI.ClangTidy
python ..\..\..\..\..\..\tools\run-clang-tidy.py -fix -checks=-*,readability-braces-around-statements
clang-format -i  ..\..\..\..\..\..\libs\DiabloUI\src\*.cpp
popd

pushd libs\ddshim\ddshim.dir\Debug\ddshim.ClangTidy
python ..\..\..\..\..\..\tools\run-clang-tidy.py -fix -checks=-*,readability-braces-around-statements
clang-format -i  ..\..\..\..\..\..\libs\ddshim\src\*.cpp
popd

pushd libs\libpk\libpk.dir\Debug\libpk.ClangTidy
python ..\..\..\..\..\..\tools\run-clang-tidy.py -fix -checks=-*,readability-braces-around-statements
clang-format -i  ..\..\..\..\..\..\libs\libpk\src\*.c
popd

pushd libs\libsmacker-1.1.1\libsmacker.dir\Debug\libsmacker.ClangTidy
python ..\..\..\..\..\..\tools\run-clang-tidy.py -fix -checks=-*,readability-braces-around-statements
clang-format -i  ..\..\..\..\..\..\libs\libsmacker-1.1.1\src\*.c
popd

popd
