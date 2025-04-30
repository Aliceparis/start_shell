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
1. token中的单双引号是否保留
2. test 是最新的版本，已经去除了tshell这个global
3. exit时free的问题，大部分的问题来自于env还有token以及parse
4. 当输入的commande没办法找到时，free的问题
5. 在echo 后面如果有没有闭合的情况下，现在是直接退出program，或许直接返回到pompt会更好？（返回到pompt的话，那么terminos的设置问题需要处理）
6. $?值返回的设置在expansion那一部分还没有加进去



**ft_fprintf OK**


**Les commades qui marche**:
- echo
- exit
- pwd
- env
- ls
