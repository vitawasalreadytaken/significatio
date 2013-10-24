CC = gcc
LIBS = gtk+-2.0 gdk-pixbuf-2.0 gdk-pixbuf-xlib-2.0 dbus-1 dbus-glib-1
CFLAGS = -Wall -L/usr/X11R6/lib `pkg-config --cflags $(LIBS)`
LDFLAGS = `pkg-config --libs $(LIBS)`

significatio: significatio.o actions.o
	$(CC) $(LDFLAGS) significatio.o actions.o -o significatio

significatio.o: significatio.c actions.h
	$(CC) $(CFLAGS) -c significatio.c -o significatio.o

actions.o: actions.c actions.h
	$(CC) $(CFLAGS) -c actions.c -o actions.o

clean:
	rm -rf *.o significatio

