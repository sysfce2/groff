/* Copyright (C) 2015-2020 Free Software Foundation, Inc.

This file is part of groff.

groff is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation, either version 2 of the License, or
(at your option) any later version.

groff is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

The GNU General Public License version 2 (GPL2) is available in the
internet at <http://www.gnu.org/licenses/gpl-2.0.txt>. */

#include "lib.h"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "curtime.h"
#include "errarg.h"
#include "error.h"

struct tm *current_time()
{
#ifdef LONG_FOR_TIME_T
  long
#else
  time_t
#endif
    t;
  char *source_date_epoch = getenv("SOURCE_DATE_EPOCH");

  if (source_date_epoch) {
    errno = 0;
    char *endptr;
    long epoch = strtol(source_date_epoch, &endptr, 10);

    if (errno == ERANGE)
      fatal("$SOURCE_DATE_EPOCH: strtol: %1", strerror(errno));
    if (endptr == source_date_epoch)
      fatal("$SOURCE_DATE_EPOCH: no digits found: '%1'", endptr);
    if (*endptr != '\0')
      fatal("$SOURCE_DATE_EPOCH: trailing garbage: '%1'", endptr);
    t = epoch;
    return gmtime(&t);
  } else {
    t = time(0);
    return localtime(&t);
  }
}
