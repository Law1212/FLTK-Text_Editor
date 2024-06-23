#include <string.h>
#include <stdio.h>
#include <cstdlib>

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Text_Editor.H>
#include <FL/Fl_Menu_Item.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/fl_ask.H>
#include <FL/Fl_File_Chooser.H>

#include "TextEditor.h"

using namespace std;

int changed = 0;
char filename[256] = "";
Fl_Text_Buffer* textbuf = 0;
char title[] = "Untitled";
int loading;

EditorWindow::EditorWindow(int w, int h, const char* t)
    : Fl_Double_Window(w, h, t) {
    // Initialize window components, etc.
    replace_dlg = new Fl_Window(300, 105, "Replace");
    replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
    replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
    replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
    replace_next = new Fl_Return_Button(105, 70, 90, 25, "Replace Next");
    replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");

    editor = new Fl_Text_Editor(10, 30, w - 20, h - 40);
    editor->buffer(textbuf);
}
EditorWindow::~EditorWindow() {
    delete replace_dlg;
    delete replace_find;
    delete replace_with;
    delete replace_all;
    delete replace_next;
    delete replace_cancel;
    delete editor;
}
static void build_Menu(Fl_Menu_Bar* menu, Fl_Window* window)
{

    Fl_Menu_Item menuitems[] = {

        {"&File",               0, 0, 0, FL_SUBMENU},
            {"&New File",       0, (Fl_Callback*)new_cb },
            {"&Open File...",   FL_CTRL + 'o', (Fl_Callback*)open_cb },

            {"&Save File",      FL_CTRL + 's', (Fl_Callback*)save_cb},
            {"Save File &As...",FL_CTRL + FL_SHIFT + 's', (Fl_Callback*)saveas_cb, 0, FL_MENU_DIVIDER},

            {"E&xit",           FL_CTRL + 'q', (Fl_Callback*)quit_cb, 0},
            {0},
        {"&Edit",               0, 0, 0, FL_SUBMENU},
            {"&Undo",           FL_CTRL + 'z', (Fl_Callback*)undo_cb, 0, FL_MENU_DIVIDER},
            {"Cu&t",            FL_CTRL + 'x', (Fl_Callback*)cut_cb },
            {"&Copy",           FL_CTRL + 'c', (Fl_Callback*)copy_cb },
            {"&Paste",          FL_CTRL + 'v', (Fl_Callback*)paste_cb },
            {"&Delete",         0, (Fl_Callback*)delete_cb},
            {0},
        {"&Search",             0, 0, 0, FL_SUBMENU},
            {"&Find...",        FL_CTRL + 'f', (Fl_Callback*)find_cb },
            {"F&ind Again",     FL_CTRL + 'g', find2_cb},
            {"&Replace...",     FL_CTRL + 'r', replace_cb },
            {"&Re&place Again", FL_CTRL + 't', replace2_cb },
            {0},
        {0}

    };
    Fl_Menu_Bar* m = new Fl_Menu_Bar(0, 0, 640, 30);
    m->copy(menuitems);
}

Fl_Window* replace_dlg = new Fl_Window(300, 105, "Replace");
Fl_Input* replace_find = new Fl_Input(70, 10, 200, 25, "Find:");
Fl_Input* replace_with = new Fl_Input(70, 40, 200, 25, "Replace:");
Fl_Button* replace_all = new Fl_Button(10, 70, 90, 25, "Replace All");
Fl_Button* replace_next = new Fl_Button(105, 70, 90, 25, "Replace Next");
Fl_Button* replace_cancel = new Fl_Button(230, 70, 60, 25, "Cancel");

