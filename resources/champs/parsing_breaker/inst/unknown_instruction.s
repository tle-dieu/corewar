;[33mfile:[0m resources/champs/parsing_breaker/inst/unknown_instruction.s
;[1mresources/champs/parsing_breaker/inst/unknown_instruction.s:11:1: [1m[31merror: [0m[1munknow instruction 'bonjjour'
;[0mbonjjour
;[1m[32m^[0m[1m[32m~~~~~~~[0m
;1 error generated.

.name "Zork"
.comment "Hello, i'm Zork"          

sti r1,%:live,%1      
bonjjour
ld %0, r16
live:
live %1
zjmp %:live
