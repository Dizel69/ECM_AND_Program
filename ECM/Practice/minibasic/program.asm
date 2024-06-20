include\masm32\include\masm32rt.inc

.386

.data?
hout dd ?
 
.data

.code
start:
label20:
mov eax, x286
cmp eax, x287
JE label30
label30:

invoke GetStdHandle, STD_OUTPUT_HANDLE
mov hout, eax
mov eax, x44
cmp eax, 1
JE labelR
print " NET"
jmp labelE
labelR:
print " DA"
labelE:
mov eax, x70
cmp eax, 1
JE labelR2
print " NET"
jmp labelE2
labelR2:
print " DA"
labelE2:

inkey
exit
end start