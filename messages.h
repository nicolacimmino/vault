#ifndef __PGEN_MESSAGES_H__
#define __PGEN_MESSAGES_H__

const char message0[] PROGMEM =
    "   __      __    _    _ _   _______ \r\n"
    "   \\ \\    / /\\  | |  | | | |__   __|  \e[32mEncryption:\e[33m AES-256\r\n"
    "    \\ \\  / /  \\ | |  | | |    | |     \e[32mBlock Mode:\e[33m CTR\r\n"
    "     \\ \\/ / /\\ \\| |  | | |    | |     \e[32mDev:\e[33m Nicola Cimmino\r\n"
    "      \\  / ____ \\ |__| | |____| |     \e[32mBuilt:\e[33m " __DATE__ " " __TIME__ "\r\n"
    "       \\/_/    \\_\\____/|______|_|     \e[32mSrc:\e[33m github.com/nicolacimmino/vault\r\n"
    "                  \e[32m(c) Nicola 2020";

const char message1[] PROGMEM =
    "\e[13C\e(0\x6c\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x6b\e(B\r\n"
    "\e[13C\e(0\x78                                                    \x78\e(B\r\n"
    "\e[13C\e(0\x78                                                    \x78\e(B\r\n"
    "\e[13C\e(0\x78                                                    \x78\e(B\r\n"
    "\e[13C\e(0\x78                                                    \x78\e(B\r\n"
    "\e[13C\e(0\x78                                                    \x78\e(B\r\n"
    "\e[13C\e(0\x6d\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x71\x6a\e(B\r\n";

const char *const messages[] PROGMEM = {message0, message1};

#define TXT_ENTER_MASTER_PASSWORD "Master password:"
#define TXT_PASSWORD_MASK '*'

#define TXT_WARNING VT_FOREGROUND_RED VT_TEXT_BLINK "                  WARNING!" VT_TEXT_DEFAULT VT_FOREGROUND_YELLOW 
#define TXT_WIPE_FULL_CONFIRMATION "Erase all data?  (y/n)"
#define TXT_DELETE_PASSWORD_CONFIRMATION "Delete password? (y/n)"
#define TXT_WIPING "Wiping storage....."
#define TXT_ENTER_TOKENS_POS "Enter tokens positions: "
#define TXT_PROGRESS_PROTOTYPE "Done: %d%%"
#define TXT_UNLOCKED_TERMINAL_HEADER_PROTOTYPE " Vault V0.1                            EPOOL: %3d%%  KFP: %04u  RAM: %4d bytes"
#define TXT_UNLOCKED_TERMINAL_HEADER_PROTOTYPE_NOKFP " Vault V0.1                            EPOOL: %3d%%  KFP: ****  RAM: %4d bytes"
#define TXT_LOCKED_TERMINAL_HEADER_PROTOTYPE " Vault V0.1                                                    RAM: %4d bytes"
#define TXT_ENTER_DATE "Enter Date/Time (yyyy-mm-dd hh:mm): "
#define TXT_BACKUP_FULL_CONFIRMATION "Start full backup? (y/n)"
#define TXT_BACKUP_RESTORE_CONFIRMATION "Start restore? (y/n)"
#define TXT_BACKUP_IN_PROGRESS "Backup in progress....."
#define TXT_BACKUP_RESTORE_IN_PROGRESS "Backup restore in progress....."
#define TXT_TERMINAL_STATUS_COMPLETED "[OK]"
#define TXT_PRESS_BUTTON "Ready. Press button to type."
#define TXT_SELECTED_PASSWORD_LABEL " Selected: %s"
#define TXT_MENU_PASSWORD_TO_CLIPBOARD "Copy to clipboard"
#define TXT_MENU_PASSWORD_TO_CLIPBOARD_PARTIAL "Partial copy to clipboard"
#define TXT_MENU_PASSWORD_SHOW "Show only"
#define TXT_MENU_BACK "Back"
#define TXT_SELECT_POSITION "Select position: "
#define TXT_ENTER_LABEL "Enter label: "
#define TXT_ENTER_PASSWORD "Enter password: "
#define TXT_ENCRYPTING " Enctrypting......"
#define TXT_EMPTY_LABEL "---"

#endif
