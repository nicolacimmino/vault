#include "DeletePasswordService.h"

DeletePasswordService::DeletePasswordService(Terminal *terminal, EncryptedStore *encryptedStore, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;
    this->encryptedStore = encryptedStore;
}

bool DeletePasswordService::start(byte arg = 0)
{
    this->terminal->print(TXT_SELECT_POSITION, TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 5);

    char selection = this->terminal->waitKeySelection();

    if (selection != TERMINAL_OPERATION_ABORTED && selection >= 'a' && selection <= 'a' + ENCRYPTED_STORE_MAX_ENTRIES)
    {
        if (this->terminal->askYesNoQuestion(TXT_DELETE_PASSWORD_CONFIRMATION, true))
        {
            this->encryptedStore->wipe(selection - 'a');
        }
    }

    this->reportCompletion();
}

bool DeletePasswordService::loop()
{
    return this->running;
}