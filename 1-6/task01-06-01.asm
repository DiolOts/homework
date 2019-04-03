format PE GUI 4.0
entry start

include 'win32a.inc'

;---------------------------------------------

section '.data' data readable writeable

crc32table dd 0x000000000, 0x077073096, 0x0ee0e612c, 0x0990951ba, 0x0076dc419
        dd 0x0706af48f, 0x0e963a535, 0x09e6495a3, 0x00edb8832, 0x079dcb8a4
        dd 0x0e0d5e91e, 0x097d2d988, 0x009b64c2b, 0x07eb17cbd, 0x0e7b82d07
        dd 0x090bf1d91, 0x01db71064, 0x06ab020f2, 0x0f3b97148, 0x084be41de
        dd 0x01adad47d, 0x06ddde4eb, 0x0f4d4b551, 0x083d385c7, 0x0136c9856
        dd 0x0646ba8c0, 0x0fd62f97a, 0x08a65c9ec, 0x014015c4f, 0x063066cd9
        dd 0x0fa0f3d63, 0x08d080df5, 0x03b6e20c8, 0x04c69105e, 0x0d56041e4
        dd 0x0a2677172, 0x03c03e4d1, 0x04b04d447, 0x0d20d85fd, 0x0a50ab56b
        dd 0x035b5a8fa, 0x042b2986c, 0x0dbbbc9d6, 0x0acbcf940, 0x032d86ce3
        dd 0x045df5c75, 0x0dcd60dcf, 0x0abd13d59, 0x026d930ac, 0x051de003a
        dd 0x0c8d75180, 0x0bfd06116, 0x021b4f4b5, 0x056b3c423, 0x0cfba9599
        dd 0x0b8bda50f, 0x02802b89e, 0x05f058808, 0x0c60cd9b2, 0x0b10be924
        dd 0x02f6f7c87, 0x058684c11, 0x0c1611dab, 0x0b6662d3d, 0x076dc4190
        dd 0x001db7106, 0x098d220bc, 0x0efd5102a, 0x071b18589, 0x006b6b51f
        dd 0x09fbfe4a5, 0x0e8b8d433, 0x07807c9a2, 0x00f00f934, 0x09609a88e
        dd 0x0e10e9818, 0x07f6a0dbb, 0x0086d3d2d, 0x091646c97, 0x0e6635c01
        dd 0x06b6b51f4, 0x01c6c6162, 0x0856530d8, 0x0f262004e, 0x06c0695ed
        dd 0x01b01a57b, 0x08208f4c1, 0x0f50fc457, 0x065b0d9c6, 0x012b7e950
        dd 0x08bbeb8ea, 0x0fcb9887c, 0x062dd1ddf, 0x015da2d49, 0x08cd37cf3
        dd 0x0fbd44c65, 0x04db26158, 0x03ab551ce, 0x0a3bc0074, 0x0d4bb30e2
        dd 0x04adfa541, 0x03dd895d7, 0x0a4d1c46d, 0x0d3d6f4fb, 0x04369e96a
        dd 0x0346ed9fc, 0x0ad678846, 0x0da60b8d0, 0x044042d73, 0x033031de5
        dd 0x0aa0a4c5f, 0x0dd0d7cc9, 0x05005713c, 0x0270241aa, 0x0be0b1010
        dd 0x0c90c2086, 0x05768b525, 0x0206f85b3, 0x0b966d409, 0x0ce61e49f
        dd 0x05edef90e, 0x029d9c998, 0x0b0d09822, 0x0c7d7a8b4, 0x059b33d17
        dd 0x02eb40d81, 0x0b7bd5c3b, 0x0c0ba6cad, 0x0edb88320, 0x09abfb3b6
        dd 0x003b6e20c, 0x074b1d29a, 0x0ead54739, 0x09dd277af, 0x004db2615
        dd 0x073dc1683, 0x0e3630b12, 0x094643b84, 0x00d6d6a3e, 0x07a6a5aa8
        dd 0x0e40ecf0b, 0x09309ff9d, 0x00a00ae27, 0x07d079eb1, 0x0f00f9344
        dd 0x08708a3d2, 0x01e01f268, 0x06906c2fe, 0x0f762575d, 0x0806567cb
        dd 0x0196c3671, 0x06e6b06e7, 0x0fed41b76, 0x089d32be0, 0x010da7a5a
        dd 0x067dd4acc, 0x0f9b9df6f, 0x08ebeeff9, 0x017b7be43, 0x060b08ed5
        dd 0x0d6d6a3e8, 0x0a1d1937e, 0x038d8c2c4, 0x04fdff252, 0x0d1bb67f1
        dd 0x0a6bc5767, 0x03fb506dd, 0x048b2364b, 0x0d80d2bda, 0x0af0a1b4c
        dd 0x036034af6, 0x041047a60, 0x0df60efc3, 0x0a867df55, 0x0316e8eef
        dd 0x04669be79, 0x0cb61b38c, 0x0bc66831a, 0x0256fd2a0, 0x05268e236
        dd 0x0cc0c7795, 0x0bb0b4703, 0x0220216b9, 0x05505262f, 0x0c5ba3bbe
        dd 0x0b2bd0b28, 0x02bb45a92, 0x05cb36a04, 0x0c2d7ffa7, 0x0b5d0cf31
        dd 0x02cd99e8b, 0x05bdeae1d, 0x09b64c2b0, 0x0ec63f226, 0x0756aa39c
        dd 0x0026d930a, 0x09c0906a9, 0x0eb0e363f, 0x072076785, 0x005005713
        dd 0x095bf4a82, 0x0e2b87a14, 0x07bb12bae, 0x00cb61b38, 0x092d28e9b
        dd 0x0e5d5be0d, 0x07cdcefb7, 0x00bdbdf21, 0x086d3d2d4, 0x0f1d4e242
        dd 0x068ddb3f8, 0x01fda836e, 0x081be16cd, 0x0f6b9265b, 0x06fb077e1
        dd 0x018b74777, 0x088085ae6, 0x0ff0f6a70, 0x066063bca, 0x011010b5c
        dd 0x08f659eff, 0x0f862ae69, 0x0616bffd3, 0x0166ccf45, 0x0a00ae278
        dd 0x0d70dd2ee, 0x04e048354, 0x03903b3c2, 0x0a7672661, 0x0d06016f7
        dd 0x04969474d, 0x03e6e77db, 0x0aed16a4a, 0x0d9d65adc, 0x040df0b66
        dd 0x037d83bf0, 0x0a9bcae53, 0x0debb9ec5, 0x047b2cf7f, 0x030b5ffe9
        dd 0x0bdbdf21c, 0x0cabac28a, 0x053b39330, 0x024b4a3a6, 0x0bad03605
        dd 0x0cdd70693, 0x054de5729, 0x023d967bf, 0x0b3667a2e, 0x0c4614ab8
        dd 0x05d681b02, 0x02a6f2b94, 0x0b40bbe37, 0x0c30c8ea1, 0x05a05df1b
        dd 0x02d02ef8d


