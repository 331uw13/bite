#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP



enum EditorKey : int {
    K_LEFT = 1,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_BACKSPACE,
    K_ENTER,
    K_TAB,
    
    K_NULLMODE,
    K_INSERTMODE,
    K_SELECTMODE,
    K_COMMANDMODE,

    K_UPDATE_SCRN,

    NUM_KEYS
};

struct KeyCommand {
    EditorKey editor_key;
    // Other things like commands can now be assigned to keys.
    // (Not implemented yet)
};


class Editor;
class Buffer;

namespace InputHandler {
   
    void handle_shared        (Editor* bite, Buffer* buf, int input, bool can_move);
    void handle_null_mode     (Editor* bite, Buffer* buf, int input);
    void handle_insert_mode   (Editor* bite, Buffer* buf, int input);
    void handle_select_mode   (Editor* bite, Buffer* buf, int input);
    void handle_command_input (Editor* bite, Buffer* buf, int input);

}





#endif
