#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

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
#include <cstdio>
#include <cstdlib>

// Forward declaration of callback functions
static void build_Menu(Fl_Menu_Bar* menu, Fl_Window* window);
void changed_cb(int, int nInserted, int nDeleted, int, const char*, void* v);
void copy_cb(Fl_Widget*, void* v);
void cut_cb(Fl_Widget*, void* v);
void delete_cb(Fl_Widget*, void* v);
void find_cb(Fl_Widget* w, void* v);
void find2_cb(Fl_Widget* w, void* v);
void new_cb(Fl_Widget*, void*);
void open_cb(Fl_Widget*, void*);
void undo_cb(void);
void paste_cb(Fl_Widget*, void* v);
void quit_cb(Fl_Widget*, void*);
void replace_cb(Fl_Widget*, void* v);
void replace2_cb(Fl_Widget*, void* v);
void replacall_cb(Fl_Widget*, void* v);
void replcan_cb(Fl_Widget*, void* v);
void save_cb(void);
void saveas_cb(void);
int check_save(void);
void load_file(char* newfile, int ipos);
void save_file(char* newfile);
void set_title(Fl_Window* w);
Fl_Window* new_view(); // Uncomment this line

class EditorWindow : public Fl_Double_Window {
public:
    EditorWindow(int w, int h, const char* t);
    ~EditorWindow();

    Fl_Window* replace_dlg;
    Fl_Input* replace_find;
    Fl_Input* replace_with;
    Fl_Button* replace_all;

    Fl_Return_Button* replace_next;
    Fl_Button* replace_cancel;

    Fl_Text_Editor* editor;
    char search[256];
};

#endif // TEXTEDITOR_H