void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v) {
    if ((nInserted || nDeleted) && !loading) changed = 1;
    EditorWindow* w = (EditorWindow*)v;
    set_title(w);
    if (loading) w->editor->show_insert_position();
}
void copy_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_copy(0, e->editor);

}
void cut_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_cut(0, e->editor);

}
void delete_cb(Fl_Widget*, void* v) {
    textbuf->remove_selection();
}
void find_cb(Fl_Widget* w, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    const char* val;

    val = fl_input("Search String:", e->search);
    if (val) {

        strcpy_s(e->search, val);
        find2_cb(w, v);
    }
}
void find2_cb(Fl_Widget* w, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    if (e->search[0] == '\0') {

        find_cb(w, v);
        return;
    }

    int pos = e->editor->insert_position();
    int found = textbuf->search_forward(pos, e->search, &pos);
    if (found) {

        textbuf->select(pos, pos + strlen(e->search));
        e->editor->insert_position(pos + strlen(e->search));
        e->editor->show_insert_position();
    }
    else fl_alert("No occurrences of \'%s\' found!", e->search);
}
void new_cb(Fl_Widget*, void*) {
    if (!check_save()) return;

    filename[0] = '\0';
    textbuf->select(0, textbuf->length());
    textbuf->remove_selection();
    changed = 0;
    textbuf->call_modify_callbacks();
}
void open_cb(Fl_Widget*, void*) {
    if (!check_save()) return;

    char* newfile = fl_file_chooser("Open File?", "*", filename);
    if (newfile != NULL) load_file(newfile, -1);
}
void undo_cb(void)
{
    textbuf->undo();
}
void paste_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    Fl_Text_Editor::kf_paste(0, e->editor);
}
void quit_cb(Fl_Widget*, void*) {
    if (changed && !check_save())
        return;
    exit(0);
}
void replace_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    e->replace_dlg->show();
}
void replace2_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    const char* find = e->replace_find->value();
    const char* replace = e->replace_with->value();

    if (find[0] == '\0') {

        e->replace_dlg->show();
        return;
    }

    e->replace_dlg->hide();

    int pos = e->editor->insert_position();
    int found = textbuf->search_forward(pos, find, &pos);

    if (found) {

        textbuf->select(pos, pos + strlen(find));
        textbuf->remove_selection();
        textbuf->insert(pos, replace);
        textbuf->select(pos, pos + strlen(replace));
        e->editor->insert_position((pos + strlen(replace)));
        e->editor->show_insert_position();
    }
    else {
        fl_alert("No occurrences of \'%s\' found!", find);
    }

}
void replacall_cb(Fl_Widget*, void* v)
 {
    EditorWindow* e = (EditorWindow*)v;
    const char* find = e->replace_find->value();
    const char* replace = e->replace_with->value();

    if (*find == '\0') {

        e->replace_dlg->show();
        return;
    }

    e->replace_dlg->hide();

    e->editor->insert_position(0);
    int times = 0;

    for (int found = 1; found;) {
        int pos = e->editor->insert_position();
        found = textbuf->search_forward(pos, find, &pos);

        if (found) {

            textbuf->select(pos, pos + strlen(find));
            textbuf->remove_selection();
            textbuf->insert(pos, replace);

            e->editor->insert_position(pos + strlen(replace));
            e->editor->show_insert_position();

            times++;
        }
    }

    if (times) fl_message("Replaced %d occurrences", times);
    else fl_alert("No occurrences of \'%s\' found!", find);


}
void replcan_cb(Fl_Widget*, void* v) {
    EditorWindow* e = (EditorWindow*)v;
    e->replace_dlg->hide();
}
void save_cb(void) {

    if (filename[0] == '\0') {
        saveas_cb();
        return;
    }
    else save_file(filename);

}
void saveas_cb(void) {
    char* newfile;

    newfile = fl_file_chooser("Save File As?", "*", filename);
    if (newfile != NULL) save_file(newfile);
}
int check_save(void) {
    if (changed)  return 1;

    int r = fl_choice("Do you want to save your changes?", "Cancel", "Save", "Discard");

    if (r == 1) {
        save_cb();
        return !changed;
    }

    return (r == 2) ? 1 : 0;
}
void load_file(char* newfile, int ipos) {
    loading = 1;
    int insert = (ipos != -1);

    changed = insert;
    if (!insert) strcpy_s(filename, " ");

    int r;
    if (!insert) r = textbuf->insertfile(newfile, ipos);
    else r = textbuf->insertfile(newfile, ipos);

    if (r) {
        char err_buffer[256];  // Buffer to hold the error message
        strerror_s(err_buffer, sizeof(err_buffer), errno);  // Get the error message
        fl_alert("Error reading from the file \'%s\':\n%s.", newfile, err_buffer);
    }
        
    else
        if (!insert) strcpy_s(filename, newfile);

    loading = 0;
    textbuf->call_modify_callbacks();
}
void save_file(char* newfile) {

    if (textbuf->savefile(newfile)) {
        char err_buffer[256];  // Buffer to hold the error message
        strerror_s(err_buffer, sizeof(err_buffer), errno);  // Get the error message
        fl_alert("Error reading from the file \'%s\':\n%s.", newfile, err_buffer);
    }

    else
        strcpy_s(filename, newfile);
    changed = 0;
    textbuf->call_modify_callbacks();
}
void set_title(Fl_Window* w) {
    if (filename[0] == '\0') strcpy_s(title, "Untitled");
    else {
        char* slash;
        slash = strrchr(filename, '/');
#ifdef WIN32
        if (slash == NULL) slash = strrchr(filename, '\\');
#endif
        if (slash != NULL) strcpy_s(title, slash + 1);
        else strcpy_s(title, filename);
    }

    if (changed) strcat_s(title, " （modified）");

    w->label(title);
}

Fl_Window* new_view() {
    EditorWindow* window = new EditorWindow(800, 600, title);

    window->begin();

    window->editor = new Fl_Text_Editor(10, 30, 780, 560);
    window->editor->buffer(textbuf);

    Fl_Menu_Bar* menuBar = new Fl_Menu_Bar(0, 0, 800, 30);
    build_Menu(menuBar, window);

    window->end();
    window->resizable(window->editor);
    window->editor->linenumber_width(60);

    textbuf->add_modify_callback(changed_cb, window);
    textbuf->call_modify_callbacks();
    return window;
}

int main(int argc, char** argv) {
    textbuf = new Fl_Text_Buffer;
    Fl_Window* window = new_view();
    window->show(1, argv);
    if (argc > 1) load_file(argv[1], -1);
    return Fl::run();
}