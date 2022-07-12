#!/bin/ksh
set -euo pipefail

## Corrects repl asignment by running unit tests. All questions but the one on
## sub shells are corrected here. We have no way to specify the behaviour of a
## subshell since we do not handle variables, nor do we have the `exec` builtin
## to replace the code.

archive="$1"
tmpd="$(mktemp -d)"

success() {
    question="$1"
    printf '[\e[32mOK\e[0m] %s\n' "$question"
}
failure () {
    question="$1"; output="$2"; err="$3"
    # Write a file to mention there has been a failure
    touch 'failure'
    printf '[\e[31mKO\e[0m] %s\n' "$question"
    printf '\e[33mError log:\e[0m %s\n' "$(cat "$err")"
    printf '\e[33mResult   :\e[0m %s\n' "$output"
}
patch_sys_stat () {
    # Linux defines 'S_IRUSR' in fcntl.h while BSD defines it in 'sys/stat.h'
    # so we include sys/stat.h
    # NOTE: BSD sed does not accept '\n', so we use literal newlines
    if ! grep -q '^#include <sys/stat.h>' main.c; then
        sed -i '1i\
#include <sys/stat.h>\
' main.c
    fi
}
keep_files() {
    printf 'Content kept in %s\n' "$tmpd"
    # Beautify for inspection
    (
        cd "$tmpd" || exit 1
        print "Beautifying with astyle"
        for f in *.c; do
            mv "$f" "${f}.orig"
            astyle < "$f.orig" > "$f"
        done
    )
}
trap keep_files EXIT

# Unpack student's files
if [ -d "$archive" ]; then
    tar xzf shell-bootstrap.tar.gz -C "$tmpd"
    for f in ${archive}/*.c ${archive}/*.patch ${archive}/*.h; do
        [ -r "$f" ] && cp "$f" "$tmpd"
    done;
else
    tar xzf "$archive" -C "$tmpd"
fi
(
    cd "$tmpd" || exit 1
    patch_sys_stat
    for f in *.patch; do
        [ -r "$f" ] && patch "${f%.patch}" "$f"
    done

    if make CC='gcc -fPIC -std=gnu99' > /dev/null; then
        success 'compiling'
    else
        failure 'compiling' '' ''
        exit 1
    fi

    ## 1. Execution of external command
    print '## Q1. External command ##'
    out="$(echo 'echo "q1 external"' | ./shell 2> 1.err)"
    if echo "$out" | grep -q 'q1 external' && [ ! -s 1.err ]; then
        success 'external call'
    else
        failure 'external call' "$out" 1.err
    fi

    ## 2. Sequence operator ;
    ## Verify that even though a command fails, other are still performed
    ## and that the error is reported.
    print '## Q2. Sequence operator ##'
    out="$(echo 'false ; echo still' | ./shell 2> 2a.err)"
    if echo "$out" | grep -q still && [ ! -s 2a.err ]; then
        success 'sequence false'
    else
        failure 'sequence false' "$out" 2a.err
    fi

    out="$(echo 'true ; echo still' | ./shell 2> 2b.err)"
    if echo "$out" | grep -q still && [ ! -s 2b.err ]; then
        success 'sequence true'
    else
        failure 'sequence true' "$out" 2b.err
    fi

    ## 3. And and Or
    print '## Q3. And, Or ##'
    out="$(echo 'false && echo still' | ./shell 2> 3a.err)"
    if ! echo "$out" | grep -q 'still' && [ ! -s 3a.err ]; then
        success 'and false'
    else
        failure 'and false' "$out" 3a.err
    fi

    out="$(echo 'true && echo still' | ./shell 2> 3b.err)"
    if echo "$out" | grep -q 'still' && [ ! -s 3b.err ]; then
        success 'and true'
    else
        failure 'and true' "$out" 3b.err
    fi

    out="$(echo 'false || echo still' | ./shell 2> 3c.err)"
    if echo "$out" | grep -q 'still' && [ ! -s 3c.err ]; then
        success 'or false'
    else
        failure 'or false' "$out" 3c.err
    fi

    ## In that case, rhs of || is not executed
    out="$(echo 'true || echo still' | ./shell 2> 3d.err)"
    if ! echo "$out" | grep -q still && [ ! -s 3d.err ]; then
        success 'or true'
    else
        failure 'or' "$out" 3d.err
    fi

    ## 4. Sub shell and redirect
    ## Use exec in a sub shell
    print '## Q4. Sub shell ##'
    if (echo '( true )' | ./shell) > /dev/null; then
        success 'sub shell'
    else
        failure 'sub shell' '' ''
    fi

    out="$( ( ( echo '( ls ) > q4.log' ) | ./shell ) 2> 4b.err > /dev/null )"
    if [ -s q4.log ] && [ ! -s 4b.err ]; then
        success 'sub shell redirection'
    else
        failure 'sub shell redirection' "$out" 4b.err
    fi

    out="$( ( ( echo '( ls > q4_inner.log ) > q4_outer.log' | ./shell ) 2> 4c.err ) )"
    # Check that inner file is not empty and outer file is empty
    if [ -s q4_inner.log ] && [ ! -s q4_outer.log ] && [ ! -s 4c.err ]; then
        success 'sub shell inner redirection'
    else
        failure 'sub shell inner redirection' "$out" 4c.err
    fi

    ## 5. C-c interrupt
    print '## Q5. Interrupt ##'
    ./shell 2> 5.err >/dev/null &
    shid=$!
    kill -INT "$shid"
    if pgrep -q shell; then
        success 'interrupt'
    else
        failure 'interrupt' "" 5.err
    fi

    ## 6. Redirections
    print '## Q6. Redirection ##'
    out="$( echo 'echo still > q6.log' | ./shell 2> 6a.err)"
    if grep -q still q6.log && [ ! -s 6a.err ]; then
        success 'redirections stdout'
    else
        failure 'redirections stdout' "$out" 6a.err
    fi

    out="$( (echo 'cat -z 2> q6.err' | ./shell) 2> 6b.err)"
    if [ -s q6.err ] && [ ! -s 6b.err ]; then
        success 'redirections stderr'
    else
        failure 'redirections stderr' "$out" 6b.err
    fi

    out="$( echo 'cat < main.c' | ./shell 2> 6c.err )"
    if [ -n "$out" ] && [ ! -s 6c.err ]; then
        success 'redirection stdin'
    else
        failure 'redirection stdin' "$out" 6c.err
    fi

    out="$( printf 'find . > /dev/null\nfind .' | ./shell 2> 6d.err )"
    if echo "$out" | grep -q '\./shell' && [ ! -s 6d.err ]; then
        success 'reirection reset'
    else
        failure 'redirection reset' "$out" 6d.err
    fi

    ## 7. Piping
    print '## Q7. Piping ##'
    out="$( (echo 'find . | wc -w' | ./shell) 2> 7.err)"
    if echo "$out" | grep -q -E '[0-9]+'  && [ ! -s 7.err ]; then
        success 'piping'
    else
        failure 'piping' "$out" 7.err
    fi

    ## 8. Bonus
)

if [ -f "${tmpd}/failure" ]; then
    exit 0
else
    print 'No failure, cleaning up'
    rm -rf "${tmpd}"
fi
