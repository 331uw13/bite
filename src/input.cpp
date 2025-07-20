#include "input.hpp"
#include "editor.hpp"
#include "command.hpp"
#include "util.hpp"


void InputHandler::handle_shared(Editor* bite, Buffer* buf, int input, bool can_move) {

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
            if(can_move) { buf->mov_cursor(-1, 0); }
            break;
        
        case EditorKey::K_RIGHT:
            if(can_move) { buf->mov_cursor(1, 0); }
            break;

        case EditorKey::K_UP:
            if(can_move) { buf->mov_cursor(0, -1); }
            break;
        
        case EditorKey::K_DOWN:
            if(can_move) { buf->mov_cursor(0, 1); }
            break;
       
        case EditorKey::K_NULLMODE:
            buf->set_mode(BufferMode::NULLMODE);
            break;
        
        case EditorKey::K_INSERTMODE:
            buf->set_mode(BufferMode::INSERT);
            break;
        
        case EditorKey::K_SELECTMODE:
            buf->set_mode(BufferMode::SELECT);
            break;
        
        case EditorKey::K_COMMANDMODE:
            buf->set_mode(BufferMode::COMMAND_INPUT);
            break;

        default:
            break;
    }
} // END SHARED_MODE


void InputHandler::handle_null_mode(Editor* bite, Buffer* buf, int input) {
    if(input <= 0) {
        return;    
    }

    KeyCommand kc = bite->keymap[buf->get_mode()][input];

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

        case EditorKey::K_UPDATE_SCRN:
            move(0, 0);
            clrtobot();
            break;
    }

} // END NULL_MODE

void InputHandler::handle_insert_mode(Editor* bite, Buffer* buf, int input) {
    if(input <= 0) {
        return;
    }
    KeyCommand kc = bite->keymap[buf->get_mode()][input];

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

} // END INSERT_MODE


void InputHandler::handle_select_mode(Editor* bite, Buffer* buf, int input) {

} // END SELECT_MODE



void InputHandler::handle_command_input(Editor* bite, Buffer* buf, int input) {

    switch(input) {
        case KEY_LEFT:
            if(buf->cmd_cur_x > 0) {
                buf->cmd_cur_x--;
            }
            break;

        case KEY_RIGHT:
            if((size_t)buf->cmd_cur_x < buf->cmd.size()) {
                buf->cmd_cur_x++;
            }
            break;

        case KEY_BACKSPACE:
            if(buf->cmd_cur_x > 0) {
                buf->cmd.erase(buf->cmd_cur_x-1, 1);
                buf->cmd_cur_x--;
            }
            break;

        case 0x0A: /* Enter */
            Command::execute(bite, buf, buf->cmd);
            buf->cmd.clear();
            buf->cmd_cur_x = 0;
            break;

        default:
            if(buf->cmd.size() < CMDLINE_MAX && U::is_valid_char(input)) {
                buf->cmd.insert(buf->cmd_cur_x, 1, input);
                buf->cmd_cur_x++;
            }
            break;
    }

} // END COMMAND_INPUT



