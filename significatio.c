/*
 * significatio 0.1
 *
 * This kludge can do something whenever a certain keyboard group is set.
 * By default, it changes the color of Compiz window shadows. Make changes
 * to actions.c if you want a different form of notification.
 *
 * The number of the keyboard group is specified as the first (and only)
 * argument.
 *
 * Copyright (c) 2010-2013 Vita Smid <me@ze.phyr.us>
 * Licensed under the terms of The MIT License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <X11/XKBlib.h>
#include <gdk/gdkx.h>

//#define DEBUG

#include "actions.h"


int xkbEventType;

GdkFilterReturn filter(XEvent* xev, GdkEvent* event, gpointer groupId) {
	if (xev->type == xkbEventType) {
		XkbEvent *xkbev = (XkbEvent *) xev;
		if (xkbev->any.xkb_type == XkbStateNotify) {
			#ifdef DEBUG
				fprintf(stderr, "Group changed to %d\n", xkbev->state.group);
			#endif
			onGroupChange(xkbev->state.group == *(int*)groupId);
		}
		return GDK_FILTER_REMOVE;
	}
	return GDK_FILTER_CONTINUE;
}

/**
 * Validate a string argument and convert it to int. Unlike atoi,
 * convert() returns -1 on error.
 */
int s2i(char* arg) {
	if (strcmp(arg, "0") == 0) {
		return 0;
	}
	int i = atoi(arg);
	return (i > 0 ? i : -1);
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	int groupId;

	// Boring input validation. Ensure that the group id is a positive integer.
	if (argc != 2 || -1 == (groupId = s2i(argv[1]))) {
		fprintf(stderr, "Usage: %s <keyboard group id>\n", argv[0]);
		return 1;
	}

	#ifdef DEBUG
		fprintf(stderr, "Looking for group %d\n", groupId);
	#endif

	init();

	// Add the filter.
	Display* display = GDK_DISPLAY();
	int dummy;
	XkbQueryExtension(display, &dummy, &xkbEventType, &dummy, &dummy, &dummy);
	XkbSelectEventDetails(display, XkbUseCoreKbd, XkbStateNotify, XkbAllStateComponentsMask, XkbGroupStateMask);
	gdk_window_add_filter(NULL, (GdkFilterFunc) filter, &groupId);

	gtk_main();

	return 0;
}
