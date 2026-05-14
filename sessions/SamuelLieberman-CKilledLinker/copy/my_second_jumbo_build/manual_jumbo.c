
#include <stdbool.h>

#include "conf.h"

// Useful macro utilities
#ifndef _STR
#   define _STR(text) #text
#endif
#ifndef XSTR
#   define XSTR(text) _STR(text)
#endif

#include "conf.h"

// Preprocessor #define expected by NAppGUI when build target uses GTK.
// See NAppGUI's src/sewer/arch.hxx
#ifndef __GTK3_TOOLKIT__
#   define __GTK3_TOOLKIT__
#endif

// ogl3d/glew.c and ogl3d/gtk/ogl3dimp.c both indirectly #include X11/Xlib.h,
// X11/Xlib.h #includes X11/X.h, and X11/X.h dumps a bunch of typedefs into
// the global namespace! Effectively X11 is squatting useful identifiers
// like Mask, Atom, Time, Window, Font, Cursor, and KeyCode. Dang it X11!
// We actually need some of those for NAppGUI! I re-#define them just for
// these two files to avoid naming conflicts.
#define Font X11_Font
#define Cursor X11_Cursor
#define Window X11_Window
#include XSTR(NAPPGUI_SRC/ogl3d/glew.c)
#include XSTR(NAPPGUI_SRC/ogl3d/gtk/ogl3dimp.c)
#undef Font
#undef Cursor
#undef Window
// Don't bother #including this files later
#define SKIP_GLEW_C
#define SKIP_OGL3DIMP_C

// In NAppGUI's /gui/items.c on line 45 is this line:
// DeclSt(PElem);
// DeclSt(<type>) is a NAppGUI macro that declares a dynamic array whose
// elements are the given type. Because PElem is a typedef'd multiple times,
// it needs to be #defined to PElem_items_c. gen_jumbo.c can understand
// and fix this much, but it doesn't understand the next issue
#define PElem PElem_items_c

// Unfortunately, while DeclSt is a multi-level macro, some other NAppGUI
// dynamic array macros only have a single level, preventing expansion.
// arrst_create() being an example. This means that for the declaration,
// PElem gets time to expand to PElem_items_c, but for the usage of the
// PElem dynamic array, no expansion occurs. The following is a little bit
// of manual work re-#defining these to the names they were declared with.
#define ArrStPElem ArrStPElem_items_c
#define arrst_PElem_destroy arrst_PElem_items_c_destroy
#define arrst_PElem_delete arrst_PElem_items_c_delete
#define arrst_PElem_clear arrst_PElem_items_c_clear
#define arrst_PElem_create arrst_PElem_items_c_create
#define arrst_PElem_size arrst_PElem_items_c_size
#define arrst_PElem_get arrst_PElem_items_c_get
#define arrst_PElem_get_const arrst_PElem_items_c_get_const
#define arrst_PElem_insert0 arrst_PElem_items_c_insert0

// This one is just good old name duplication of course though.
#define i_create i_create_items_c

// We are #including gui/items.c right here, so don't #include it later.
#define SKIP_ITEMS_C
#include XSTR(NAPPGUI_SRC/gui/items.c)

#undef ArrStPElem
#undef arrst_PElem_destroy
#undef arrst_PElem_delete
#undef arrst_PElem_clear
#undef PElem
#undef arrst_PElem_create
#undef arrst_PElem_size
#undef arrst_PElem_get
#undef arrst_PElem_get_const
#undef arrst_PElem_insert0
#undef i_create

// /gui/listbox.c has the same problem as gui/items.c above, but it's the
// only other file with this problem, so we don't bother #including it here.
#define ArrStPElem_listbox_c ArrStPElem
#define arrst_PElem_listbox_c_create arrst_PElem_create_listbox_c
#define arrst_PElem_listbox_c_size arrst_PElem_size_listbox_c
#define arrst_PElem_listbox_c_get arrst_PElem_get_listbox_c
#define arrst_PElem_listbox_c_insert0 arrst_PElem_insert0_listbox_c
#define arrst_PElem_listbox_c_all arrst_PElem_all_listbox_c
#define arrst_PElem_listbox_c_destroy arrst_PElem_destroy
#define arrst_PElem_listbox_c_delete arrst_PElem_delete
#define arrst_PElem_listbox_c_clear arrst_PElem_clear

// custom NAppGUI macros that screw with any path containing 'unix' or 'linux'
#undef unix
#undef linux

#include "autogen_jumbo.c"

// Build the demo
#define i_OnClose i_OnClose_hello_c
#include "hello.c"
#undef i_OnClose
