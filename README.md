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
1. token中quote没有闭合情况下的erro显示，其他部分没有问题
2. token中的单双引号是否保留
3. expansion 中,需要将ast的内容update到新的结构体中，现在这一步出现了问题，一旦解决，后面buildin暂时没有问题



**ft_fprintf OK**
