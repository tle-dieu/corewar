;[33mfile:[0m resources/champs/parsing_breaker/inst/bad_type.s
;[1mresources/champs/parsing_breaker/inst/bad_type.s:10:5: [1m[31merror: [0m[1mparameter[1] type is INDIRECT, expected type(s) (REGISTER) for instruction 'sti'
;[0msti 1,%:live,%1  
;    [1m[32m^[0m[1m[32m[0m
;1 error generated.

.name "Zork"
.comment "Hello, i'm Zork"          

sti 1,%:live,%1  
ld %0, r16
live:
live %1
zjmp %:live
