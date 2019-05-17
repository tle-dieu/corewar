;[33mfile:[0m resources/champs/parsing_breaker/header/error_combination.s
;[1mresources/champs/parsing_breaker/header/error_combination.s:61:14: [1m[31merror: [0m[1munexpected expression in .name declaration
;[0m.name     "Zork"end
;                [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:62:1: [1m[31merror: [0m[1munknow instruction 'error'
;[0merror
;[1m[32m^[0m[1m[32m~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:63:1: [1m[35mwarning: [0m[1minvalid command '.invalid'[0m
;[0m.invalid
;[1m[32m^[0m[1m[32m~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:64:1: [1m[35mwarning: [0m[1m.name already defined (ignored)
;[0m.name         
;[1m[32m^[0m[1m[32m~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:64:7: [1m[31merror: [0m[1mexpected string after .name
;[0m.name         
;      [1m[32m^[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:65:1: [1m[35mwarning: [0m[1m.name already defined (ignored)
;[0m.name         end
;[1m[32m^[0m[1m[32m~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:65:7: [1m[31merror: [0m[1mexpected string after .name
;[0m.name         end
;      [1m[32m^[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:65:9: [1m[31merror: [0m[1munexpected expression in .name declaration
;[0m.name         end
;              [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:66:10: [1m[31merror: [0m[1mexpected string after .comment
;[0m.comment
;         [1m[32m^[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:67:1: [1m[35mwarning: [0m[1m.comment already defined (ignored)
;[0m.comment ""
;[1m[32m^[0m[1m[32m~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:68:1: [1m[35mwarning: [0m[1m.comment already defined (ignored)
;[0m.comment "Hello
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:70:2: [1m[31merror: [0m[1munexpected expression in .comment declaration
;[0m"f
; [1m[32m^[0m[1m[32m[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:73:1: [1m[35mwarning: [0m[1m.name should be before instruction or label
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:73:1: [1m[35mwarning: [0m[1m.name already defined (ignored)
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:73:7: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;      [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:74:11: [1m[31merror: [0m[1munexpected expression in .name declaration
;[0maaaaaaa"        end
;                [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:75:1: [1m[35mwarning: [0m[1m.name should be before instruction or label
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:75:1: [1m[35mwarning: [0m[1m.name already defined (ignored)
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;[1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/error_combination.s:75:133: [1m[31merror: [0m[1mmissing terminating '"' character
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;                                                                                                                                    [1m[32m^[0m
;9 warnings and 10 errors generated.

.name 	"Zork"end
error
.invalid
.name 		
.name 		end
.comment
.comment ""
.comment "Hello
i'm zork
"f

live %1
.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
aaaaaaa"		end
.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
