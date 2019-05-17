;[33mfile:[0m resources/champs/parsing_breaker/header/too_long_mult1.s
;[1mresources/champs/parsing_breaker/header/too_long_mult1.s:7:7: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m.name "aaaaaaaaaaaa
;      [1m[32m^[0m[1m[32m~~~~~~~~~~~~[0m
;1 error generated.

.name "aaaaaaaaaaaa
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
.comment "Hello, i'm Kroz"
		
l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live
