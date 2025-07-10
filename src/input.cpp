#include "input.hpp"
#include "editor.hpp"

#include "util.hpp"


void InputHandler::handle_shared(Editor* bite, Buffer* buf, int input) {

    if(input <= 0) {
        return;
    }

    if(input == KEY_END) {
        bite->running = false;
        return;
    }

    KeyCommand kc = bite->keymap[BufferMode::SHARED][input];


    switch(kc.editor_key) {
        
        case EditorKey::K_LEFT:
            buf->mov_cursor(-1, 0);
            break;
        
        case EditorKey::K_RIGHT:
            buf->mov_cursor(1, 0);
            break;

        case EditorKey::K_UP:
            buf->mov_cursor(0, -1);
            break;
        
        case EditorKey::K_DOWN:
            buf->mov_cursor(0, 1);
            break;
       
        case EditorKey::K_NULLMODE:
            buf->mode = BufferMode::NULLMODE;
            break;
        
        case EditorKey::K_INSERTMODE:
            buf->mode = BufferMode::INSERT;
            break;
        
        case EditorKey::K_SELECTMODE:
            buf->mode = BufferMode::SELECT;
            break;

        default:
            break;
    }

}


void InputHandler::handle_null_mode(Editor* bite, Buffer* buf, int input) {


}

void InputHandler::handle_insert_mode(Editor* bite, Buffer* buf, int input) {
    if(input <= 0) {
        return;
    }
    KeyCommand kc = bite->keymap[buf->mode][input];

    if(!kc.editor_key) {
        if(buf->add_char(buf->cursor, input)) {
            buf->mov_cursor(1, 0);
        }
        return;
    }

    switch(kc.editor_key) {
     
        case EditorKey::K_ENTER:
            buf->handle_enter();
            break;
        
        case EditorKey::K_BACKSPACE:
            buf->handle_backspace();
            break;

        case EditorKey::K_TAB:
            buf->add_tabs(buf->cursor, 1);
            buf->mov_cursor(buf->tab_width, 0);
            break;
    }
}


void InputHandler::handle_select_mode(Editor* bite, Buffer* buf, int input) {


}



