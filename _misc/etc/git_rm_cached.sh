#!/bin/sh
git rm --cached `git ls-files --full-name -i --exclude-standard`
git clean -f -d -x
