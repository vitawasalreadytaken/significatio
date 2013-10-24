#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <glib-object.h>
#include <dbus/dbus.h>
#include <dbus/dbus-glib.h>


//#define DEBUG


DBusGProxy* proxy;

/**
 * Connect to the DBus session bus and set up a proxy for the
 * /org/freedesktop/compiz/decoration/allscreens/shadow_color object.
 */
void init() {
	DBusGConnection* connection;
	GError* error = NULL;
	g_type_init();

	connection = dbus_g_bus_get(DBUS_BUS_SESSION, &error);
	if (connection == NULL) {
		fprintf(stderr, "Failed to connect to bus: %s\n", error->message);
		g_error_free(error);
		exit(2);
	}

	proxy = dbus_g_proxy_new_for_name(connection, "org.freedesktop.compiz", "/org/freedesktop/compiz/decoration/allscreens/shadow_color", "org.freedesktop.compiz");
}

/**
 * Called whenever the keyboard group changes.
 */
void onGroupChange(int monitoredGroupSet) {
	GError* error = NULL;
	const char* color = (monitoredGroupSet ? "#c32c00ff" : "#000000ff");

	#ifdef DEBUG
		fprintf(stderr, "Setting shadow color to %s\n", color);
	#endif

	if (!dbus_g_proxy_call(proxy, "set", &error, G_TYPE_STRING, color, G_TYPE_INVALID, G_TYPE_INVALID)) {
		fprintf(stderr, "Failed to call set(): %s\n", error->message);
		g_error_free(error);
		exit(3);
	}
}

