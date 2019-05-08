;resources/parsing_breaker/comment_midle.s:12:1: warning: .comment should be before instruction or label
;.comment "Hello, i'm Kroz"
;^~~~~~~~~~~~~~~~~~~~~~~~~~
;1 warning generated.
;Writing output program to resources/parsing_breaker/comment_midle.cor

.name "Kroz"

l2:	sti	r1, %:live,%1
	and	r1, %0,r1

.comment "Hello, i'm Kroz"

live: live %1
	  zjmp %:live
