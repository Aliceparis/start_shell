update list: 

Ajout dossier de pipex

**Builtins:**
- echo
- pwd
- env
- cd
- unset
- export
- exit

**Dispatch_cmd**
将我们的cmd 分成 builtin/简单cmd 和 pipe 然后execute

***************待解决的问题***********
1. echo der > out vs  echo "$USER" > out
3. echo 'der' < defg (un fichier n'existe pas)
4. echo 'der' > deaf(un fichier n'existe pas)
5. heredoc + ctrl c
6. ctrl d
7. 

**tout ok mais pb de free et norm**
cat main.c | cat ok
ls | grep.c ok 


新问题： 
- 我们的path删了 env还能用 但是bash里面是不能用的 我们是否让env用不了如果我们把path删了。其他的话我更新了一下environ cat ls 当path删了 现在就不能用了 跟bash是一样的。 

- 在main里加了if (!shell_program.ast-＞left)  Dispatch 就用不了了
比如我们写 echo ishs＞out 这个out的fichier就没有创造

- CRTL + C

Minishell> cat | cat | cat
cs
cs
加了  CRTL + C
Minishell> 

Minishell> Minishell> Minishell> 

bash是
bash-5.1$ cat | cat | cat
sc
sc
^C
bash-5.1$ 

- Minishell> /bin/ls

/bin/ls: cannot access '': No such file or directory
Minishell> /bin/ls -l
/bin/ls: cannot access '': No such file or directory
Minishell>\n
mais notre chemin absolue dans affcihe like ls les fichier......(test dans bash)


echo $? 运行后，显示出了结果，第二次在运行时，返回值应该为0
ctrlc 以后，echo $?值应该是130
cat < 1 -e ,实际上就是cat -e < 1
ls > text.txt ,head text.txt 显示错行
export A=$A
cd /home没问题，之后的echo $PWD没有更新path
echo “hello” > text.txt > text2.txt text2没有creer，按理hello出现在2中，text.txt中没有内容
在minishell中再次运行minishell出现了问题
unset后面很么也不加出现了segmentation的问题
unset fdear dear运行正确，exitstatus值为0,而非127
unset A B 之山除掉A



已解决的问题
- exit_status ok 因为我们init 0 然后在每个fonction最后又加了exit_status=0 所以他显示的一直是0 需要将所有的fonction试试.
- echo -nnnnnnnnnnnnnnn C est OKKKK
- 
