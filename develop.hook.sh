#!/usr/bin/env sh

## ================================|
# git-ci script for hardware (pio) |
### ===============================|

# Run by something like this:
# ==========================>
# while read oldrev newrev refname
# do
# 	branch=$(git rev-parse --symbolic --abbrev-ref $refname)
# 	if [ “develop” = “$branch” ]; then
# 		echo GIT_DIR $GIT_DIR
#  		project_root="${GIT_DIR}/.."
# 		echo project_root $project_root
#
# 		OLDREV=$oldrev \
#		NEWREV=$newrev \
#		PROJECT_ROOT=$project_root \
# 			$project_root/develop.hook.sh
# 	fi
# done

# this project runs on the "bb" hardware
SESSION="$(cat /tmp/bb_hardware)"
COMMAND='pio run -t upload -t monitor'

tmux kill-window -t ${SESSION}:${OLDREV} 
tmux new-window -d -t ${SESSION}:${NEWREV} -c ${PROJECT_ROOT}
tmux send-keys -t ${SESSION}:${NEWREV} "${COMMAND}" Enter

