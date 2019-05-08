;Writing output program to resources/parsing_breaker/commentary.cor
;name: '#Zork#'
;comment: 'Hello#,\n#i'm Zork#'

#.name "Zork"
			#.name "Zork"
		#.name ###"Zork"#
 .name "#Zork#"
.comment "Hello#,
#i'm Zork#"#end         

sti r1,%:live,%1           
ld %0, r16
live: #sti r1,%:live,%1           
#live:
live %1
zjmp %:live
