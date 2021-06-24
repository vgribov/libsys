" Local vim configuration

set cino=(0,Ws

nnoremap <silent><leader>mr :make! run<cr>

if @% == ""
    edit main.cpp
endif
