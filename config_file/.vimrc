source ~/.vim/bundle/.vundlerc

" 设置backspace为增强模式
set backspace=indent,eol,start

" 使用鼠标
set mouse=a
set nu
set ruler

" 高亮相关
set t_Co=256
set background=dark
syntax on 

set encoding=utf-8
set clipboard^=unnamed

" 缩进相关
set autoindent
set smartindent
set tabstop=4
set shiftwidth=4
set expandtab
set softtabstop=4

" 搜索高亮
set hlsearch

" 行首行尾
nmap <C-a> ^
nmap <C-e> $

" 删除光标前后所有字符
nmap <C-u> d^
nmap <C-k> d$

" 切换buf
nmap ,n :bn<CR>
nmap ,p :bp<CR>
nmap ,d :bwipe<CR>

"
nmap fw  :w<CR>
nmap fq  :q<CR>
nmap fwq    :wq<CR>

" colorscheme
colorscheme desert

" 恢复vim上次打开位置
if has("autocmd")
    au BufReadPost * if line("'\"") > 1 && line("'\"") <= line("$") | exe "normal! g'\"" | endif
endif

" GoImports
autocmd BufWritePre *.go :GoImports

" YouCompleteMe
let g:ycm_python_binary_path = '/home/liutun/repos/toutiao/runtime/bin/python'
let g:ycm_show_diagnostics_ui = 0
let g:ycm_complete_in_comments = 1
nmap gd :YcmCompleter GoToDefinitionElseDeclaration<CR>

" syntastic
set statusline+=%#warningmsg#
set statusline+=%{SyntasticStatuslineFlag()}
set statusline+=%*
let g:syntastic_python_checkers = ['flake8']
let g:syntastic_go_checkers = ['go']
let g:syntastic_always_populate_loc_list = 0
let g:syntastic_auto_loc_list = 0
let g:syntastic_check_on_open = 0
let g:syntastic_check_on_wq = 0
let g:syntastic_aggregate_errors = 1
command! Sgo let g:syntastic_go_checkers = ['go']
command! NoSgo let g:syntastic_go_checkers = []

" airline
set laststatus=2
let g:airline_powerline_fonts = 1
let g:airline#extensions#tabline#enabled = 1

" auto complete
inoremap ' ''<ESC>i
inoremap " ""<ESC>i
inoremap ( ()<ESC>i
inoremap [ []<ESC>i
inoremap { {<CR>}<ESC>O
