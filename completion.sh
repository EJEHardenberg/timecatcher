_tcBase() 
{
    local cur prev opts base flags
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    #
    #  The basic options we'll complete.
    #
    opts="start add-info finish view --help"
    flags="--help -h"

    if [[ ${prev} == "start" ]] ; then 
        _tcStart
        return 0
    fi

    if [[ ${prev} == "finish" ]] ; then 
        _tcFinish
        return 0
    fi

    if [[ ${prev} == "view" ]] ; then 
        _tcView
        return 0
    fi

    if [[ ${prev} == "add-info" ]] ; then 
        _addInfo
        return 0
    fi

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${flags}" -- ${cur}) )
        return 0
    fi

    if [[ ${prev} == "tcatch" ]] ; then 
        COMPREPLY=($(compgen -W "${opts}" -- ${cur}))  
    fi
    return 0
}
_tcStart()
{
    local cur prev opts base
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    opts="-s --switch -h --help"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    if [[ ${cur} == * ]] ; then
        local tasknames=$(for x in `cat ~/.tc/indexes/*.index | cut -d ' ' -f 2- | uniq | grep -v  [[:space:]]*8 | rev | cut -d ' ' -f 2- | rev`; do echo ${x} ; done )
        COMPREPLY=( $(compgen -W "${tasknames}" -- ${cur}) )
        return 0
    fi


    COMPREPLY=($(compgen -W "${opts}" -- ${cur}))  
    return 0
}
_tcFinish()
{
    local cur prev opts base
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    opts="-h --help"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    local tasknames=$(for x in `cat ~/.tc/indexes/*.index | cut -d ' ' -f 2- | uniq | grep -v [[:space:]]*32 | rev | cut -d ' ' -f 2- | rev`; do echo ${x} ; done )
    COMPREPLY=( $(compgen -W "${tasknames}" -- ${cur}) )
    return 0
    
    
}
_tcView()
{
    local cur prev opts base
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    opts="-v --verbose -h --help -a --all"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    if [[ ${cur} == * ]] ; then
        local tasknames=$(for x in `cat ~/.tc/indexes/*.index | cut -d ' ' -f 2- | uniq | rev | cut -d ' ' -f 2- | rev`; do echo ${x} ; done )
        COMPREPLY=( $(compgen -W "${tasknames}" -- ${cur}) )
        return 0
    fi


    COMPREPLY=($(compgen -W "${opts}" -- ${cur}))  
    return 0
}
_addInfo()
{
    local cur prev opts base
    COMPREPLY=()
    cur="${COMP_WORDS[COMP_CWORD]}"
    prev="${COMP_WORDS[COMP_CWORD-1]}"

    opts="-h --help"

    if [[ ${cur} == -* ]] ; then
        COMPREPLY=( $(compgen -W "${opts}" -- ${cur}) )
        return 0
    fi

    if [[ ${cur} == * ]] ; then
        local tasknames=$(for x in `cat ~/.tc/indexes/*.index | cut -d ' ' -f 2- | uniq | rev | cut -d ' ' -f 2- | rev`; do echo ${x} ; done )
        COMPREPLY=( $(compgen -W "${tasknames}" -- ${cur}) )
        return 0
    fi


    COMPREPLY=($(compgen -W "${opts}" -- ${cur}))  
    return 0
}

complete -F _tcBase tcatch


PATH=$PATH:~/Programming/C/timecatcher/