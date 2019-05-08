;[1mresources/champs/parsing_breaker/commentary_error.s:23:7: [1m[31merror: [0m[1mexpected string after .name
;[0m .name 
;       [1m[32m^[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:25:14: [1m[31merror: [0m[1munexpected expression in .comment declaration
;[0mi'm # Zork #""#"
;             [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:27:1: [1m[31merror: [0m[1munknow instruction 'li'
;[0mli
;[1m[32m^[0m[1m[32m~[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:28:1: [1m[31merror: [0m[1munknow instruction 'str'
;[0mstr "#"
;[1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:30:4: [1m[31merror: [0m[1m'%0'
;missing parameter, have 1 parameter expected 2
;[0mld %0
;   [1m[32m^[0m[1m[32m~[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:33:7: [1m[31merror: [0m[1mlabel 'live' is undefined (1 other)
;[0mzjmp %:live
;       [1m[32m^[0m
;[1mresources/champs/parsing_breaker/commentary_error.s:33:7: [1m[30mnote: [0m[1mundefined label reported only once
;[0m6 errors generated.

 .name #"Zork#"#end
.comment "#Hello,
i'm # Zork #""#"#end

li#ve:
str "#"#end
sti r1,%:live,%1           
ld %0#, r16
#live: #sti r1,%:live,%1           
live %1
zjmp %:live
