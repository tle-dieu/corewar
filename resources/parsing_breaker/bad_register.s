.name "Zork"
.comment "Hello, i'm Zork"          

sti r1,%:live,%1           
ld %0, r20
live:
live %1
zjmp %:live
