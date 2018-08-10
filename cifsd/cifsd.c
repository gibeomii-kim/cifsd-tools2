/*
 *   Copyright (C) 2018 Samsung Electronics Co., Ltd.
 *
 *   linux-cifsd-devel@lists.sourceforge.net
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

#include <config_parser.h>
#include <cifsdtools.h>

#include <ipc.h>
#include <rpc.h>
#include <worker.h>
#include <management/user.h>
#include <management/share.h>
#include <management/session.h>
#include <management/tree_conn.h>

static void usage(void)
{
	fprintf(stderr, "cifsd-tools version : %s, date : %s\n",
			CIFSD_TOOLS_VERSION,
			CIFSD_TOOLS_DATE);
	fprintf(stderr, "Usage: cifsd\n");
	fprintf(stderr, "\t-c smb.conf | --config=smb.conf\n");
	fprintf(stderr, "\t-i cifspwd.db | --import-users=cifspwd.db\n");
	fprintf(stderr, "\t-v | --verbose\n");

	exit(EXIT_FAILURE);
}

static int parse_configs(char *pwddb, char *smbconf)
{
	int ret;

	ret = cp_parse_pwddb(pwddb);
	if (ret)
		return ret;

	ret = cp_parse_smbconf(smbconf);
	if (ret)
		return ret;

	if (pwddb != PATH_PWDDB)
		free(pwddb);
	if (smbconf!= PATH_SMBCONF)
		free(smbconf);
	return 0;
}

int main(int argc, char *argv[])
{
	int ret = EXIT_FAILURE;
	char *pwddb = PATH_PWDDB;
	char *smbconf = PATH_SMBCONF;
	int c;

	set_logger_app_name("cifsd");

	memset(&global_conf, 0x00, sizeof(struct smbconf_global));

	opterr = 0;
	while ((c = getopt(argc, argv, "c:i:vh")) != EOF)
		switch (c) {
		case 'c':
			smbconf = strdup(optarg);
			break;
		case 'i':
			pwddb = strdup(optarg);
			break;
		case 'v':
			break;
		case '?':
		case 'h':
		default:
			usage();
	}

	if (!smbconf || !pwddb) {
		pr_err("Out of memory\n");
		goto out;
	}

	ret = usm_init();
	if (ret)
		goto out;

	ret = shm_init();
	if (ret)
		goto out;

	ret = parse_configs(pwddb, smbconf);
	if (ret)
		goto out;

	ret = sm_init();
	if (ret)
		goto out;

	ret = wp_init();
	if (ret)
		goto out;

	ret = rpc_init();
	if (ret)
		goto out;

	ret = ipc_init();
	if (ret)
		goto out;

	ret = ipc_receive_loop();
out:
	/*
	 * NOTE, this is the final release, we don't look at ref_count
	 * values. User management should be destroyed last.
	 */
	ipc_destroy();
	rpc_destroy();
	wp_destroy();
	sm_destroy();
	shm_destroy();
	usm_destroy();
	return ret;
}
