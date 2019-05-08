;resources/parsing_breaker/comment_double_define.s:9:1: warning: .comment already defined (ignored)
;.comment "Hello, i'm Kroz"
;^~~~~~~~~~~~~~~~~~~~~~~~~~
;1 warning generated.
;Writing output program to resources/parsing_breaker/comment_double_define.cor

.name "Kroz"
.comment "Hello, i'm Kroz"
.comment "Hello, i'm Kroz"
		
l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live