buff         rb MAX_PATH*2
copied       db 'Hash copied to Clipboard',0
blanc        db '',0
hMem         dd ?

;---------------------------------------------

section '.code' code readable executable

  start:
        invoke  GetModuleHandle,0
        invoke  DialogBoxParam,eax,37,HWND_DESKTOP,DialogProc,0
        invoke  ExitProcess,0

;---------------------------------------------

proc DialogProc hwnddlg,msg,wparam,lparam
        push    ebx esi edi
        cmp     [msg],WM_INITDIALOG
        je      .wminitdialog
        cmp     [msg],WM_COMMAND
        je      .wmcommand
        cmp     [msg],WM_CLOSE
        je      .wmclose
        xor     eax,eax
        jmp     .finish
        .wminitdialog:
        jmp     .wmencode
        .wmcommand:
        cmp     [wparam],BN_CLICKED shl 16 + IDCANCEL
        je      .wmclose
        cmp     [wparam],BN_CLICKED shl 16 + ID_COPYB
        je      .wmcopy2clipboard
        cmp     [wparam],EN_CHANGE shl 16 + ID_TXT
        je      .wmencode
        jmp     .processed
        .wmencode:
        invoke  GetDlgItemText,[hwnddlg],ID_TXT,buff,MAX_PATH
        invoke  lstrlen,buff
        test    eax, eax
        jnz     @f

        invoke  SetDlgItemText,[hwnddlg],ID_CRC,NULL
        jmp     .processed
        @@:
        stdcall crc32proc,buff,eax
        invoke  wsprintf,buff,mask,eax
        add     esp,12
        invoke  SetDlgItemText,[hwnddlg],ID_CRC,buff
        invoke  SetDlgItemText,[hwnddlg],ID_COPY, blanc
        jmp     .processed
        .wmcopy2clipboard:

        invoke  lstrlen,buff
        mov ebx, eax
        add ebx, 1

    invoke OpenClipboard,0
    invoke GlobalAlloc,GMEM_MOVEABLE or GMEM_ZEROINIT,ebx
    mov [hMem],eax
    invoke GlobalLock,[hMem]
    invoke lstrcpy,eax,buff
    invoke EmptyClipboard
    invoke GlobalUnlock,[hMem]

    invoke SetClipboardData,CF_TEXT,[hMem]
    invoke CloseClipboard



        invoke  SetDlgItemText,[hwnddlg],ID_COPY, copied
        jmp     .processed
        .wmclose:
        invoke  EndDialog,[hwnddlg],0
        .processed:
        mov     eax,1
        .finish:
        pop     edi esi ebx
        ret
