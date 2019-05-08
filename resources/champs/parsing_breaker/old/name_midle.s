;resources/parsing_breaker/name_midle.s:12:1: warning: .name should be before instruction or label
;.name "Kroz"
;^~~~~~~~~~~~
;1 warning generated.
;Writing output program to resources/parsing_breaker/name_midle.cor

.comment "Hello, i'm Kroz"
		
l2:	sti	r1, %:live,%1
	and	r1, %0,r1

.name "Kroz"

live: live %1
	  zjmp %:live
