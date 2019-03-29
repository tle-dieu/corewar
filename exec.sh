i=0
for arg in $*; do
	./asm $arg
	mv $arg .
done
./corewar 
