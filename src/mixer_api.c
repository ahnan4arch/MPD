/*
 * Copyright (C) 2003-2009 The Music Player Daemon Project
 * http://www.musicpd.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <assert.h>

#include <glib.h>

#include "mixer_api.h"

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "mixer"

static bool mixers_enabled = true;

void mixer_disable_all(void)
{
	g_debug("mixer api is disabled\n");
	mixers_enabled = false;
}

struct mixer *
mixer_new(const struct mixer_plugin *plugin, const struct config_param *param)
{
	struct mixer *mixer;

	//mixers are disabled (by using software volume)
	if (!mixers_enabled) {
		return NULL;
	}
	assert(plugin != NULL);

	mixer = plugin->init(param);

	assert(mixer == NULL || mixer->plugin == plugin);

	return mixer;
}

void
mixer_free(struct mixer *mixer)
{
	if (!mixer) {
		return;
	}
	assert(mixer->plugin != NULL);

	mixer->plugin->finish(mixer);
}

bool mixer_open(struct mixer *mixer)
{
	if (!mixer) {
		return false;
	}
	assert(mixer->plugin != NULL);
	return mixer->plugin->open(mixer);
}

void mixer_close(struct mixer *mixer)
{
	if (!mixer) {
		return;
	}
	assert(mixer->plugin != NULL);
	mixer->plugin->close(mixer);
}
