#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

#include "editor.hpp"
#include "input.hpp"
#include "util.hpp"



void update_term_size(Editor* bite) {
    int twidth = 0;
    int theight = 0;
    getmaxyx(stdscr, theight, twidth);
    
    if((twidth == bite->term_width)
    && (theight == bite->term_height)) {
        return;
    }

    bite->term_width = twidth;
    bite->term_height = theight;

    bite->update_buffer_areas();
   
    move(0, 0);
    clrtobot();
}


void main_loop(Editor* bite) {
    while(bite->running) {

        update_term_size(bite);

        bite->buf->draw(bite);
        BufferMode bufmode = bite->buf->get_mode();

        const int input = getch();
        const bool shared_mode_can_move = (bufmode != BufferMode::COMMAND_INPUT);
        InputHandler::handle_shared(bite, bite->buf, input, shared_mode_can_move);
        
        switch(bufmode) {
            case BufferMode::NULLMODE:
                InputHandler::handle_null_mode(bite, bite->buf, input);
                break;
            
            case BufferMode::INSERT:
                InputHandler::handle_insert_mode(bite, bite->buf, input);
                break;
            
            case BufferMode::SELECT:
                InputHandler::handle_select_mode(bite, bite->buf, input);
                break;    

            case BufferMode::COMMAND_INPUT:
                InputHandler::handle_command_input(bite, bite->buf, input);
                break;
        }

        refresh();
    }
}


int main(int argc, char** argv) {
  
    assign_logfile("bite.log");

    // By default there is not unicode support with POSIX locale.
    // so it can be enabled with selecting the user locale setting.
    setlocale(LC_ALL, "");

    // Initialize ncurses.
    //
    initscr();
    raw();     // Do not generate any interupt signal.
    noecho();  // Do not print characters that user types. It will be handled differently.
    keypad(stdscr, 1); // Enable arrow, backspace keys etc..
    curs_set(0);       // Hide cursor.

    // Initialize the Editor.
    // It will keep track of all buffers.

    Editor bite;
    bite.init();
    bite.init_style();
    bite.map_input_keys();

    FileIO::read_to_buffer(bite.buf, "test_file.c");


    // Get the terminal width and height
    // and set initial buffer size and position.
    // First buffer is created by 'bite.init()'
    getmaxyx(stdscr, bite.term_height, bite.term_width);
    bite.update_buffer_areas();

    main_loop(&bite);

    
    // Free memory.

    for(size_t i = 0; i < bite.buffers.size(); i++) {
        bite.buffers[i].free_memory();
    }
    
    close_logfile();
   
    // TODO: maybe its a good idea to have signal handler
    //       because if crash happens user will go into "broken" terminal state
    //       if ncurses doesnt quit properly.
    curs_set(1); // Enable cursor.
    endwin();

    return 0;
}



