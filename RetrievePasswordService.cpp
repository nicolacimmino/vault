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
    char *clipboard = new char[ENCRYPTED_STORE_DATA_SIZE];
    memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);

    if (arg == RETRIEVE_PASSWORD_ACTION_FULL || arg == RETRIEVE_PASSWORD_ACTION_SHOW)
    {
        this->encryptedStore->get(selectedPasswordIndex, clipboard);
    }

    if (arg == RETRIEVE_PASSWORD_ACTION_PARTIAL)
    {
        char *buffer = new char[TERMINAL_WIDTH];

        this->terminal->askQuestion(TXT_ENTER_TOKENS_POS, buffer, TERMINAL_WIDTH, 0, TERMINAL_FIRST_CANVAS_LINE + 5, TERMINAL_RIGHT_HALF_FIRST_COLUMN);
        this->encryptedStore->getTokens(selectedPasswordIndex, buffer, clipboard);

        delete buffer;
    }

    if (arg == RETRIEVE_PASSWORD_ACTION_SHOW)
    {
        terminal->alert(clipboard);
        terminal->waitKeySelection();
    }
    else
    {
        this->typeClipboard(clipboard, (arg == RETRIEVE_PASSWORD_ACTION_PARTIAL) ? PASSWORD_TYPE_INTERDIGIT_DELAY_PARTIAL : 0);
    }

    delete clipboard;

    this->reportCompletion();

    return false;
}

void RetrievePasswordService::typeClipboard(char *clipboard, uint16_t interDigitDelay)
{
    this->terminal->alert(TXT_TOUCH_TO_TYPE);

    int ref = ADCTouch.read(A1, 500);

    while (true)
    {        
        if (ADCTouch.read(A1) - ref > TOUCH_THRESHOLD)
        {
            for (byte ix = 0; ix < strlen(clipboard); ix++)
            {
                Keyboard.print(clipboard[ix]);
                delay(interDigitDelay);
            }

            memset(clipboard, 0, ENCRYPTED_STORE_DATA_SIZE);
            break;
        }

        if (this->terminal->clientRequestedAbort())
        {
            break;
        }
    }
}

bool RetrievePasswordService::loop()
{
    return this->running;
}