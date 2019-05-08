;[1mresources/champs/parsing_breaker/label/label_redeclaration.s:22:1: [1m[35mwarning: [0m[1mredefinition of label 'label' not allowed (previous definition line 20)
;[0mlabel:
;[1m[32m^[0m
;[1mresources/champs/parsing_breaker/label/label_redeclaration.s:24:1: [1m[35mwarning: [0m[1mredefinition of label 'label' not allowed (previous definition line 20)
;[0mlabel:
;[1m[32m^[0m
;2 warnings generated.
;00000000:  [38;2;155;89;182m00 ea 83 f3 [38;2;241;196;15m5a 6f 72 6b  00 00 00 00 00 00 00 00 [0m [38;2;155;89;182m....[38;2;241;196;15mZork........[0m
;00000010:  [38;2;241;196;15m00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 [0m [38;2;241;196;15m................[0m
;*
;00000080:  [38;2;241;196;15m00 00 00 00 [0m00 00 00 00  [38;2;46;204;113m00 00 00 0a [38;2;52;152;240m48 65 6c 6c [0m [38;2;241;196;15m....[0m....[38;2;46;204;113m....[38;2;52;152;240mHell[0m
;00000090:  [38;2;52;152;240m6f 2c 20 69 27 6d 20 5a  6f 72 6b 00 00 00 00 00 [0m [38;2;52;152;240mo, i'm Zork.....[0m
;000000a0:  [38;2;52;152;240m00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 [0m [38;2;52;152;240m................[0m
;*
;00000890:  [38;2;231;76;60m01 00 00 00 00 01 ff ff  ff fb [0m                   [38;2;231;76;60m..........[0m

.name "Zork"
.comment "Hello, i'm Zork"          

label:
live %:label
label:
live %:label
label: