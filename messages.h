#ifndef __PGEN_MESSAGES_H__
#define __PGEN_MESSAGES_H__

const char message0[] PROGMEM =
    " __      __    _    _ _   _______ \r\n"
    " \\ \\    / /\\  | |  | | | |__   __|  \e[32mEncryption:\e[33m AES-256\r\n"
    "  \\ \\  / /  \\ | |  | | |    | |     \e[32mBlock Mode:\e[33m CTR\r\n"
    "   \\ \\/ / /\\ \\| |  | | |    | |     \e[32mDev:\e[33m Nicola Cimmino\r\n"
    "    \\  / ____ \\ |__| | |____| |     \e[32mBuilt:\e[33m " __DATE__ " " __TIME__ "\r\n"
    "     \\/_/    \\_\\____/|______|_|     \e[32mSrc:\e[33m https://github.com/nicolacimmino/pgen\r\n"
    "                \e[32m(c) Nicola 2020";

const char *const messages[] PROGMEM = {message0};

#define TXT_ENTER_MASTER_PASSWORD "Enter master password: "
#define TXT_PASSWORD_MASK '*'

#endif
