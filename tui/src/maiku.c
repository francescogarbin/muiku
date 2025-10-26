/*
 * Maiku TUI - A minimalist TUI haiku viewer
 * Written in C23 with ncurses
 * 
 * Features:
 * - Left panel: 3 selectable haiku titles
 * - Right panel: Display selected haiku
 * - Navigation: Arrow keys (UP/DOWN)
 * - Exit: 'q' key
 * 
 * Compile with: gcc -std=c23 -o tuiku tuiku.c -lncurses
 */

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    const char *title;
    const char *line1;
    const char *line2;
    const char *line3;
    const char *author;
} Haiku;

static const Haiku haikus[] = {
    {
        .title = "Ancient Pond",
        .line1 = "An ancient pond",
        .line2 = "A frog jumps in",
        .line3 = "The sound of water",
        .author = "— Matsuo Bashō"
    },
    {
        .title = "First Autumn Morning",
        .line1 = "First autumn morning",
        .line2 = "The mirror I stare into",
        .line3 = "Shows my father's face",
        .author = "— Murakami Kijo"
    },
    {
        .title = "Terminal Love",
        .line1 = "Cursor blinking slow",
        .line2 = "Segfault in my heart malloc",
        .line3 = "Free() cannot help",
        .author = "— Anonymous Hacker"
    }
};

#define NUM_HAIKUS (sizeof(haikus) / sizeof(haikus[0]))

// Disegna una finestra con bordo
void draw_window_border(WINDOW *win, const char *title) {
    box(win, 0, 0);
    
    // Scrivi il titolo centrato in alto
    if (title) {
        int width = getmaxx(win);
        int title_len = strlen(title);
        int x = (width - title_len) / 2;
        
        wattron(win, A_BOLD);
        mvwprintw(win, 0, x, " %s ", title);
        wattroff(win, A_BOLD);
    }
    
    wrefresh(win);
}

// Funzione per disegnare il pannello sinistro con la lista
void draw_left_panel(WINDOW *win, int selected) {
    werase(win);
    draw_window_border(win, "Haiku Menu");
    
    int y = 2;  // Inizio scrittura dopo il bordo
    
    for (size_t i = 0; i < NUM_HAIKUS; i++) {
        // Se questa voce è selezionata, invertila
        if (i == selected) {
            wattron(win, A_REVERSE | A_BOLD);
        }
        
        // Disegna il numero e il titolo
        mvwprintw(win, y + i * 2, 2, "%zu. %s", i + 1, haikus[i].title);
        
        if (i == selected) {
            wattroff(win, A_REVERSE | A_BOLD);
        }
    }
    
    // Istruzioni in basso
    int height = getmaxy(win);
    wattron(win, A_DIM);
    mvwprintw(win, height - 2, 2, "up/down: Navigate");
    mvwprintw(win, height - 1, 2, "q: Quit");
    wattroff(win, A_DIM);
    
    wrefresh(win);
}

// Funzione per disegnare il pannello destro con l'haiku
void draw_right_panel(WINDOW *win, int selected) {
    werase(win);
    draw_window_border(win, "Haiku");
    
    const Haiku *h = &haikus[selected];
    
    int height = getmaxy(win);
    int width = getmaxx(win);
    
    // Calcola posizione centrale verticale
    int center_y = height / 2 - 2;
    
    // Disegna il titolo dell'haiku
    wattron(win, A_BOLD | A_UNDERLINE);
    int title_len = strlen(h->title);
    mvwprintw(win, center_y - 2, (width - title_len) / 2, "%s", h->title);
    wattroff(win, A_BOLD | A_UNDERLINE);
    
    // Disegna le tre linee dell'haiku centrate
    int line1_len = strlen(h->line1);
    int line2_len = strlen(h->line2);
    int line3_len = strlen(h->line3);
    
    wattron(win, COLOR_PAIR(1));
    mvwprintw(win, center_y, (width - line1_len) / 2, "%s", h->line1);
    mvwprintw(win, center_y + 1, (width - line2_len) / 2, "%s", h->line2);
    mvwprintw(win, center_y + 2, (width - line3_len) / 2, "%s", h->line3);
    wattroff(win, COLOR_PAIR(1));
    
    // Disegna l'autore in basso
    wattron(win, A_DIM | A_ITALIC);
    int author_len = strlen(h->author);
    mvwprintw(win, center_y + 4, (width - author_len) / 2, "%s", h->author);
    wattroff(win, A_DIM | A_ITALIC);
    
    wrefresh(win);
}

int main(void) {
    // Inizializza ncurses
    initscr();              // Inizializza lo schermo
    cbreak();               // Disabilita line buffering
    noecho();               // Non mostrare l'input
    keypad(stdscr, TRUE);   // Abilita i tasti speciali (frecce, F1, etc)
    curs_set(0);            // Nascondi il cursore
    
    // Inizializza i colori se supportati
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);  // Colore per l'haiku
    }
    
    // Ottieni dimensioni dello schermo
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    
    // Calcola dimensioni dei pannelli
    // Left: 30% della larghezza, Right: 70%
    int left_width = max_x * 3 / 10;
    int right_width = max_x - left_width;
    
    // Crea le due finestre (pannelli)
    WINDOW *left_win = newwin(max_y, left_width, 0, 0);
    WINDOW *right_win = newwin(max_y, right_width, 0, left_width);
    
    // Stato dell'applicazione
    int selected = 0;  // Indice dell'haiku selezionato
    bool running = true;
    
    // Main loop
    while (running) {
        // Ridisegna i pannelli
        draw_left_panel(left_win, selected);
        draw_right_panel(right_win, selected);
        
        // Aspetta input dall'utente
        int ch = getch();
        
        switch (ch) {
            case KEY_UP:
                // Vai su nella lista (con wrap-around)
                selected = (selected - 1 + NUM_HAIKUS) % NUM_HAIKUS;
                break;
                
            case KEY_DOWN:
                // Vai giù nella lista (con wrap-around)
                selected = (selected + 1) % NUM_HAIKUS;
                break;
                
            case 'q':
            case 'Q':
                // Esci dal programma
                running = false;
                break;
                
            case KEY_RESIZE:
                // Gestisci il resize del terminale
                getmaxyx(stdscr, max_y, max_x);
                left_width = max_x * 3 / 10;
                right_width = max_x - left_width;
                
                // Ridimensiona le finestre
                wresize(left_win, max_y, left_width);
                wresize(right_win, max_y, right_width);
                mvwin(right_win, 0, left_width);
                
                // Forza il refresh completo
                clear();
                refresh();
                break;
        }
    }
    
    // Cleanup: distruggi le finestre e termina ncurses
    delwin(left_win);
    delwin(right_win);
    endwin();
    
    return EXIT_SUCCESS;
}
