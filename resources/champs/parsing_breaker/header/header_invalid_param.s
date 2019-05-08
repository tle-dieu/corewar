;[1mresources/champs/parsing_breaker/header/header_invalid_param.s:10:6: [1m[31merror: [0m[1mparameter[1] type is REGISTER, expected type(s) (DIRECT) for instruction 'live'
;[0mlive r1
;      [1m[32m^[0m
;[1mresources/champs/parsing_breaker/header/header_invalid_param.s:11:1: [1m[35mwarning: [0m[1m.comment should be before instruction or label
;[0m.comment "Hello, i'm Zork"          
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;1 warning and 1 error generated.

.name "Zork"
live r1
.comment "Hello, i'm Zork"          

live %1