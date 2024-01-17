#!/bin/sh
#
# Copyright (C) 2024 Free Software Foundation, Inc.
#
# This file is part of groff.
#
# groff is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# groff is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#

groff="${abs_top_builddir:-.}/test-groff"

fail=

wail () {
  echo ...FAILED >&2
  fail=YES
}

for device in ascii cp1047 dvi html xhtml latin1 lbp lj4 pdf ps utf8 \
              X75 X75-12 X100 X100-12
do
  echo "checking early backslash-? escape on $device device" >&2
  output=$(printf '\\!cA\n' "$input" | "$groff" -T $device -Z) || wail
  echo "$output"
  echo "checking that leader starts on line 1 for $device device" >&2
  echo "$output" | sed -n '1p' | grep -Eq '^x *T' || wail
  echo "checking that expected output is present for $device device" >&2
  echo "$output" | grep -Eq 'c *A' || wail
done

test -z "$fail"

# vim:set autoindent expandtab shiftwidth=2 tabstop=2 textwidth=72:
