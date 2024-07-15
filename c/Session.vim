let SessionLoad = 1
let s:so_save = &g:so | let s:siso_save = &g:siso | setg so=0 siso=0 | setl so=-1 siso=-1
let v:this_session=expand("<sfile>:p")
silent only
silent tabonly
cd ~/learning/vulkan/c
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
let s:shortmess_save = &shortmess
if &shortmess =~ 'A'
  set shortmess=aoOA
else
  set shortmess=aoO
endif
badd +65 src/device_context.c
badd +1 lib/ily_types.h
badd +1 lib/ily_types.c
badd +1 term://~/learning/vulkan/c//18431:/usr/bin/bash
badd +0 src/main.c
argglobal
%argdel
$argadd src/device_context.c
edit lib/ily_types.h
let s:save_splitbelow = &splitbelow
let s:save_splitright = &splitright
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd w
wincmd w
let &splitbelow = s:save_splitbelow
let &splitright = s:save_splitright
wincmd t
let s:save_winminheight = &winminheight
let s:save_winminwidth = &winminwidth
set winminheight=0
set winheight=1
set winminwidth=0
set winwidth=1
exe '1resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 1resize ' . ((&columns * 186 + 140) / 280)
exe '2resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 2resize ' . ((&columns * 93 + 140) / 280)
exe '3resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 3resize ' . ((&columns * 92 + 140) / 280)
exe 'vert 4resize ' . ((&columns * 93 + 140) / 280)
argglobal
if bufexists(fnamemodify("term://~/learning/vulkan/c//18431:/usr/bin/bash", ":p")) | buffer term://~/learning/vulkan/c//18431:/usr/bin/bash | else | edit term://~/learning/vulkan/c//18431:/usr/bin/bash | endif
if &buftype ==# 'terminal'
  silent file term://~/learning/vulkan/c//18431:/usr/bin/bash
endif
balt lib/ily_types.c
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
let s:l = 1 - ((0 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
lcd ~/learning/vulkan/c
wincmd w
argglobal
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
4,11fold
13,16fold
let &fdl = &fdl
let s:l = 4 - ((3 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 4
normal! 0
lcd ~/learning/vulkan/c
wincmd w
argglobal
if bufexists(fnamemodify("~/learning/vulkan/c/lib/ily_types.h", ":p")) | buffer ~/learning/vulkan/c/lib/ily_types.h | else | edit ~/learning/vulkan/c/lib/ily_types.h | endif
if &buftype ==# 'terminal'
  silent file ~/learning/vulkan/c/lib/ily_types.h
endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
4,11fold
13,16fold
let &fdl = &fdl
let s:l = 4 - ((3 * winheight(0) + 17) / 34)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 4
normal! 02|
lcd ~/learning/vulkan/c
wincmd w
argglobal
if bufexists(fnamemodify("~/learning/vulkan/c/src/main.c", ":p")) | buffer ~/learning/vulkan/c/src/main.c | else | edit ~/learning/vulkan/c/src/main.c | endif
if &buftype ==# 'terminal'
  silent file ~/learning/vulkan/c/src/main.c
endif
setlocal fdm=manual
setlocal fde=0
setlocal fmr={{{,}}}
setlocal fdi=#
setlocal fdl=99
setlocal fml=1
setlocal fdn=20
setlocal fen
silent! normal! zE
21,28fold
33,35fold
32,36fold
38,41fold
43,51fold
54,65fold
let &fdl = &fdl
let s:l = 1 - ((0 * winheight(0) + 34) / 69)
if s:l < 1 | let s:l = 1 | endif
keepjumps exe s:l
normal! zt
keepjumps 1
normal! 0
lcd ~/learning/vulkan/c
wincmd w
4wincmd w
exe '1resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 1resize ' . ((&columns * 186 + 140) / 280)
exe '2resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 2resize ' . ((&columns * 93 + 140) / 280)
exe '3resize ' . ((&lines * 34 + 35) / 71)
exe 'vert 3resize ' . ((&columns * 92 + 140) / 280)
exe 'vert 4resize ' . ((&columns * 93 + 140) / 280)
tabnext 1
if exists('s:wipebuf') && len(win_findbuf(s:wipebuf)) == 0 && getbufvar(s:wipebuf, '&buftype') isnot# 'terminal'
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20
let &shortmess = s:shortmess_save
let &winminheight = s:save_winminheight
let &winminwidth = s:save_winminwidth
let s:sx = expand("<sfile>:p:r")."x.vim"
if filereadable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &g:so = s:so_save | let &g:siso = s:siso_save
nohlsearch
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
