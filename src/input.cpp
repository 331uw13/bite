#include "input.hpp"
#include "editor.hpp"

#include <ncurses.h> // TEMPORARY HERE!




void InputHandler::handle_all_mode(Editor* bite, Buffer* buf, int input) {

    if(input <= 0) {
        return;
    }

    // TODO: Abstract this key input handler.
    //
    // TODO: Make function to map keys to input that bite editor can understand.
    //
    //       "add_keymap(raw_input_num, KEY_MOVE_LEFT)"

    //printf("%i\n", input);



    KeyCommand kc = bite->keymap[buf->mode][input];
    bite->last_key_input = kc.test;
    

    switch(input) {
        
        case KEY_END:
            bite->running = false;
            break;

        case KEY_LEFT:
            buf->mov_cursor(-1, 0);
            break;

        case KEY_RIGHT:
            buf->mov_cursor(1, 0);
            break;

        case KEY_UP:
            buf->mov_cursor(0, -1);
            break;

        case KEY_DOWN:
            buf->mov_cursor(0, 1);
            break;


    // Arrow keys + Left Shift.
    
        case KEY_SLEFT:
            break;
            
        case KEY_SRIGHT:
            break;
        
        case KEY_SR: // Shift + Up.
            break;
        
        case KEY_SF: // Shift + Down.
            break;


        case KEY_BACKSPACE:
            buf->handle_backspace();
            break;

        case '\t':
            if(buf->add_tabs(buf->cursor, 1)) {
                buf->mov_cursor(buf->tab_width, 0);
            }
            break;

        case 0x0A:
            buf->handle_enter();
            break;
        
        default:
            if(buf->add_char(buf->cursor, input)) {
                buf->mov_cursor(1, 0);
            }
            break;

    }
}


void InputHandler::handle_edit_mode(Editor* bite, Buffer* buf, int input) {


}


void InputHandler::handle_select_mode(Editor* bite, Buffer* buf, int input) {


}



