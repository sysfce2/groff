#!/bin/sh
# Copyright (C) 2014-2024 Free Software Foundation, Inc.
#
# This file is part of groff.
# 
# groff is free software; you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, either version 2 of the License (GPL2).
# 
# groff is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# for more details.
# 
# The GPL2 license text is available in the internet at
# <http://www.gnu.org/licenses/gpl-2.0.txt>.

# Provision of this shell script should not be taken to imply that use of
# GNU eqn with groff -Tascii|-Tlatin1|-Tutf8 is supported.

@GROFF_BIN_PATH_SETUP@
PATH="$GROFF_RUNTIME$PATH"
export PATH
exec @g@eqn -Tascii ${1+"$@"}

# eof
