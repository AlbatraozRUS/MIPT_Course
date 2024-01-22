.model tiny 
.code
org 100h

Start:	
	mov ah, 02h 
	mov cx, 65082d
	mov dl, 90h
Again:
	int 21h
	loop Again
	
	mov dl, 01h
	int 21h
	mov dl, 92h
	int 21h	

	mov ax, 4c00h
	int 21h

end Start