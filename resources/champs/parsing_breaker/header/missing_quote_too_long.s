;[1mresources/champs/parsing_breaker/header/missing_quote_too_long.s:10:7: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;      [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/header/missing_quote_too_long.s:10:1: [1m[31merror: [0m[1mmissing terminating '"' character
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
;                                                                                                                                         [1m[32m^[0m
;2 errors generated.

.comment "Hello, i'm Kroz"
.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
i	
l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live