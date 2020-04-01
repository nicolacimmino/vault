#include "RetrievePasswordService.h"

RetrievePasswordService::RetrievePasswordService(Terminal *terminal, EncryptedStore *encryptedStore, byte selectedPasswordIndex, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;
    this->encryptedStore = encryptedStore;
    this->selectedPasswordIndex = selectedPasswordIndex;
}

bool RetrievePasswordService::start(byte arg = 0)
{
    if (arg != 0 && arg != 1)
    {
        this->reportCompletion();
        
        return false;
    }

    char *clipboard = new char[ENCRYPTED_STORE_DATA_SIZE];

    if (arg == 0)
    {
        this->encryptedStore->get(selectedPasswordIndex, clipboard);
    }

    if (arg == 1)
    {
        char *buffer = new char[TERMINAL_WIDTH];

        this->terminal->askQuestion(TXT_ENTER_TOKENS_POS, buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 4, TERMINAL_RIGHT_HALF_FIRST_COLUMN);
        this->encryptedStore->getTokens(selectedPasswordIndex, buffer, clipboard);

        delete buffer;
    }

    this->terminal->alert("Ready. Press button to type.");

    while (strlen(clipboard) > 0)
    {
        if (digitalRead(BUTTON_SENSE) == LOW)
        {
            for (byte ix = 0; ix < strlen(clipboard); ix++)
            {
                Keyboard.print(clipboard[ix]);
                delay(300);
            }

            memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
        }

        if (this->terminal->clientRequestedAbort())
        {
            break;
        }
    }

    delete clipboard;

    this->reportCompletion();

    return false;
}

bool RetrievePasswordService::loop()
{
    return this->running;
}