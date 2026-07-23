echo "Don't forget to install raylib first! If you're on arch linux like me that'd be:"
echo "  sudo pacman -S raylib"
echo "It will be different on distros that don't use pacman."
echo "If you do not heed this warning, the build will fail."
echo "Also, this warning does not mean raylib is not installed, it literally displays every time to keep the install script logic simple."
echo "=="
echo ""

echo "+ time gcc -Ofast -march=native snake.c -o snake -lraylib"
time gcc -Ofast -march=native snake.c -o snake -lraylib
