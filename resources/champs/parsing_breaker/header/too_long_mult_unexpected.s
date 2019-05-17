;[33mfile:[0m resources/champs/parsing_breaker/header/too_long_mult_unexpected.s
;[1mresources/champs/parsing_breaker/header/too_long_mult_unexpected.s:10:7: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m.name "iiiii
;      [1m[32m^[0m[1m[32m~~~~~[0m
;[1mresources/champs/parsing_breaker/header/too_long_mult_unexpected.s:11:127: [1m[31merror: [0m[1munexpected expression in .name declaration
;[0miiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"fin
;                                                                                                                              [1m[32m^[0m[1m[32m~~[0m
;2 errors generated.

.name "iiiii
iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii"fin
.comment "Hello, i'm Kroz"
		
l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live
