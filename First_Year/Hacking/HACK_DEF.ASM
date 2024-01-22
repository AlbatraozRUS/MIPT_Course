.model tiny
.code
org 100h
;__________________________________________________________________________

Start:
	Call Welcome
	Call Scan
	Call Compare
	cmp ax, 1
	je Correct_Password
	jmp Incorrect_Password

Welcome	proc	
	lea ds:dx, Greeting
	mov ah, 09h
	int 21h
	lea ds:dx, FieldForPassword
	int 21h
	ret
	endp

Scan	proc
	lea bx, Input
	mov ah, 01h
rep_Scan:
	int 21h		
	cmp al, 13d
	je end_Scan
	mov [bx], al
	inc bx
	inc cx
	jmp rep_Scan
end_Scan:	
	ret
	endp
	

Compare	proc
	mov  si, offset Input
	mov  di, offset Password
	dec si
	dec di
	mov cx, 3d
rep_com:
	inc si
	inc di
	mov bx, [si]
	mov si, bx
	mov bx, [di]
	mov di, bx
	cmp si, di 
	jne N_Eq
	loop rep_com
	
	mov ax, 1	;1 - means password is correct
	ret
 N_eq:
	mov ax, 0
	ret		;0 - means password is incoorect
	endp

Correct_Password:
	mov ah, 09h
	lea ds:dx, R_password  
	int 21h
	jmp Ex

Incorrect_Password:
	mov ah, 09h
	lea ds:dx, W_password
	int 21h
	jmp Ex	
	
Comment $	
	lea bx, Input
	mov ah, 02h
	mov dl, 13d
	int 21h
	mov dl, 10d
	int 21h
	
Print: 
	mov dl, [bx]
	Call Delay
	int 21h
	inc bx
	loop Print
end_Print:
$



;_________________________________________________________________________

Ex:
	mov ax, 4c00h
	int 21h

Delay	proc
	push cx
	push bx
	mov bx, 15d
rep_Del:
	mov cx, 60000d
rep_Del_1:
	loop rep_Del_1
	dec bx
	cmp bx, 0
	jne rep_Del
	pop bx 
	pop cx
	ret
	endp
	


Input:   db 17 dup (?)
Greeting: db 'Welcome to Pentagon system', 13, 10, 'Please enter super secret passsword!', 13, 10, '$'
FieldForPassword: db '    Password:', '$'
Password: db 'GOD'
R_password: db 13, 10, 13, 10, 'Congratulaions, you have entered right password!', 13, 10, '$'
W_password: db 13, 10, 13, 10, 'Sorry, access denied :(', 13, 10, '$'
end	Start