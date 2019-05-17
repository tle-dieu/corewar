;[33mfile:[0m resources/champs/parsing_breaker/other/wave_spaces.s
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:49:7: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"                         
;      [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:51:3: [1m[35mwarning: [0m[1m.name already defined (ignored)
;[0m     .name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"                         end        
;     [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:51:9: [1m[31merror: [0m[1m.name declaration too long (Max length: 128)
;[0m     .name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"                         end        
;           [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:51:147: [1m[31merror: [0m[1munexpected expression in .name declaration
;[0m     .name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"                         end        
;                                                                                                                                                                       [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:54:3: [1m[35mwarning: [0m[1minvalid command '.invalid_cmd'[0m
;[0m     .invalid_cmd            "test"        
;     [1m[32m^[0m[1m[32m~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:55:3: [1m[35mwarning: [0m[1minvalid command '.invalid_cmd2'[0m
;[0m     .invalid_cmd2                    
;     [1m[32m^[0m[1m[32m~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:57:4: [1m[35mwarning: [0m[1m.comment should be before instruction or label
;[0m   .comment "description"         
;   [1m[32m^[0m[1m[32m~~~~~~~~~~~~~~~~~~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:59:2: [1m[31merror: [0m[1munknow instruction 'sta'
;[0m    sta
;    [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:62:6: [1m[31merror: [0m[1minvalid parameter
;[0mand %param        , r1, r1
;     [1m[32m^[0m[1m[32m~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:64:16: [1m[31merror: [0m[1munexpected expression after parameter
;[0mand r1, r1, r1 end        
;               [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:65:14: [1m[31merror: [0m[1munexpected expression after parameter
;[0mand r1, r1         end     , r1        
;                   [1m[32m^[0m[1m[32m~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:66:6: [1m[31merror: [0m[1minvalid parameter
;[0mand     param        , r1, r1
;        [1m[32m^[0m[1m[32m~~~~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:67:10: [1m[31merror: [0m[1mregister index must be between 1 and 16 have 20
;[0mand r1, r20        ,r1
;         [1m[32m^[0m[1m[32m~[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:68:11: [1m[31merror: [0m[1millegal character for label
;[0mand r1,     ra        ,r1
;             [1m[32m^[0m
;[1mresources/champs/parsing_breaker/other/wave_spaces.s:69:2: [1m[35mwarning: [0m[1mredefinition of label 'label' not allowed (previous definition line 61)
;[0m    label:            
;    [1m[32m^[0m
;5 warnings and 10 errors generated.

.name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"						 

	 .name "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"						 end		


	 .invalid_cmd			"test"		
	 .invalid_cmd2					
live %1
   .comment "description"		 

	sta

	label:
and %param		, r1, r1
live	%:label		
and r1, r1, r1 end		
and r1, r1 		end 	, r1		
and 	param		, r1, r1
and r1, r20		,r1
and r1, 	ra		,r1
	label:			
