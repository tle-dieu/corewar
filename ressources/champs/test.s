.name "acompagn"
.comment "Bouh attention wouwoup je fais peur"

ld %11173625857, r9
ld %2, r8
sti r8 , %-429, %-429
sti r9, %-437, %-424
begin:	ld %1, r3
		st r1, r5
		sti r3,%-3,%-1
		sti r3,%-18,%-1
		sti r3,%-12,%-1
fly:	sti r5,%100,%100
		sti r3,%100,%89
		sti r5,%150,%150
		sti r3,150,%289
		sti r5,%200,%200
		sti r3,%200,%189
		ld %167770880, r6
		sti r6,%100,%55
		live %12
		sti r1,%12,%6
		ld %0, r2
		fork %:erase
		live %1
		sti r3,%-300,%-300
		sti r1,%-300,%-303
		ld %590804, r4
		sti r4, %-900, %-1
		fork %:second
		fork %:fly
		zjmp %:fly
		fork %:fly
second: sti r3, %130,%222
		sti r1, %130,%219
		fork %0
		live %1
erase: sti r1, %-32,%-54
		sti r3,%-32,%-65
