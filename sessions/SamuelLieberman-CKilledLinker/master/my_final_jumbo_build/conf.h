
#ifndef CONF_H
#define CONF_H

#ifdef __clang__
#pragma clang diagnostic ignored "-Weverything"
#endif

// Locations of NAppGUI and NAppGUI's dependencies.
#define NAPPGUI_H_DIR nappgui_src
#define CAIRO_H_DIR /usr/include/cairo
#define PANGO_H_DIR /usr/include/pango-1.0
#define HARFBUZZ_H_DIR /usr/include/harfbuzz
#define PIXBUF_H_DIR /usr/include/gdk-pixbuf-2.0
#define ATK_H_DIR /usr/include/atk-1.0
#define GTK3_H_DIR /usr/include/gtk-3.0
#define GLIB_H_DIR /usr/include/glib-2.0
#define GLIB_CONF_H_DIR /usr/lib/x86_64-linux-gnu/glib-2.0/include

// The directory where the NAppGUI's source code is.
#define NAPPGUI_SRC nappgui_src

#endif // CONF_H

