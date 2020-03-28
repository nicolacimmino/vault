#ifndef __PGEN_MESSAGES_H__
#define __PGEN_MESSAGES_H__

const char message0[] PROGMEM =
    " __      __    _    _ _   _______ \r\n"
    " \\ \\    / /\\  | |  | | | |__   __|  \e[32mEncryption:\e[33m AES-256\r\n"
    "  \\ \\  / /  \\ | |  | | |    | |     \e[32mBlock Mode:\e[33m CTR\r\n"
    "   \\ \\/ / /\\ \\| |  | | |    | |     \e[32mDev:\e[33m Nicola Cimmino\r\n"
    "    \\  / ____ \\ |__| | |____| |     \e[32mBuilt:\e[33m " __DATE__ " " __TIME__ "\r\n"
    "     \\/_/    \\_\\____/|______|_|     \e[32mSrc:\e[33m github.com/nicolacimmino/vault\r\n"
    "                \e[32m(c) Nicola 2020";

const char *const messages[] PROGMEM = {message0};

#define TXT_ENTER_MASTER_PASSWORD "Master password:"
#define TXT_PASSWORD_MASK '*'

#define TXT_WIPE_FULL_CONFIRMATION VT_FOREGROUND_RED VT_TEXT_BLINK "WARNING! " VT_TEXT_DEFAULT VT_FOREGROUND_YELLOW "This will ERASE ALL data! Sure ? (y/n)"
#define TXT_WIPE_PASSWORD_CONFIRMATION VT_FOREGROUND_RED VT_TEXT_BLINK "WARNING! " VT_TEXT_DEFAULT VT_FOREGROUND_YELLOW "This will ERASE the password! Sure ? (y/n)"
#define TXT_WIPING "Wiping storage....."
#define TXT_ENTER_TOKENS_POS "Enter tokens positions: "
#define TXT_PROGRESS_PROTOTYPE "Done: %d%%"
#define TXT_UNLOCKED_TERMINAL_HEADER_PROTOTYPE " Vault V0.1 - %d bytes free   %02u:%02u:%02u 20%02u-%02u-%02u   KFP: %04u     %s [ULK]"
#define TXT_LOCKED_TERMINAL_HEADER_PROTOTYPE " Vault V0.1 - %d bytes free                                             [LCK]"
#define TXT_ENTER_DATE "Enter Date/Time (yyyy-mm-dd hh:mm): "

#endif
