;resources/parsing_breaker/char_after_comment_mult.s:8:8: error: unexpected expression in .comment declaration
;     Kroz"fin
;          ^~~
;1 error generated.


.name "Zork"
.comment "Hello, i'm Kroz"end

l2:	sti	r1, %:live,%1
	and	r1, %0,r1
live: live %1
	  zjmp %:live