endp

mask db '%8.X',0


proc crc32proc lData:dword, dLen:dword
        push    ebx ecx edx esi

        mov     edx,0xFFFFFFFF
        mov     esi,[lData]
        mov     ecx,[dLen]

        xor     eax,eax
        crc32loop:
        lodsb
        mov     ebx,edx
        and     ebx,0x0FF
        xor     bl,al
        shl     ebx,2

        shr     edx,8
        and     edx,0x0FFFFFF
        xor     edx,[crc32table+ebx]

        loop    crc32loop

        mov     eax,edx
        xor     eax,0xFFFFFFFF

        pop     esi edx ecx ebx
        ret
endp



;---------------------------------------------

section '.idata' import data readable writeable

  library kernel32,'KERNEL32.DLL',\
          user32,'USER32.DLL'

  include 'api\kernel32.inc'
  include 'api\user32.inc'

;---------------------------------------------

section '.rsrc' resource data readable

  directory RT_DIALOG,dialogs

  resource dialogs,\
           37,LANG_ENGLISH+SUBLANG_DEFAULT,demonstration

  ID_TXT = 100
  ID_CRC = 101
  ID_COPYB = 102
  ID_COPY = 103

  dialog demonstration,'CRC32. Otus Reverse Engineering, Task 1-6',0,0,190,55,WS_CAPTION+WS_SYSMENU+DS_CENTER+DS_SYSMODAL
    dialogitem 'BUTTON','',-1, 2, -1, 185, 35,WS_VISIBLE+BS_GROUPBOX
    dialogitem 'EDIT','0123456789', ID_TXT,5,7,177,13,WS_VISIBLE+WS_BORDER+WS_TABSTOP+ES_AUTOHSCROLL
    dialogitem 'STATIC','',ID_CRC,6,23,100,10,WS_VISIBLE
    dialogitem 'BUTTON','Copy',ID_COPYB,135,19,50,13,WS_VISIBLE+WS_TABSTOP+BS_PUSHBUTTON
    dialogitem 'STATIC','',ID_COPY,6,39,100,10,WS_VISIBLE
    dialogitem 'BUTTON','Exit',IDCANCEL,135,37,50,15,WS_VISIBLE+WS_TABSTOP+BS_PUSHBUTTON
  enddialog