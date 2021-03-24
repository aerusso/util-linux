/*
 * lsfd(1) - list file descriptors
 *
 * Copyright (C) 2021 Red Hat, Inc. All rights reserved.
 * Written by Masatake YAMATO <yamato@redhat.com>
 *
 * Very generally based on lsof(8) by Victor A. Abell <abe@purdue.edu>
 * It supports multiple OSes. lsfd specializes to Linux.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "xalloc.h"
#include "nls.h"
#include "libsmartcols.h"

#include "lsfd.h"

static bool regular_fd_file_fill_column(struct proc *proc __attribute__((__unused__)),
					struct file *file __attribute__((__unused__)),
					struct libscols_line *ln,
					int column_id,
					size_t column_index)
{
	switch(column_id) {
	case COL_TYPE:
		if (scols_line_set_data(ln, column_index, "REG"))
			err(EXIT_FAILURE, _("failed to add output data"));
		return true;
	}

	return false;
}

const struct file_class regular_fd_file_class = {
	.super = &fd_file_class,
	.size = sizeof(struct fd_file),
	.fill_column = regular_fd_file_fill_column,
	.free_content = NULL,
};

struct file *make_regular_fd_file(const struct file_class *class,
				  struct stat *sb, const char *name, int fd)
{
	return make_fd_file(class? class: &regular_fd_file_class,
			    sb, name, fd);
}
