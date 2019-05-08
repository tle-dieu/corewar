;resources/parsing_breaker/commentary_error.s:23:7: error: expected string after .name
; .name 
;       ^
;resources/parsing_breaker/commentary_error.s:25:14: error: unexpected expression in .comment declaration
;i'm # Zork #""#"
;             ^~~
;resources/parsing_breaker/commentary_error.s:27:1: error: unknow instruction 'li'
;li
;^~
;resources/parsing_breaker/commentary_error.s:28:1: error: unknow instruction 'str'
;str "#"
;^~~
;resources/parsing_breaker/commentary_error.s:30:4: error: '%0'
;missing parameter, have 1 parameter expected 2
;ld %0
;   ^~
;resources/parsing_breaker/commentary_error.s:33:7: error: label 'live' is undefined (1 other)
;zjmp %:live
;       ^
;resources/parsing_breaker/commentary_error.s:33:7: note: undefined label reported only once
;6 errors generated.

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
