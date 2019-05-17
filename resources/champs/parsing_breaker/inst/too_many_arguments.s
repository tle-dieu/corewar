;[33mfile:[0m resources/champs/parsing_breaker/inst/too_many_arguments.s
;[1mresources/champs/parsing_breaker/inst/too_many_arguments.s:14:22: [1m[31merror: [0m[1munexpected expression after parameter
;[0msti r1,%:live,%1     i,  
;                     [1m[32m^[0m
;[1mresources/champs/parsing_breaker/inst/too_many_arguments.s:14:5: [1m[31merror: [0m[1m'r1,%:live,%1     i,  '
;too many parameter, have 4 parameter expected 3
;[0msti r1,%:live,%1     i,  
;    [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~[0m
;2 errors generated.

.name "Zork"
.comment "Hello, i'm Zork"          

sti r1,%:live,%1     i,  
ld %0, r16
live:
live %1
zjmp %:live
