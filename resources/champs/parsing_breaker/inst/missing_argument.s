;[33mfile:[0m resources/champs/parsing_breaker/inst/missing_argument.s
;[1mresources/champs/parsing_breaker/inst/missing_argument.s:11:5: [1m[31merror: [0m[1m'r1,%:live  '
;missing parameter, have 2 parameter expected 3
;[0msti r1,%:live  
;    [1m[32m^[0m[1m[32m~~~~~~~~[0m
;1 error generated.

.name "Zork"
.comment "Hello, i'm Zork"          

sti r1,%:live  
ld %0, r16
live:
live %1
zjmp %:live
