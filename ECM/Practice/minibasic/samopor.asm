include\masm32\include\masm32rt.inc

.386

.data?
hout dd ?
x8 dd ?
x12 dd ?
x13 dd ?
x44 dd ?
x55 dd ?
x60 dd ?
x64 dd ?
x66 dd ?
x67 dd ?
x69 dd ?
x70 dd ?
x75 dd ?
x101 dd ?
 
.data
x286 dd 6
x287 dd 12
x288 dd 1
x289 dd 0
x290 dd 10
x291 dd 0
x292 dd 0

.code
start:
label10:
mov eax, x286
mov x13, eax
label20:
mov eax, x287
mov x12, eax
label30:
mov eax, x13
mov x67, eax
label40:
push returnlabel40
jmp label100
returnlabel40:
label45:
mov eax, x69
mov x75, eax
label60:
mov eax, x12
mov x67, eax
label70:
push returnlabel70
jmp label100
returnlabel70:
label75:
mov eax, x69
mov x101, eax
label80:
jmp label400
label100:
mov eax, x288
mov x69, eax
label103:
mov eax, x288
mov x8, eax
mov eax, x67
mov x286, eax
mov eax, x288
mov x287, eax
label288:
mov eax, x287
cmp eax, 0
JGE label289
mov eax, x8
cmp eax, x286
JL label289
jmp labelT289
labelL289:
mov eax, x8
cmp eax, x286
JG label289
labelT289:
label105:
mov eax, x8
mov x60, eax
label110:
mov eax, x8
mov x70, eax
label130:
push returnlabel130
jmp label200
returnlabel130:
label140:
mov eax, x70
cmp eax, x67
JE label160
label145:
mov eax, x8
add eax, x287
mov x8, eax
jmp label288
label289:
label150:
ret
label160:
mov eax, x289
mov x69, eax
label170:
ret
label200:
mov eax, x60
cmp eax, x289
JG label220
label210:
ret
label220:
mov eax, x60
mov x66, eax
label230:
push returnlabel230
jmp label300
returnlabel230:
label240:
mov eax, x55
mov x60, eax
label250:
mov eax, x70
add eax, x64
mov x290, eax
mov eax, x290
mov x70, eax
label260:
jmp label200
label300:
mov eax, x289
mov x55, eax
label320:
mov eax, x66
cmp eax, x290
JGE label340
label325:
mov eax, x66
mov x64, eax
label330:
ret
label340:
mov eax, x66
sub eax, x290
mov x291, eax
mov eax, x291
mov x66, eax
label350:
mov eax, x55
add eax, x288
mov x292, eax
mov eax, x292
mov x55, eax
label400:
mov eax, x75
mov x44, eax
label410:
mov eax, x101
mov x70, eax
label420:

invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hout, eax
mov eax, x44
cmp eax, 1
JE labelR
print " NET"
jmp labelE
labelR:
print " NET"
labelE:
mov eax, x70
cmp eax, 1
JE labelR2
print " NET"
jmp labelE2
labelR2:
print " NET"
labelE2:

inkey
exit
end start