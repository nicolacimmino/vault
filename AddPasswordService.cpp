#include "AddPasswordService.h"

AddPasswordService::AddPasswordService(Terminal *terminal, EncryptedStore *encryptedStore, const Functor1<byte> &reportProgress, const Functor0 &reportCompletion)
    : Service(reportProgress, reportCompletion)
{
    this->terminal = terminal;
    this->encryptedStore = encryptedStore;
}

bool AddPasswordService::start(byte arg = 0)
{
    this->terminal->print(TXT_SELECT_POSITION, TERMINAL_FIRST_CANVAS_LINE + TERMINAL_CANVAS_LINES - 1, 2);

    char selection = this->terminal->waitKeySelection();

    if (selection != TERMINAL_OPERATION_ABORTED && selection >= 'a' && selection <= 'a' + ENCRYPTED_STORE_MAX_ENTRIES)
    {
        this->terminal->clearCanvas();

        char *label = new char[ENCRYPTED_STORE_LABEL_SIZE];
        char *password = new char[ENCRYPTED_STORE_DATA_SIZE];

        this->terminal->askQuestion(TXT_ENTER_LABEL, label, ENCRYPTED_STORE_LABEL_SIZE, 0, TERMINAL_FIRST_CANVAS_LINE + 2, 2);
        this->terminal->askQuestion(TXT_ENTER_PASSWORD, password, ENCRYPTED_STORE_DATA_SIZE, '*', TERMINAL_FIRST_CANVAS_LINE + 3, 2);

        this->terminal->printStatusMessage(TXT_ENCRYPTING);
        this->encryptedStore->set(selection - 'a', password, label);

        delete label;
        delete password;
    }

    this->reportCompletion();

    return false;
}

bool AddPasswordService::loop()
{
    return this->running;
}