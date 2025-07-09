#ifndef INPUT_HANDLER_HPP
#define INPUT_HANDLER_HPP



enum EditorKey : int {
    K_LEFT = 0,
    K_RIGHT,
    K_UP,
    K_DOWN,
    K_CHANGE_MODE,
    
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
   
    void handle_all_mode      (Editor* bite, Buffer* buf, int input);
    void handle_edit_mode     (Editor* bite, Buffer* buf, int input);
    void handle_select_mode   (Editor* bite, Buffer* buf, int input);


}





#endif
