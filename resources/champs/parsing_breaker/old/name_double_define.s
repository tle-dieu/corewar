;resources/parsing_breaker/name_double_define.s:9:1: warning: .name already defined (ignored)
;.name "Kroz"
;^~~~~~~~~~~~
;1 warning generated.
;Writing output program to resources/parsing_breaker/name_double_define.cor

.name "Kroz"
.comment "Hello, i'm Kroz"
.name "Kroz"
		
l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live
